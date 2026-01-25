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

#include <QMap>
#include <QtGlobal>
// As long as qAsConst is used.
// IWYU pragma: no_include "type_traits"
// IWYU pragma: no_include <type_traits>
#if (QT_VERSION < QT_VERSION_CHECK(5, 7, 0))
	#define qAsConst(s) (s)
#endif

#include "ebook.h"
#include "ebook_chm.h"
#include "ebook_epub.h"


EBook::EBook() :
	m_navigatorLoaded{false},
	m_navigator{}
{
}

EBook::~EBook()
{
}

EBook* EBook::loadFile( const QString& archiveName )
{
	EBook_CHM* cbook = new EBook_CHM();

	if ( cbook->load( archiveName ) )
		return cbook;

	delete cbook;

	EBook_EPUB* ebook = new EBook_EPUB();

	if ( ebook->load( archiveName ) )
		return ebook;

	delete ebook;
	return 0;
}

QUrl EBook::navigatorPrev( const QUrl& current )
{
	checkNavigation();
	qsizetype i = m_navigator.getIndex( current );
	return m_navigator.getUrl( --i );
}

QUrl EBook::navigatorNext( const QUrl& current )
{
	checkNavigation();
	qsizetype i = m_navigator.getIndex( current );
	return m_navigator.getUrl( i < 0 ? i : ++i );
}

void EBook::loadNavigation( Navigator& nav )
{
	QList<EBookTocEntry> toc;
	getTableOfContents( toc );

	for ( const auto& e : qAsConst( toc ) )
		nav.addUrl( e.url );
}

void EBook::checkNavigation()
{
	if ( !m_navigatorLoaded )
		loadNavigation( m_navigator );

	m_navigatorLoaded = true;
}

void EBook::Navigator::addUrl( const QUrl& url )
{
	if ( url.isEmpty() )
		return;

	if ( !isEmpty() && last() == url )
		return;

	push_back( url );
}

void EBook::Navigator::merge( const QList<EBookTocEntry>& toc )
{
	QMap<QString, qsizetype> tocUrlMap{};
	qsizetype i = 0;

	for ( const auto& e : toc )
	{
		const QString& key = e.url.toString( QUrl::RemoveFragment );

		if ( !tocUrlMap.contains( key ) )
			tocUrlMap.insert( key, i ) ;

		i++;
	}

	qsizetype vacant = 0;
	const QList<QUrl> oldUrls = *this;
	clear();

	for ( const auto& url : oldUrls )
	{
		qsizetype hit = tocUrlMap.value( url.toString(), -1 );

		if ( hit >= vacant )
		{
			for ( ; vacant < hit; ++vacant )
				addUrl( toc.at( vacant ).url );

			addUrl( url );

			for ( vacant = hit ; vacant < toc.size(); ++vacant )
			{
				const QUrl& tocUrl = toc.at( vacant ).url;

				if ( url != tocUrl.adjusted( QUrl::RemoveFragment ) )
					break;

				addUrl( tocUrl );
			}
		}
		else
			addUrl( url );
	}
}

qsizetype EBook::Navigator::getIndex( const QUrl& url )
{
	qsizetype i;

	if ( url == m_lastUrl )
		i = m_lastIndex;
	else
		i = indexOf( url );

	if ( i < 0 )
		i = indexOf( url.adjusted( QUrl::RemoveFragment ) );

	return i;
}

QUrl EBook::Navigator::getUrl( qsizetype i )
{
	if ( i < 0 || i >= size() )
		return QUrl{};

	m_lastUrl = at( i );

	m_lastIndex = i;

	return m_lastUrl;
}
