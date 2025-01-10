/*
 *  Kchmviewer - a CHM and EPUB file viewer with broad language support
 *  Copyright (C) 2021 Nick Egorrov, nicegorov@yandex.ru
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

#include <QByteArray>
#include <QString>
#include <QUrl>

#include "mimehelper.h"


// Yes, I know about std::isspace(), but it may depend on the locale.
#define isspace(c) (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v')

QByteArray MimeHelper::mimeType( const QUrl& url, const QByteArray& buf )
{
	QString path = url.path().toLower();

	// Try to recognize the MIME type by the extension of the file name.
	if ( path.endsWith( ".cs" ) || path.endsWith( ".css" ) )
		return "text/css";
	else if ( path.endsWith( ".js" ) )
		return "text/js";

	/* BOM          Encoding Form
	 * 00 00 FE FF  UTF-32, big-endian
	 * FF FE 00 00  UTF-32, little-endian
	 * FE FF        UTF-16, big-endian
	 * FF FE        UTF-16, little-endian
	 * EF BB BF     UTF-8
	 */
	auto iter = buf.begin();

	// Skip utf-8 Byte Order Mark
	if ( buf.startsWith( "\xEF\xBB\xBF" ) )
	{
		iter += 3;
	}

	/* If the first non space character in buf is '<',
	 * then we assume that the buffer contains HTML,
	 * or if it is followed by '?', then XML.
	 *
	 * TODO Skip comments like "<!-- -->" */
	for ( ; iter != buf.end() ; iter++ )
	{
		char c = *iter;

		if ( c == '<' )
		{
			c = *( ++iter );

			if ( c == '?' )
				return "text/xml";
			else
				return "text/html";
		}

		if ( !isspace( c ) )
			break;
	}

	if ( path.endsWith( ".htm" ) || path.endsWith( ".html" ) )
		return "text/html";
	else if ( path.endsWith( ".xhtml" ) || path.endsWith( ".xml" ) )
		return "text/xml";

	return "application/octet-stream";
}
