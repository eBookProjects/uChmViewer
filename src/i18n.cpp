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

#include "i18n.h"

#if !defined USE_KDE && defined USE_GETTEXT
#include <libintl.h>

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

        // Special for qt4. See https://doc.qt.io/archives/qt-4.8/qtranslator.html#translate
		virtual QString translate( const char* context, const char* sourceText, const char* disambiguation ) const
		{
			return translate( context, sourceText, disambiguation, -1 );
		}
};
#endif // !defined USE_KDE && defined USE_GETTEXT

void app_i18n::init()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	QString qt_file_name = QLatin1String( "qtbase" );
#else
	QString qt_file_name = QLatin1String( "qt" );
#endif
	static QTranslator qt_i18n;
	bool loaded = qt_i18n.load( QLocale(), qt_file_name, "_",
	                            QLibraryInfo::location( QLibraryInfo::TranslationsPath ) );

	if ( loaded )
		QCoreApplication::installTranslator( &qt_i18n );

#if defined USE_WEBENGINE
	static QTranslator we_i18n;
	loaded = we_i18n.load( QLocale(), QLatin1String( "qtwebengine" ),
	                       "_",  QLibraryInfo::location( QLibraryInfo::TranslationsPath ) );

	if ( loaded )
		QCoreApplication::installTranslator( &we_i18n );

#endif // defined USE_WEBENGINE

#if !defined USE_KDE && defined USE_GETTEXT
	static Translator app_i18n;
	//bindtextdomain ( "kchmviewer", "/usr/share/locale" );
	textdomain ( "kchmviewer" );
	QCoreApplication::installTranslator( &app_i18n );
#endif
}
