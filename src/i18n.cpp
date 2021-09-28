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
#include <QTranslator>

#if !defined USE_KDE && defined USE_GETTEXT
#include <libintl.h>

class Translator: public QTranslator
{
	public:
		Translator() {}

		QString translate( const char* context, const char* sourceText, const char* disambiguation = nullptr, int n = -1 ) const
		{
			Q_UNUSED( context )
			Q_UNUSED( disambiguation )
			Q_UNUSED( n )

			const char* res = gettext( sourceText );

			if ( res != sourceText )
				return QString( res );


			return QString();
		}
};
#endif
#include "i18n.h"

void app_i18n::init()
{
	static QTranslator qt_i18n;
	bool loaded = qt_i18n.load( QLocale(), QLatin1String( "qtbase" ),
	                            "_",  QLibraryInfo::location( QLibraryInfo::TranslationsPath ) );

	if ( loaded )
		QCoreApplication::installTranslator( &qt_i18n );

#if !defined USE_KDE && defined USE_GETTEXT
	static Translator app_i18n;

	//bindtextdomain ( "kchmviewer", "/usr/share/locale" );
	textdomain ( "kchmviewer" );
	QCoreApplication::installTranslator( &app_i18n );
#endif
}
