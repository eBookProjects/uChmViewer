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

#ifndef TREEITEM_TOC_H
#define TREEITEM_TOC_H

#include <QString>
#include <QTreeWidget>
#include <QUrl>

class QVariant;


class TreeItem_TOC : public QTreeWidgetItem
{
	public:
		TreeItem_TOC( QTreeWidgetItem* parent, QTreeWidgetItem* after, const QString& name, const QList<QUrl>& urls, int image );
		TreeItem_TOC( QTreeWidget* parent, QTreeWidgetItem* after, const QString& name, const QList<QUrl>& urls, int image );

		QUrl        getUrl( bool isFirst = true ) const;
		bool        containsUrl( const QUrl& url, bool ignorefragment ) const;
		QUrl        findPrevUrl( const QUrl& url );
		QUrl        findNextUrl( const QUrl& url );

		// Overridden methods
		int         columnCount() const;
		QVariant    data( int column, int role ) const;

	private:
		QString     m_name;
		QList<QUrl> m_urls;
		int         m_image;
};

#endif // TREEITEM_TOC_H
