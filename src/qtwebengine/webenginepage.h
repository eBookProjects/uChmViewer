/*
 *  Kchmviewer - a CHM and EPUB file viewer with broad language support
 *  Copyright (C) 2021 Nick Egorrov, nicegorov@yandex.ru
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef QTWEBENGINE_WEBENGINEPAGE_H
#define QTWEBENGINE_WEBENGINEPAGE_H

#include <QApplication>
#include <QtDebug>
#include <QObject>
#include <Qt>
#include <QTimer>
#include <QWebEnginePage>
#include <QWebEngineProfile>

#include <browser-types.hpp>
#include <ebook_chm.h>
#include <ebook_epub.h>

#include "dataprovider.h"

#if defined PRINT_DEBUG_ALL || defined PRINT_DEBUG_WEBENGINE || defined PRINT_DEBUG_WEBENGINEPAGE
	#define PRINT_DEBUG 1
#endif

class WebEnginePage : public QWebEnginePage
{
		Q_OBJECT

	signals:
		// This signal is emitted whenever the user clicks on a link.
		void linkClicked( const QUrl& url, UBrowser::OpenMode mode );

	public:
		WebEnginePage( QObject* parent )
			: QWebEnginePage( parent )
		{
			QWebEngineProfile* pf = profile();

			if ( pf->urlSchemeHandler( EBook_CHM::URL_SCHEME_CHM ) == NULL )
			{
				pf->installUrlSchemeHandler( EBook_CHM::URL_SCHEME_CHM, new DataProvider( pf ) );
			}

			if ( pf->urlSchemeHandler( EBook_EPUB::URL_SCHEME_EPUB ) == NULL )
			{
				pf->installUrlSchemeHandler( EBook_EPUB::URL_SCHEME_EPUB, new DataProvider( pf ) );
			}

			connect( this, SIGNAL( linkHovered( const QString& ) ), this, SLOT( onLinkHovered( const QString& ) ) );
		}

		~WebEnginePage()
		{
		}

		/* Link click capture. This does not work for the right mouse button.
		 * Important! In Qt5, this function is called both when clicking with
		 * the Ctrl and/or Shift keys pressed and when clicking without keyboard
		 * modifiers, whereas in Qt6 it is called only when clicking without
		 * keyboard modifiers. In any Qt, the createWindow function is called
		 * when clicking with keyboard modifier.
		 */
		virtual bool acceptNavigationRequest( const QUrl& url, NavigationType type, bool isMainFrame ) override
		{
#if PRINT_DEBUG
			qDebug() << "[DEBUG] WebEnginePage::acceptNavigationRequest";
			qDebug() << "  url = " << url.toString();
			qDebug() << "  type  = " << type;
			qDebug() << "  isMainFrame  = " << isMainFrame;
#else
			Q_UNUSED( isMainFrame );
#endif

			if ( type == QWebEnginePage::NavigationTypeLinkClicked )
			{
				Qt::KeyboardModifiers mods = QApplication::keyboardModifiers();
				bool enableEmit = ( mods & ( Qt::ShiftModifier | Qt::ControlModifier ) ) == 0;

				/*
				 * Emitting the linkClicked signal directly from here worked until Qt 6.10.
				 * In 6.10, this sometimes caused the application to crash. The solution was
				 * to use a timer to emit the signal asynchronously. Another way was to
				 * connect the WebEnginePage::linkClicked signal to the ViewWindow::onLinkClicked
				 * slot via a queue, but this requires registering the UBrowser::OpenMode type.
				 */
				if ( enableEmit )
					emitLinkClicked( url, UBrowser::OPEN_IN_CURRENT );

				return false;
			}

			return true;
		}

		/*
		 * If the link has the "target = _new" attribute, then the WebEngine tries to create
		 * a new page using the createWindow function. However, the old page does not receive
		 * an acceptNavigationRequest unless the Ctrl or Shift key has been pressed.
		 *
		 * The createWindow function is also used from JavaScript to create a new page, but this
		 * does not work in this implementation.
		 */
		QWebEnginePage* createWindow( QWebEnginePage::WebWindowType type ) override
		{
#if PRINT_DEBUG
			qDebug() << "[DEBUG] WebEnginePage::createWindow";
			qDebug() << "  type = " << type;
#else
			Q_UNUSED( type );
#endif

			if ( !m_url.isEmpty() )
			{
				Qt::KeyboardModifiers mods = QApplication::keyboardModifiers();

				if ( ( mods & Qt::ShiftModifier ) != 0 || type == QWebEnginePage::WebBrowserTab )
					emitLinkClicked( m_url, UBrowser::OPEN_IN_NEW );
				else
					emitLinkClicked( m_url, UBrowser::OPEN_IN_BACKGROUND );
			}

			return nullptr;
		}

	protected slots:
		void    onLinkHovered( const QString& url )
		{
#if PRINT_DEBUG
			qDebug() << "[DEBUG] WebEnginePage::linkHovered";
			qDebug() << "  url = " << url;
#endif

			m_url = url;
		}

		void emitLinkClicked( const QUrl& url, UBrowser::OpenMode mode )
		{
			QTimer::singleShot( 10,
			                    [ this, url, mode ]()
			                    {
			                        emit linkClicked( url, mode );
			                    } );
		}

	protected:
		QString m_url;
};

#undef PRINT_DEBUG

#endif // QTWEBENGINE_WEBENGINEPAGE_H
