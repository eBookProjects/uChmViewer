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

#include <QCoreApplication>
#include <QObject>
#include <QList>
#include <QString>
#include <QtGlobal>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	#include <QStringList>
	#include <Qt>
	#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
		#include <QGuiApplication>
	#endif
#else // QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	using QStringList = QList<QString>;
#endif

#if defined USE_MAC_APP
	#include "uchmviewerapp.h"
#else
	#include <QApplication>
	typedef QApplication  UchmviewerApp;
#endif

#ifdef _WIN32
	#include <windows.h>
	#include <QApplication>
	#include <QFont>
#endif

#if defined USE_DBUS
	#include <QDBusConnection>
	#include "dbus_interface.h"
#endif

#include "config.h"
#include "i18n.h"
#include "mainwindow.h"

MainWindow* mainWindow;


void fallbackFonts()
{
#ifdef _WIN32
	// Obtain default GUI font (typically "Segoe UI, 9pt", see QTBUG-58610)
	QFont systemFont = QApplication::font( "QMessageBox" );
	LANGID lid = GetUserDefaultLangID();
	QStringList result;

	switch ( lid & 0xff )
	{
	case LANG_CHINESE:
		if ( lid == 0x0804 || lid == 0x1004 )  // China mainland and Singapore
			result << "Microsoft YaHei UI" << "Microsoft YaHei" << "Arial"
			       << "Microsoft JhengHei UI" << "Microsoft JhengHei" << "Malgun Gothic" << "Yu Gothic UI" << "Meiryo UI"
			       << "Yu Gothic" << "Meiryo";
		else
			result << "Microsoft JhengHei UI" << "Microsoft JhengHei" << "Arial"
			       << "Microsoft YaHei UI" << "Microsoft YaHei" << "Malgun Gothic" << "Yu Gothic UI" << "Meiryo UI"
			       << "Yu Gothic" << "Meiryo";

		break;

	case LANG_JAPANESE:
		result << "Yu Gothic UI" << "Meiryo UI" << "Yu Gothic" << "Meiryo" << "Arial"
		       << "Malgun Gothic" << "Microsoft YaHei UI" << "Microsoft YaHei" << "Microsoft JhengHei UI" << "Microsoft JhengHei";
		break;

	case LANG_KOREAN:
		result << "Malgun Gothic" << "Arial"
		       << "Microsoft JhengHei UI" << "Microsoft JhengHei" << "Microsoft YaHei UI" << "Microsoft YaHei"
		       << "Yu Gothic UI" << "Meiryo UI" << "Yu Gothic" << "Meiryo";
		break;

	default:
		result << "Arial" << "Yu Gothic UI" << "Meiryo UI" << "Yu Gothic" << "Meiryo" << "Malgun Gothic"
		       << "Microsoft JhengHei UI" << "Microsoft JhengHei" << "Microsoft YaHei UI" << "Microsoft YaHei";
		break;
	}

	result << "Nirmala UI"
	       << "Iskoola Pota"
	       << "Ebrima"
	       << "Arial Unicode MS"
	       << "Segoe UI Emoji"
	       << "Segoe UI Symbol";
	systemFont.insertSubstitutions( systemFont.family(), result );
	systemFont.setHintingPreference( QFont::PreferNoHinting );
	QApplication::setFont( systemFont );
#endif
}

int main( int argc, char** argv )
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	/* Attribute Qt::AA_EnableHighDpiScaling must be set before QCoreApplication is created. */
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
	QCoreApplication::setAttribute( Qt::AA_EnableHighDpiScaling );
#endif

	QCoreApplication::setAttribute( Qt::AA_UseHighDpiPixmaps );

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
	QGuiApplication::setHighDpiScaleFactorRoundingPolicy( Qt::HighDpiScaleFactorRoundingPolicy::PassThrough );
#endif
#endif

	UchmviewerApp app( argc, argv );

	app.addLibraryPath( "qt-plugins" );

	fallbackFonts();

	app_i18n::init();

	// Set data for QSettings
	QCoreApplication::setOrganizationName( "uChmViewer" );
	QCoreApplication::setOrganizationDomain( "uChmViewer.net" );
	QCoreApplication::setApplicationName( "uchmviewer" );

	// Configuration
	pConfig = new Config();

#if defined USE_DBUS

	if ( QDBusConnection::sessionBus().isConnected() )
	{
		if ( QDBusConnection::sessionBus().registerService( SERVICE_NAME ) )
		{
			DBusInterface* dbusiface = new DBusInterface();
			QDBusConnection::sessionBus().registerObject( "/", dbusiface, QDBusConnection::ExportAllSlots );
		}
		else
			qWarning( "Cannot register service %s on session bus. Going without D-BUS support.", SERVICE_NAME );
	}
	else
		qWarning( "Cannot connect to the D-BUS session bus. Going without D-BUS support." );

#endif

	mainWindow = new MainWindow( QCoreApplication::arguments() );

	// If we already have the duplicate instance, the data has been already sent to it - quit now
	if ( mainWindow->hasSameTokenInstance() )
	{
		return 0;
	}

	mainWindow->launch();

	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	return app.exec();
}
