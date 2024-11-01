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
#include <QDialog>                          // QDialog::Accepted
#include <QKeySequence>
#include <QMenu>
#include <QPalette>
#include <QPrinter>
#include <QPrintDialog>
#include <QString>
#include <QtGlobal>                         // QT_VERSION, QT_VERSION_CHECK
#include <QUrl>
#include <QVariant>
#if (QT_VERSION >= QT_VERSION_CHECK(6, 2, 0))
	#include <QWebEngineContextMenuRequest>
#else
	#include <QWebEngineContextMenuData>
#endif
#include <QWebEngineHistory>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QWebEngineScript>
#include <QWidget>

#include "i18n.h"

#include "browser-settings.hpp"  // BrowserSettings
#include "mainwindow.h"          // ::mainWindow
#include "viewwindow.h"          // ViewWindow, QWebEngineView
#include "viewwindowmgr.h"       // ViewWindowMgr
#include "webenginepage.h"          // WebEnginePage


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
	connect( page, SIGNAL( linkClicked ( const QUrl& ) ), this, SLOT( onLinkClicked( const QUrl& ) ) );
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

bool ViewWindow::openUrl ( const QUrl& url )
{
	// Do not use setContent() here, it resets QWebHistory
	load( url );

	m_newTabLinkKeeper.clear();
	mainWindow->viewWindowMgr()->setTabName( this );

	return true;
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
			m_contextMenuLink->addAction( i18n("&Open this link in a new tab"), ::mainWindow, SLOT(onOpenPageInNewTab()), QKeySequence("Ctrl+Enter") );
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

bool ViewWindow::printCurrentPage()
{
	QPrinter* printer = new QPrinter( QPrinter::HighResolution );
	QPrintDialog dlg( printer, this );
	if ( dlg.exec() != QDialog::Accepted )
	{
		::mainWindow->showInStatusBar( i18n( "Printing aborted") );
		return false;
	}

#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
	page()->print( printer, [printer](bool result) {
		Q_UNUSED(result);
		::mainWindow->showInStatusBar( i18n( "Printing finished") );
		delete printer;
	});
#else
	// TODO make slot onPrintFinished()
	print( printer );
#endif

	return true;
}

void ViewWindow::setZoomFactor(qreal zoom)
{
	QWebEngineView::setZoomFactor( zoom );
}

qreal ViewWindow::getZoomFactor() const
{
	return zoomFactor();
}

void ViewWindow::zoomIncrease()
{
	setZoomFactor( zoomFactor() + ZOOM_FACTOR_CHANGE );
}

void ViewWindow::zoomDecrease()
{
	setZoomFactor( zoomFactor() - ZOOM_FACTOR_CHANGE );
}

int ViewWindow::getScrollbarPosition()
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

void ViewWindow::setScrollbarPosition(int pos, bool force)
{
	if ( !force )
		m_storedScrollbarPosition = pos;
	else
		page()->runJavaScript( QString( "document.body.scrollTop=%1" ).arg( pos )
		                       , QWebEngineScript::UserWorld );
}

void ViewWindow::clipSelectAll()
{
	triggerPageAction( QWebEnginePage::SelectAll );
}

void ViewWindow::clipCopy()
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

void ViewWindow::onLinkClicked(const QUrl& url)
{
	emit linkClicked( url );
}

void ViewWindow::applySettings(BrowserSettings& settings)
{
	QWebEngineSettings* setup = QWebEngineProfile::defaultProfile()->settings();

	setup->setAttribute( QWebEngineSettings::AutoLoadImages, settings.enableImages );
	setup->setAttribute( QWebEngineSettings::JavascriptEnabled, settings.enableJS );
	setup->setAttribute( QWebEngineSettings::PluginsEnabled, settings.enablePlugins );
	setup->setAttribute( QWebEngineSettings::LocalStorageEnabled, settings.enableLocalStorage );
}
