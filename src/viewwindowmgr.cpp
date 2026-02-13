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

#include <functional>
#include <cstdlib>
#include <stdexcept>

#include <QAction>
#include <QApplication>
#include <QCheckBox>
#include <QClipboard>
#include <QColor>
#include <QFrame>
#include <QIcon>
#include <QKeySequence>
#include <QLabel>
#include <QList>
#include <QLineEdit>
#include <QMenu>
#include <QMouseEvent>
#include <QObject>
#include <QPalette>
#include <QString>
#include <QTabBar>
#include <QTabWidget>
#include <QToolButton>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>
#include <Qt>
#include <QtGlobal>

class QMouseEvent;
class QPoint;

#include <browser-settings.hpp>
#include <browser-types.hpp>
#include <ebook.h>

#include "config.h"
#include "i18n.h"
#include "mainwindow.h"
#include "settings.h"
#include "viewwindow.h"

#include "ui_window_browser.h"

#include "viewwindowmgr.h"


// A small overridden class to handle a middle click
class ViewWindowTabWidget : public QTabWidget
{
	public:
		ViewWindowTabWidget( QWidget* parent ) : QTabWidget( parent ) {}

	protected:
		void mouseReleaseEvent( QMouseEvent* event )
		{
			if ( event->button() == Qt::MiddleButton )
			{
				int tab = tabBar()->tabAt( event->pos() );

				if ( tab != -1 )
					emit tabCloseRequested( tab );
			}
		}
};


ViewWindowMgr::ViewWindowMgr( QWidget* parent )
	: QWidget( parent ), Ui::TabbedBrowser()
{
	// UIC
	setupUi( this );

	// Set up the initial settings
	applyBrowserSettings();

	// Create the tab widget
	m_tabWidget = new ViewWindowTabWidget( this );
	verticalLayout->insertWidget( 0, m_tabWidget, 10 );

	// on current tab changed
	connect( m_tabWidget, SIGNAL( currentChanged( int ) ), this, SLOT( onTabChanged( int ) ) );
	connect( m_tabWidget, SIGNAL( tabCloseRequested( int ) ), this, SLOT( onCloseWindow( int ) ) );

	// Create a "new tab" button
	QToolButton* newButton = new QToolButton( this );
	newButton->setCursor( Qt::ArrowCursor );
	newButton->setAutoRaise( true );
	newButton->setIcon( QIcon( ":/images/addtab.png" ) );
	newButton->setToolTip( i18n( "Add page" ) );
	connect( newButton, SIGNAL( clicked() ), this, SLOT( openNewTab() ) );

	// Put it there
	m_tabWidget->setCornerWidget( newButton, Qt::TopLeftCorner );

	// Hide the search frame
	frameFind->setVisible( false );

	// Search Line edit
	connect( editFind,
	         SIGNAL( textEdited( const QString& ) ),
	         this,
	         SLOT( editTextEdited( const QString& ) ) );

	connect( editFind, SIGNAL( returnPressed() ), this, SLOT( onFindNext() ) );

	// Search toolbar buttons
	toolClose->setShortcut( Qt::Key_Escape );
	connect( toolClose, SIGNAL( clicked() ), this, SLOT( closeSearch() ) );

	connect( toolPrevious, SIGNAL( clicked() ), this, SLOT( onFindPrevious() ) );
	connect( toolNext, SIGNAL( clicked() ), this, SLOT( onFindNext() ) );
}

ViewWindowMgr::~ViewWindowMgr( )
{
}

bool ViewWindowMgr::isEmpty() const
{
	return m_Windows.isEmpty();
}

void ViewWindowMgr::createMenu( QMenu* menuWindow, QAction* actionCloseWindow )
{
	m_menuWindow = menuWindow;
	m_actionCloseWindow = actionCloseWindow;
}

void ViewWindowMgr::invalidate()
{
	closeAllWindows();
	addNewTab( true );
}

ViewWindow* ViewWindowMgr::current()
{
	try
	{
		return findTabData( m_tabWidget->currentWidget() ).browser;
	}
	catch ( const std::invalid_argument& )
	{
		abort();
	}
}

