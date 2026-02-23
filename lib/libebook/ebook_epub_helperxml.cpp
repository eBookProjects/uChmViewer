/*
 *  uChmViewer - a CHM and EPUB file viewer with broad language support
 *  Copyright (C) 2026  Nick Egorrov, nicegorov@yandex.ru
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

#include <QStringList>
#include <QtGlobal>

#if QT_VERSION <= QT_VERSION_CHECK(5, 14, 0)
	#define SKIP_EMPTY_PARTS ( QString::SkipEmptyParts )
#else
	#include <Qt>
	#define SKIP_EMPTY_PARTS ( Qt::SkipEmptyParts )
#endif

#include "ebook_epub_helperxml.h"


namespace Ebook
{
namespace Epub
{

HelperXml::HelperXml( const QString& inputFile ) :
	filePath{ inputFile },
	baseDir{ "/" }
{
	int sep = inputFile.lastIndexOf( '/' );

	if ( sep != -1 )
		baseDir = inputFile.left( sep );
}

QString HelperXml::cleanPath( const QString& base, const QString& path )
{
	QStringList bList{};
	QStringList pList = path.split( '/', SKIP_EMPTY_PARTS );

	if ( !path.startsWith( '/' ) )
		bList = base.split( '/', SKIP_EMPTY_PARTS );

	for ( const QString& chunk : pList )
	{
		if ( chunk == "." )
			continue;
		else if ( chunk == ".." )
		{
			if ( !bList.empty() )
				bList.removeLast();
		}
		else
			bList.append( chunk );
	}

	return bList.join( '/' ).prepend( '/' );
}

QString HelperXml::cleanPath( const QString& path ) const
{
	return cleanPath( baseDir, path );
}

} // namespace Epub
} // namespace Ebook
