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

#include <QCoreApplication>
#include <QLibraryInfo>
#include <QLocale>
#include <QRegExp>
#include <QString>
#include <QTranslator>

#include "i18n.h"


#if defined USE_GETTEXT
#include <libintl.h>

namespace app_i18n
{
void initGettext();
}

#if !defined APP_INSTALL_DIR_BIN || !defined APP_INSTALL_DIR_LOCALE
void app_i18n::initGettext()
{
	textdomain ( "uchmviewer" );
}
#else
void app_i18n::initGettext()
{
	QString binDir = QLatin1String( APP_INSTALL_DIR_BIN );
	QString localeDir = QLatin1String( APP_INSTALL_DIR_LOCALE );
	QString appDir = QCoreApplication::applicationDirPath();
	// Clearing leading and trailing slashes, just in case.
	QRegExp leadingSlash = QRegExp( "^[/\\\\]*" );
	QRegExp trailingSlash = QRegExp( "[/\\\\]*$" );
	binDir.remove( leadingSlash );
	binDir.remove( trailingSlash );
	localeDir.remove( leadingSlash );
	localeDir.remove( trailingSlash );
	appDir.remove( trailingSlash );

	if (appDir.endsWith( binDir ))
	{
		// Getting the installation root folder and adding the translations path.
		appDir.remove( QRegExp(binDir + "$") );
		appDir.append( localeDir );
		bindtextdomain ( "uchmviewer", appDir.toUtf8().data() );
	}

	textdomain ( "uchmviewer" );
}
#endif //!defined APP_INSTALL_DIR_BIN || !defined APP_INSTALL_DIR_LOCALE

class Translator: public QTranslator
{
	public:
		Translator() {}

		virtual QString translate( const char* context, const char* sourceText, const char* disambiguation, int n ) const
		{
			Q_UNUSED( context )
			Q_UNUSED( disambiguation )
			Q_UNUSED( n )

			const char* res = gettext( sourceText );

			if ( res != sourceText )
				return QString::fromUtf8( res );


			return QString();
		}
};
#endif // defined USE_GETTEXT

void app_i18n::init()
{
	QString qt_file_name = QLatin1String( "qtbase" );
	static QTranslator qt_i18n;
	bool loaded = qt_i18n.load( QLocale(), qt_file_name, "_",
	                            QLibraryInfo::location( QLibraryInfo::TranslationsPath ) );

	if ( loaded )
		QCoreApplication::installTranslator( &qt_i18n );

	static QTranslator we_i18n;
	loaded = we_i18n.load( QLocale(), QLatin1String( "qtwebengine" ),
	                       "_",  QLibraryInfo::location( QLibraryInfo::TranslationsPath ) );

	if ( loaded )
		QCoreApplication::installTranslator( &we_i18n );

#if defined USE_GETTEXT
	static Translator app_i18n;
	initGettext();
	QCoreApplication::installTranslator( &app_i18n );
#endif
}
