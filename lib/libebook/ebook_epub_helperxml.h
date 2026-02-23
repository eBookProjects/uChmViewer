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

#ifndef EBOOK_EPUB_HELPERXML_H
#define EBOOK_EPUB_HELPERXML_H

#include <QString>
#include <QXmlDefaultHandler>


namespace Ebook
{
namespace Epub
{

class HelperXml : public QXmlDefaultHandler
{
	public:
		HelperXml( const QString& inputFile );

		/// The full name of the handled file, including the path from the root
		/// directory.
		QString filePath;
		/// The full path and name of the directory containing the handled file.
		QString baseDir;

		/**
		 * @brief Returns the absolute path of the @b path, calculated relative
		 * to the @b base directory.
		 *
		 * The @b base parameter must contain a stripped absolute path and
		 * can be empty.
		 *
		 * If the @b path begins with "/", the @b base parameter is ignored and
		 * the result is calculated relative to the root directory. In other
		 * words, in this case, the @b path is an absolute path.
		 *
		 * The sequences "./" and "../" in the @b path parameter are processed
		 * as standard.
		 *
		 * The result always begins with "/".
		 */
		static QString cleanPath( const QString& base,  const QString& path );

	protected:
		/**
		 * @brief Returns the result of cleanPath(const QString&, const QString&),
		 * passed #baseDir as the base directory.
		 */
		QString cleanPath( const QString& path ) const;
};

} // namespace Epub
} // namespace Ebook

#endif // EBOOK_EPUB_HELPERXML_H