ViewWindow* ViewWindowMgr::addNewTab( bool set_active )
{
	ViewWindow* browser = new ViewWindow( m_tabWidget );
	browser->setAcceptDrops( false );

	editFind->installEventFilter( this );

	// Create the tab data structure
	TabData tabdata;
	tabdata.browser = browser;
	tabdata.action = new QAction( "window", this ); // temporary name; real name is set in setTabName
	tabdata.widget = browser;

	connect( tabdata.action,
	         SIGNAL( triggered() ),
	         this,
	         SLOT( activateWindow() ) );

	m_Windows.push_back( tabdata );
	m_tabWidget->addTab( tabdata.widget, "" );
	Q_ASSERT( m_Windows.size() == m_tabWidget->count() );

	// Set active if it is the first tab
	if ( set_active || m_Windows.size() == 1 )
		m_tabWidget->setCurrentWidget( tabdata.widget );

	// Handle clicking on link in browser window
	connect( browser, &ViewWindow::linkClicked,
	         [browser, this]( const QUrl & link, UBrowser::OpenMode mode )
	         { emit linkClicked( browser, link, mode ); } );

	connect( browser, &ViewWindow::urlChanged,
	         [browser, this]( const QUrl & url )
	         { onBrowserUrlChanged( browser, url ); } );

	connect( browser, &ViewWindow::loadFinished,
	         [browser, this]( bool success )
	         { onBrowserLoadFinished( browser, success ); } );

	connect( browser, &ViewWindow::contextMenuRequested,
	         [browser, this]( const QPoint & pos, const QUrl & link )
	         { emit contextMenuRequested( browser, pos, link ); } );

	// Set up the accelerator if we have room
	if ( m_Windows.size() < 10 )
		tabdata.action->setShortcut( QKeySequence( i18n( "Alt+%1" ).arg( m_Windows.size() ) ) );

	// Add it to the "Windows" menu
	m_menuWindow->addAction( tabdata.action );

	return browser;
}

void ViewWindowMgr::closeAllWindows( )
{
	while ( m_Windows.begin() != m_Windows.end() )
		closeWindow( m_Windows.first().widget );
}

void ViewWindowMgr::setTabName( ViewWindow* browser )
{
	try
	{
		const TabData& tab = findTabData( browser );
		QString title = browser->title().trimmed();

		// Trim too long string
		if ( title.length() > 25 )
			title = title.left( 22 ) + "...";

		m_tabWidget->setTabText( m_tabWidget->indexOf( browser ), title );
		tab.action->setText( title );
		updateCloseButtons();
	}
	catch ( const std::invalid_argument& )
	{
	}
}

void ViewWindowMgr::onCloseCurrentWindow( )
{
	// Do not allow to close the last window
	if ( m_Windows.size() == 1 )
		return;

	try
	{
		closeTab( findTabData( m_tabWidget->currentWidget() ) );
	}
	catch ( const std::invalid_argument& )
	{
	}
}

void ViewWindowMgr::onCloseWindow( int num )
{
	// Do not allow to close the last window
	if ( m_Windows.size() == 1 )
		return;

	try
	{
		closeTab( findTabData( num ) );
	}
	catch ( const std::invalid_argument& )
	{
	}
}

void ViewWindowMgr::closeWindow( QWidget* widget )
{
	try
	{
		closeTab( findTabData( widget ) );
	}
	catch ( const std::invalid_argument& )
	{
		qFatal( "ViewWindowMgr::closeWindow called with unknown widget!" );
	}
}

void ViewWindowMgr::closeTab( const TabData& data )
{
	m_menuWindow->removeAction( data.action );

	m_tabWidget->removeTab( m_tabWidget->indexOf( data.widget ) );
	delete data.browser;
	delete data.action;

	m_Windows.removeOne( data );
	updateCloseButtons();

	// Change the accelerators, as we might have removed the item in the middle
	int count = 1;

	for ( WindowsIterator it = m_Windows.begin(); it != m_Windows.end() && count < 10; ++it, count++ )
		( *it ).action->setShortcut( QKeySequence( i18n( "Alt+%1" ).arg( count ) ) );
}

void ViewWindowMgr::restoreSettings( EBook* ebook, const Settings::viewindow_saved_settings_t& settings )
{
	// Destroy automatically created tab
	closeWindow( m_Windows.first().widget );

	for ( int i = 0; i < settings.size(); i++ )
	{
		ViewWindow* browser = addNewTab( false );

		if ( browser == nullptr )
			break;

		// This confusing code fixes a URL that could contain a relative path.
		// This behavior  was previously present in the EBook_EPUB class, and
		// now the EBook_EPUB::pathToUrl function first tries to find the page
		// by absolute path, and if that fails, by relative path.
		QUrl url( settings[i].url );
		QString path = url.path();

		if ( url.hasFragment() )
			path.append( "#" ).append( url.fragment() );

		browser->load( ebook->pathToUrl( path ) ); // will call setTabName()
		browser->setAutoScroll( settings[i].scroll_y );
		browser->setZoomFactor( settings[i].zoom );
	}
}

