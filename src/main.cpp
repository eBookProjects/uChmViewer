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
#include <QStringList>
#include <QtGlobal>

#if defined USE_KDE
	#include <k4aboutdata.h>
	#include <kapplication.h>
	#include <kcmdlineargs.h>
	#include "version.h"
#elif defined USE_MAC_APP
	#include "uchmviewerapp.h"
#else
	#include <QApplication>
	typedef QApplication  UchmviewerApp;
#endif

#if defined USE_DBUS
	#include <QDBusConnection>
	#include "dbus_interface.h"
#endif

#include "config.h"
#include "i18n.h"
#include "mainwindow.h"

MainWindow* mainWindow;


int main( int argc, char** argv )
{
#if QT_VERSION >= 0x050600
	/* Attribute Qt::AA_EnableHighDpiScaling must be set before QCoreApplication is created. */
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

#if defined (USE_KDE)
	K4AboutData aboutdata ( "uChmViewer",
	                        QByteArray(),
	                        ki18n( "uChmViewer" ),
	                        APP_VERSION,
	                        ki18n( "CHM file viewer" ),
	                        K4AboutData::License_GPL,
	                        ki18n( "(c) 2004-2015 George Yunaev" ),
	                        ki18n( "Please report bugs to nicegorov@ya.com" ),
	                        "https://github.com/u-235/uchmviewer",
	                        "" );

	KCmdLineArgs::init( &aboutdata );
	KApplication app;
#else
	UchmviewerApp app( argc, argv );

	app.addLibraryPath ( "qt-plugins" );
#endif

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

#if defined (USE_KDE)
	// Because KDE insists of using its KCmdLineArgs class for argument processing, and does not let you just
	// to use QCoreApplication::arguments(), it forces us to write two different process functions. To avoid this,
	// we convert command-line options to arguments ourselves here.
	QStringList arguments;

	for ( int i = 0; i < argc; i++ )
		arguments << QString::fromUtf8( argv[i] );

	mainWindow = new MainWindow( arguments );
#else
	mainWindow = new MainWindow( QCoreApplication::arguments() );
#endif

	// If we already have the duplicate instance, the data has been already sent to it - quit now
	if ( mainWindow->hasSameTokenInstance() )
	{
		return 0;
	}

	mainWindow->show();
	mainWindow->launch();

	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	return app.exec();
}
