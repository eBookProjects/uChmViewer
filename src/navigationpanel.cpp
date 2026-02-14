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

#include <QDockWidget>
#include <QList>
#include <QStringList>
#include <QTabWidget>
#include <QUrl>
#include <QtGlobal>

#include <ebook.h>

#include "i18n.h"
#include "settings.h"
#include "tab_bookmarks.h"
#include "tab_contents.h"
#include "tab_index.h"
#include "tab_search.h"
#include "treeitem_toc.h"

#include "navigationpanel.h"


NavigationPanel::NavigationPanel( QWidget* parent )
	: QDockWidget( parent ), Ui::NavigatorPanel()
{
	setupUi( this );

	// There are tabs in UI-creared widget
	m_tabWidget->clear();

	// Add the required tabs
	m_searchTab = new TabSearch( m_tabWidget );
	m_tabWidget->addTab( m_searchTab, i18n( "Search" ) );

	m_bookmarksTab = new TabBookmarks( m_tabWidget );
	m_tabWidget->addTab( m_bookmarksTab, i18n( "Bookmarks" ) );

	// Those tabs will be added later
	m_contentsTab = 0;
	m_indexTab = 0;
}

void NavigationPanel::setBookmarkMenu( QMenu* menu )
{
	m_bookmarksTab->createMenu( menu );
}

void NavigationPanel::invalidate()
{
	if ( m_contentsTab )
	{
		m_tabWidget->removeTab( m_tabWidget->indexOf( m_contentsTab ) );
		delete m_contentsTab;
		m_contentsTab = 0;
	}

	if ( m_indexTab )
	{
		m_tabWidget->removeTab( m_tabWidget->indexOf( m_indexTab ) );
		delete m_indexTab;
		m_indexTab = 0;
	}

	m_searchTab->invalidate();
	m_bookmarksTab->invalidate();
}

void NavigationPanel::updateTabs( EBook* file )
{
	invalidate();

	// Insert index first
	if ( file->hasFeature( EBook::FEATURE_INDEX ) )
	{
		m_indexTab = new TabIndex( m_tabWidget );
		m_tabWidget->insertTab( 0, m_indexTab, i18n( "Index" ) );
	}

	if ( file->hasFeature( EBook::FEATURE_TOC ) )
	{
		m_contentsTab = new TabContents( m_tabWidget );
		m_tabWidget->insertTab( 0, m_contentsTab, i18n( "Contents" ) );
	}
}

void NavigationPanel::applySettings( Settings* settings )
{
	m_searchTab->restoreSettings( settings->m_searchhistory );
	m_bookmarksTab->restoreSettings( settings->m_bookmarks );
}

void NavigationPanel::getSettings( Settings* settings )
{
	m_searchTab->saveSettings( settings->m_searchhistory );
	m_bookmarksTab->saveSettings( settings->m_bookmarks );
}

void NavigationPanel::refresh()
{
	if ( m_indexTab )
		m_indexTab->refillIndex();

	if ( m_contentsTab )
		m_contentsTab->refillTableOfContents();
}

bool NavigationPanel::findUrlInContents( const QUrl& url )
{
	if ( !m_contentsTab )
		return false;

	if ( m_contentsTab->currentUrl() == url )
		return true;

	TreeItem_TOC* treeitem = m_contentsTab->getTreeItem( url );

	if ( treeitem )
	{
		TreeItem_TOC* itemparent = treeitem;

		while ( ( itemparent = ( TreeItem_TOC* ) itemparent->parent() ) != 0 )
			itemparent->setExpanded( true );

		m_contentsTab->showItem( treeitem );
		return true;
	}

	m_contentsTab->showItem( nullptr );
	return false;
}

void NavigationPanel::addBookmark()
{
	m_bookmarksTab->onAddBookmarkPressed();
}

int NavigationPanel::active() const
{
	QWidget* cur = m_tabWidget->currentWidget();

	if ( cur == m_bookmarksTab )
		return TAB_BOOKMARK;
	else if ( cur == m_searchTab )
		return TAB_SEARCH;
	else if ( cur == m_indexTab )
		return TAB_INDEX;

	return TAB_CONTENTS;
}

void NavigationPanel::setActive( int index )
{
	switch ( index )
	{
	case TAB_CONTENTS:
		if ( m_contentsTab )
		{
			m_tabWidget->setCurrentWidget( m_contentsTab );
			m_contentsTab->focus();
		}

		break;

	case TAB_INDEX:
		if ( m_indexTab )
		{
			m_tabWidget->setCurrentWidget( m_indexTab );
			m_indexTab->focus();
		}

		break;

	case TAB_SEARCH:
		m_tabWidget->setCurrentWidget( m_searchTab );
		m_searchTab->focus();
		break;

	case TAB_BOOKMARK:
		m_tabWidget->setCurrentWidget( m_bookmarksTab );
		m_bookmarksTab->focus();
		break;
	}
}

void NavigationPanel::findTextInContents( const QString& text )
{
	if ( m_contentsTab )
		m_contentsTab->search( text );
}

void NavigationPanel::findInIndex( const QString& text )
{
	if ( m_indexTab )
		m_indexTab->search( text );
}

void NavigationPanel::executeQueryInSearch( const QString& text )
{
	m_searchTab->execSearchQueryInGui( text );
}

QStringList NavigationPanel::searchQuery( const QString& text )
{
	QList< QUrl > res;
	QStringList result;
	m_searchTab->searchQuery( text, &res );

	Q_FOREACH ( QUrl u, res )
		result.push_back( u.path() );

	return result;
}
