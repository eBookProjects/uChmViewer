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

#ifndef VIEWWINDOWMGR_H
#define VIEWWINDOWMGR_H

#include <QList>
#include <QObject>
#include <QString>
#include <QWidget>

class QAction;
class QMenu;
class QUrl;

#include "settings.h"

class MainWindow;
class ViewWindow;
class ViewWindowTabWidget;

#include "ui_window_browser.h"


class ViewWindowMgr : public QWidget, public Ui::TabbedBrowser
{
		Q_OBJECT
	public:
		ViewWindowMgr( QWidget* parent );
		~ViewWindowMgr( );

		// Returns a handle to a currently viewed window.
		// Guaranteeed to return a valid handle, or aborts.
		ViewWindow*     current();

		// Adds a new tab, creating a new browser window
		ViewWindow*     addNewTab( bool set_active );

		// Sets the tab name and updates Windows menu
		void    setTabName( ViewWindow* browser );

		void    invalidate();

		// Creates a Window menu
		void    createMenu( MainWindow* parent, QMenu* menuWindow, QAction* actionCloseWindow );

		// Saves and restores current settings between sessions
		void    restoreSettings( const Settings::viewindow_saved_settings_t& settings );
		void    saveSettings( Settings::viewindow_saved_settings_t& settings );

		void    setCurrentPage( int index );
		int     currentPageIndex() const;

		// Reloads all windows
		void    reloadAllWindows();

		// Set up the configuration settings
		void    applyBrowserSettings();

	signals:
		void    browserChanged(ViewWindow* browser);
		/**
		 * This signal is emitted when tabs are switched or when the url of
		 * the ViewWindow changes.
		 *
		 * see onBrowserUrlChanged(), onTabChanged()
		 */
		void    historyChanged();
		void    urlChanged( const QUrl& url );
		void    loadFinished( ViewWindow* browser, bool success );

	public slots:
		void    onCloseCurrentWindow();
		void    onCloseWindow( int num );
		void    onActivateFind();
		void    onFindNext();
		void    onFindPrevious();
		void    copyUrlToClipboard();

	protected slots:
		void    openNewTab();
		void    onTabChanged( int newtabIndex );
		//! Connected to ViewWindow::urlChanged()
		void    onBrowserUrlChanged( ViewWindow* browser, const QUrl& url );
		void    onBrowserLoadFinished( ViewWindow* browser, bool success );
		void    updateCloseButtons();
		void    activateWindow();
		void    closeSearch();

		void    editTextEdited( const QString& text );

	private:
		void    find( bool backward = false );

		struct TabData
		{
			QWidget*                widget;
			ViewWindow*             browser;
			QAction*                action;

			bool operator==(const TabData& an) const
			{
				return widget == an.widget && browser == an.browser;
			}
		};

		void    closeAllWindows();
		void    closeWindow( QWidget* widget );
		void    closeTab( const TabData& data );
		TabData findTabData( QWidget* widget ) noexcept(false);
		TabData findTabData( ViewWindow* browser ) noexcept(false);
		TabData findTabData( int tabIndex ) noexcept(false);

		// Storage of all available windows
		QList< TabData >    m_Windows;
		typedef QList< TabData >::iterator WindowsIterator;

		QMenu*                  m_menuWindow;
		QAction*                    m_actionCloseWindow;

		// Window menu actions. Contains one action per window. They are not
		// linked permanently - if a middle window is deleted, all the following
		// actions will be relinked and replaced.
		QList< QAction* >       m_actions;

		// Last word searched
		QString                 m_lastSearchedWord;

		ViewWindowTabWidget*        m_tabWidget;
};

#endif /* INCLUDE_KCHMVIEWWINDOWMGR_H */
