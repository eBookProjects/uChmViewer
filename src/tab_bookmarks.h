/**************************************************************************
 *  Kchmviewer - a CHM file viewer with broad language support            *
 *  Copyright (C) 2004-2010 George Yunaev, kchmviewer@ulduzsoft.com       *
 *                                                                        *
 *  This program is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 3 of the License, or     *
 *  (at your option) any later version.                                   *
 *																	      *
 *  This program is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 **************************************************************************/

#ifndef KCHMBOOKMARKWINDOW_H
#define KCHMBOOKMARKWINDOW_H

#include "kde-qt.h"
#include "settings.h"

#include "ui_tab_bookmarks.h"


class KCHMBookmarkWindow : public QWidget, public Ui::TabBookmarks
{
	Q_OBJECT
	public:
		KCHMBookmarkWindow( QWidget *parent );
		virtual ~KCHMBookmarkWindow() {};
	
		void 	createMenu( QMenu * menuBookmarks );
		
		void	restoreSettings (const KCHMSettings::bookmark_saved_settings_t& settings);
		void	saveSettings (KCHMSettings::bookmark_saved_settings_t& settings);
		void	invalidate();
		
	public slots:
		void 	onAddBookmarkPressed ();
	
	private slots:
		void	actionBookmarkActivated();
		void	onDelBookmarkPressed( );
		void	onEditBookmarkPressed( );
		void	onItemDoubleClicked ( QListWidgetItem* );
		void	onContextMenuRequested ( const QPoint &point );

	private:
		QMenu 		*	m_menuBookmarks;
		QMenu 		* 	m_contextMenu;
		QString			m_bookmarkFileName;
		bool			m_listChanged;
};

#endif