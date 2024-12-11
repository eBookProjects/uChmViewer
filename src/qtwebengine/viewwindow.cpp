/*
 *  Kchmviewer - a CHM and EPUB file viewer with broad language support
 *  Copyright (C) 2004-2014 George Yunaev, gyunaev@ulduzsoft.com
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

#include <QApplication>
#include <QContextMenuEvent>
#include <QKeySequence>
#include <QMenu>
#include <QPalette>
#include <QString>
#include <QUrl>
#include <QVariant>
#include <QWebEngineHistory>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineScript>
#include <QWebEngineSettings>
#include <QtGlobal>


#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
	#include <QWebEngineContextMenuRequest>
	#include <QWebEngineFindTextResult>
#else
	#include <QWebEngineContextMenuData>
#endif

class QPrinter;

#include <browser-settings.hpp>
#include <browser-types.hpp>
#include <ebook.h>

#include "../i18n.h"
#include "../mainwindow.h"
#include "../viewwindowmgr.h"
#include "webenginepage.h"

#include "viewwindow.h"


#if defined PRINT_DEBUG_ALL || defined PRINT_DEBUG_WEBENGINE || defined PRINT_DEBUG_WEBENGINEVIEWWINDOW
	#define PRINT_DEBUG 1
#endif

static const qreal ZOOM_FACTOR_CHANGE = 0.1;

ViewWindow::ViewWindow( QWidget* parent )
	: QWebEngineView ( parent )
{
	invalidate();
	m_contextMenu = 0;
	m_contextMenuLink = 0;
	m_storedScrollbarPosition = 0;

	WebEnginePage* page = new WebEnginePage( this );
	connect( page, SIGNAL( linkClicked ( const QUrl&, UBrowser::OpenMode ) ), this, SLOT( onLinkClicked( const QUrl&, UBrowser::OpenMode ) ) );
	setPage( page );

	connect( this, SIGNAL( loadFinished(bool)), this, SLOT( onLoadFinished(bool)) );

	// Search results highlighter
	QPalette pal = palette();
	pal.setColor( QPalette::Inactive, QPalette::Highlight, pal.color(QPalette::Active, QPalette::Highlight) );
	pal.setColor( QPalette::Inactive, QPalette::HighlightedText, pal.color(QPalette::Active, QPalette::HighlightedText) );
	setPalette( pal );
}

ViewWindow::~ViewWindow()
{
}

void ViewWindow::invalidate( )
{
	m_newTabLinkKeeper = QString();
	m_storedScrollbarPosition = 0;
	reload();
}

void ViewWindow::load ( const QUrl& url )
{
	// Do not use setContent() here, it resets QWebHistory
	QWebEngineView::load( url );

	m_newTabLinkKeeper.clear();
	mainWindow->viewWindowMgr()->setTabName( this );
}

QMenu* ViewWindow::createStandardContextMenu( QWidget* parent )
{
	QMenu* contextMenu = new QMenu( parent );

	contextMenu->addAction( "&Copy", ::mainWindow, SLOT(slotBrowserCopy()) );
	contextMenu->addAction( "&Select all", ::mainWindow, SLOT(slotBrowserSelectAll()) );

	return contextMenu;
}

QMenu* ViewWindow::getContextMenu( const QUrl& link, QWidget* parent )
{
	if ( link.isEmpty() )
	{
		// standard context menu
		if ( !m_contextMenu )
			m_contextMenu = createStandardContextMenu( parent );

		return m_contextMenu;
	}
	else
	{
		// Open in New Tab context menu
		// standard context menu
		if ( !m_contextMenuLink )
		{
			m_contextMenuLink = createStandardContextMenu( parent );
			m_contextMenuLink->addSeparator();
			m_contextMenuLink->addAction( i18n("&Open this link in a new tab"), ::mainWindow, SLOT(onOpenPageInNewTab()), QKeySequence("Shift+Enter") );
			m_contextMenuLink->addAction( i18n("&Open this link in a new background tab"), ::mainWindow, SLOT(onOpenPageInNewBackgroundTab()), QKeySequence("Ctrl+Enter") );
		}

		setTabKeeper( link );
		return m_contextMenuLink;
	}
}

QString ViewWindow::title() const
{
	QString title = ::mainWindow->chmFile()->getTopicByUrl( url() );

	// If no title is found, use the path (without the first /)
	if ( title.isEmpty() )
		title = url().path().mid( 1 );

	return title;
}

bool ViewWindow::canGoBack() const
{
	return history()->canGoBack();
}

bool ViewWindow::canGoForward() const
{
	return history()->canGoForward();
}

void ViewWindow::setTabKeeper( const QUrl& link )
{
	m_newTabLinkKeeper = link;
}

void ViewWindow::print( QPrinter* printer, std::function<void (bool success)> result )
{
#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
	page()->print( printer, [&result](bool success) {
		result( success );
	});
#else
	connect(this, &QWebEngineView::printFinished, [result](bool success) {
		result( success );
	});
	QWebEngineView::print( printer );
#endif
}

void ViewWindow::setZoomFactor(qreal zoom)
{
	QWebEngineView::setZoomFactor( zoom );
}

qreal ViewWindow::zoomFactor() const
{
	return QWebEngineView::zoomFactor();
}

void ViewWindow::zoomIncrease()
{
	setZoomFactor( zoomFactor() + ZOOM_FACTOR_CHANGE );
}

void ViewWindow::zoomDecrease()
{
	setZoomFactor( zoomFactor() - ZOOM_FACTOR_CHANGE );
}

int ViewWindow::scrollTop()
{
	QAtomicInt value = -1;

	page()->runJavaScript("document.body.scrollTop",
	                      QWebEngineScript::UserWorld,
	[&value](const QVariant & v) { value = v.toInt(); });

	while ( value == -1 )
	{
		QApplication::processEvents();
	}

	return value;
}

void ViewWindow::setScrollTop(int pos)
{
	page()->runJavaScript( QString( "document.body.scrollTop=%1" ).arg( pos )
	                       , QWebEngineScript::UserWorld );
}

void ViewWindow::setAutoScroll(int pos)
{
	m_storedScrollbarPosition = pos;
}

void ViewWindow::findText(const QString& text,
                          bool backward,
                          bool caseSensitively,
                          bool highlightSearchResults,
                          std::function<void (bool found, bool wrapped)> result)
{
	Q_UNUSED(highlightSearchResults);
	QWebEnginePage::FindFlags webkitflags;

	if ( caseSensitively )
		webkitflags |= QWebEnginePage::FindCaseSensitively;

	if ( backward )
		webkitflags |= QWebEnginePage::FindBackward;

#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
	QWebEngineView::findText( text, webkitflags,
	                          [ = ](bool found)
	{
		result(found, false);
	});
#else
	QWebEngineView::findText( text, webkitflags,
	                          [ = ](const QWebEngineFindTextResult & found)
	{
		result(found.numberOfMatches() > 0, false);
	});
#endif
}

void ViewWindow::selectAll()
{
	triggerPageAction( QWebEnginePage::SelectAll );
}

void ViewWindow::selectedCopy()
{
	triggerPageAction( QWebEnginePage::Copy );
}

void ViewWindow::contextMenuEvent(QContextMenuEvent* e)
{
	QMenu* m = new QMenu(0);

#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
	QUrl link = lastContextMenuRequest()->linkUrl();
#else
	QUrl link = page()->contextMenuData().linkUrl();
#endif

	if ( !link.isEmpty() )
	{
		m->addAction( i18n("Open Link in a new tab\tShift+LMB"), ::mainWindow, SLOT( onOpenPageInNewTab() ) );
		m->addAction( i18n("Open Link in a new background tab\tCtrl+LMB"), ::mainWindow, SLOT( onOpenPageInNewBackgroundTab() ) );
		m->addSeparator();
		setTabKeeper( link );
	}

	::mainWindow->setupPopupMenu( m );
	m->exec( e->globalPos() );
	delete m;
}

void ViewWindow::onLoadFinished ( bool )
{
	if ( m_storedScrollbarPosition > 0 )
	{
		page()->runJavaScript( QString( "document.body.scrollTop=%1" ).arg( m_storedScrollbarPosition )
		                       , QWebEngineScript::UserWorld );
		m_storedScrollbarPosition = 0;
	}

	emit urlChanged( url() );
	emit dataLoaded( this );
}

void ViewWindow::onLinkClicked(const QUrl& url, UBrowser::OpenMode mode)
{
	emit linkClicked( url, mode );
}

void ViewWindow::applySettings(BrowserSettings& settings)
{
	QWebEngineSettings* setup = QWebEngineProfile::defaultProfile()->settings();

	setup->setAttribute( QWebEngineSettings::AutoLoadImages, settings.enableImages );
	setup->setAttribute( QWebEngineSettings::JavascriptEnabled, settings.enableJS );
	setup->setAttribute( QWebEngineSettings::PluginsEnabled, settings.enablePlugins );
	setup->setAttribute( QWebEngineSettings::LocalStorageEnabled, settings.enableLocalStorage );
}
