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
#include <QMouseEvent>
#include <QPalette>
#include <QString>
#include <QUrl>
#include <QWebFrame>
#include <QWebHistory>
#include <QWebPage>
#include <QWebSettings>
#include <Qt>
#include <QtGlobal>

class QPrinter;

#include <browser-settings.hpp>
#include <browser-types.hpp>
#include <ebook.h>

#include "../mainwindow.h"
#include "../viewwindowmgr.h"
#include "dataprovider.h"

#include "viewwindow.h"


static const qreal ZOOM_FACTOR_CHANGE = 0.1;

ViewWindow::ViewWindow( QWidget* parent )
	: QWebView ( parent )
{
	invalidate();
	m_storedScrollbarPosition = 0;

	// Use our network emulation layer
	page()->setNetworkAccessManager( new KCHMNetworkAccessManager( this ) );

	// All links are going through us
	page()->setLinkDelegationPolicy( QWebPage::DelegateAllLinks );

	connect( this, SIGNAL( loadFinished( bool ) ), this, SLOT( onLoadFinished( bool ) ) );
	connect( this, &QWebView::linkClicked,
	         [this]( const QUrl & link )
	{
		if ( ( QApplication::keyboardModifiers() & Qt::ShiftModifier ) != 0 )
			emit linkClicked( link, UBrowser::OPEN_IN_NEW );
		else if ( ( QApplication::keyboardModifiers() & Qt::ControlModifier ) != 0 )
			emit linkClicked( link, UBrowser::OPEN_IN_BACKGROUND );
		else
			emit linkClicked( link, UBrowser::OPEN_IN_CURRENT );
	} );

	// Search results highlighter
	QPalette pal = palette();
	pal.setColor( QPalette::Inactive, QPalette::Highlight, pal.color( QPalette::Active, QPalette::Highlight ) );
	pal.setColor( QPalette::Inactive, QPalette::HighlightedText, pal.color( QPalette::Active, QPalette::HighlightedText ) );
	setPalette( pal );
}

ViewWindow::~ViewWindow()
{
}

void ViewWindow::invalidate( )
{
	m_storedScrollbarPosition = 0;
	reload();
}

void ViewWindow::load( const QUrl& url )
{
	//qDebug("ViewWindow::openUrl %s", qPrintable(url.toString()));

	// Do not use setContent() here, it resets QWebHistory
	QWebView::load( url );

	mainWindow->viewWindowMgr()->setTabName( this );
}

void ViewWindow::applySettings( BrowserSettings& settings )
{
	QWebSettings* setup = QWebSettings::globalSettings();

	setup->setAttribute( QWebSettings::AutoLoadImages, settings.enableImages );
	setup->setAttribute( QWebSettings::JavascriptEnabled, settings.enableJS );
	setup->setAttribute( QWebSettings::JavaEnabled, settings.enableJava );
	setup->setAttribute( QWebSettings::PluginsEnabled, settings.enablePlugins );
	setup->setAttribute( QWebSettings::OfflineStorageDatabaseEnabled, settings.enableOfflineStorage );
	setup->setAttribute( QWebSettings::LocalStorageDatabaseEnabled, settings.enableLocalStorage );
	setup->setAttribute( QWebSettings::LocalStorageEnabled, settings.enableLocalStorage );
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

void ViewWindow::print( QPrinter* printer, std::function<void ( bool success )> result )
{
	QWebView::print( printer );
	result( true );
}

void ViewWindow::setZoomFactor( qreal zoom )
{
	QWebView::setZoomFactor( zoom );
}

qreal ViewWindow::zoomFactor() const
{
	return QWebView::zoomFactor();
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
	return page()->currentFrame()->scrollBarValue( Qt::Vertical );
}

void ViewWindow::setScrollTop( int pos )
{
	page()->currentFrame()->setScrollBarValue( Qt::Vertical, pos );
}

void ViewWindow::setAutoScroll( int pos )
{
	m_storedScrollbarPosition = pos;
}

void ViewWindow::findText( const QString& text,
                           bool backward,
                           bool caseSensitively,
                           bool highlightSearchResults,
                           std::function<void ( bool found, bool wrapped )> result )
{
	QWebPage::FindFlags webkitflags;

	if ( caseSensitively )
		webkitflags |= QWebPage::FindCaseSensitively;

	if ( backward )
		webkitflags |= QWebPage::FindBackward;

	if ( highlightSearchResults )
	{
		// From the doc:
		// If the HighlightAllOccurrences flag is passed, the
		// function will highlight all occurrences that exist
		// in the page. All subsequent calls will extend the
		// highlight, rather than replace it, with occurrences
		// of the new string.

		// If the search text is different, we run the empty string search
		// to discard old highlighting
		if ( m_lastSearchedWord != text )
			QWebView::findText( "", webkitflags | QWebPage::HighlightAllOccurrences );

		m_lastSearchedWord = text;

		// Now we call search with highlighting enabled, while the main search below will have
		// it disabled. This leads in both having the highlighting results AND working forward/
		// backward buttons.
		QWebView::findText( text, webkitflags | QWebPage::HighlightAllOccurrences );
	}

	bool found = QWebView::findText( text, webkitflags );
	bool wrapped = false;

	// If we didn't find anything, enable the wrap and try again
	if ( !found )
	{
		found = QWebView::findText( text, webkitflags | QWebPage::FindWrapsAroundDocument );
		wrapped = found;
	}

	result( found, wrapped );
}

void ViewWindow::selectAll()
{
	triggerPageAction( QWebPage::SelectAll );
}

void ViewWindow::selectedCopy()
{
	triggerPageAction( QWebPage::Copy );
}

QUrl ViewWindow::anchorAt( const QPoint& pos )
{
	QWebHitTestResult res = page()->currentFrame()->hitTestContent( pos );

	if ( !res.linkUrl().isValid() )
		return QUrl();

	return  res.linkUrl();
}

void ViewWindow::mouseReleaseEvent ( QMouseEvent* event )
{
	if ( event->button() == Qt::MidButton )
	{
		QUrl link = anchorAt( event->pos() );

		if ( !link.isEmpty() )
		{
			emit linkClicked( link, UBrowser::OPEN_IN_BACKGROUND );
			return;
		}
	}

	QWebView::mouseReleaseEvent( event );
}

void ViewWindow::contextMenuEvent( QContextMenuEvent* e )
{
	QUrl link = anchorAt( e->pos() );
	emit contextMenuRequested( e->globalPos(), link );
}

void ViewWindow::onLoadFinished ( bool )
{
	if ( m_storedScrollbarPosition > 0 )
	{
		page()->currentFrame()->setScrollBarValue( Qt::Vertical, m_storedScrollbarPosition );
		m_storedScrollbarPosition = 0;
	}

	emit dataLoaded( this );
}