void ViewWindowMgr::saveSettings( Settings::viewindow_saved_settings_t& settings )
{
	settings.clear();

	try
	{
		for ( int i = 0; i < m_tabWidget->count(); i++ )
		{
			const TabData& tab = findTabData( i );
			settings.push_back( Settings::SavedViewWindow( tab.browser->url().toString(),
			                                               tab.browser->scrollTop(),
			                                               tab.browser->zoomFactor() ) );
		}
	}
	catch ( const std::invalid_argument& )
	{
	}
}

void ViewWindowMgr::updateCloseButtons( )
{
	bool enabled = m_Windows.size() > 1;

	m_actionCloseWindow->setEnabled( enabled );
	m_tabWidget->setTabsClosable( enabled );
}

void ViewWindowMgr::onTabChanged( int newtabIndex )
{
	if ( newtabIndex == -1 )
		return;

	try
	{
		const TabData& tab = findTabData( newtabIndex );
		emit historyChanged();
		emit urlChanged( tab.browser->url() );
		emit browserChanged( tab.browser );
		tab.widget->setFocus();
	}
	catch ( const std::invalid_argument& )
	{
	}
}

void ViewWindowMgr::onBrowserUrlChanged( ViewWindow* browser, const QUrl& url )
{
	if ( browser == current() )
	{
		emit urlChanged( url );
		emit historyChanged();
	}
}

void ViewWindowMgr::onBrowserLoadFinished( ViewWindow* browser, bool success )
{
	setTabName( browser );
	emit loadFinished( browser, success );
}

void ViewWindowMgr::openNewTab()
{
	if ( isEmpty() )
		return;

	::mainWindow->openPage( current()->url(), UBrowser::OPEN_IN_NEW );
}

void ViewWindowMgr::activateWindow()
{
	QAction* action = qobject_cast< QAction* >( sender() );

	for ( WindowsIterator it = m_Windows.begin(); it != m_Windows.end(); ++it )
	{
		if ( it->action != action )
			continue;

		QWidget* widget = it->widget;
		m_tabWidget->setCurrentWidget( widget );
		break;
	}
}

void ViewWindowMgr::closeSearch()
{
	frameFind->hide();
	m_tabWidget->currentWidget()->setFocus();
}

ViewWindowMgr::TabData ViewWindowMgr::findTabData( QWidget* widget ) noexcept( false )
{
	for ( int i = 0; i < m_Windows.size(); ++i )
		if ( m_Windows[i].widget == widget )
			return m_Windows[i];

	throw std::invalid_argument( "" );
}

ViewWindowMgr::TabData ViewWindowMgr::findTabData( ViewWindow* browser ) noexcept( false )
{
	for ( int i = 0; i < m_Windows.size(); ++i )
		if ( m_Windows[i].browser == browser )
			return m_Windows[i];

	throw std::invalid_argument( "" );
}

ViewWindowMgr::TabData ViewWindowMgr::findTabData( int tabIndex ) noexcept( false )
{
	return findTabData( m_tabWidget->widget( tabIndex ) );
}

void ViewWindowMgr::setCurrentPage( int index )
{
	m_tabWidget->setCurrentIndex( index );
}

int ViewWindowMgr::currentPageIndex() const
{
	return m_tabWidget->currentIndex();
}

void ViewWindowMgr::onActivateFind()
{
	frameFind->show();
	labelWrapped->setVisible( false );
	editFind->setFocus( Qt::ShortcutFocusReason );
	editFind->selectAll();
}

void ViewWindowMgr::findResult( bool found, bool wrapped )
{
	if ( !frameFind->isVisible() )
		frameFind->show();

	if ( wrapped )
		labelWrapped->show();

	QPalette p = editFind->palette();

	if ( !found )
		p.setColor( QPalette::Active, QPalette::Base, QColor( 255, 102, 102 ) );
	else
		p.setColor( QPalette::Active, QPalette::Base, Qt::white );

	editFind->setPalette( p );
}

void ViewWindowMgr::find( bool backward )
{
	if ( isEmpty() )
		return;

	// Pre-hide the wrapper
	labelWrapped->hide();

	current()->findText( editFind->text(),
	                     backward, checkCase->isChecked(),
	                     pConfig->browser.highlightSearchResults,
	                     [ = ]( bool found, bool wrapped ) { findResult( found, wrapped ); } );
}

void ViewWindowMgr::editTextEdited( const QString& )
{
	find();
}

void ViewWindowMgr::onFindNext()
{
	find();
}

void ViewWindowMgr::onFindPrevious()
{
	find( true );
}

void ViewWindowMgr::copyUrlToClipboard()
{
	if ( isEmpty() )
		return;

	QString url = current()->url().toString();

	if ( !url.isEmpty() )
		QApplication::clipboard()->setText( url );
}

void ViewWindowMgr::applyBrowserSettings()
{
	ViewWindow::applySettings( pConfig->browser );
}
