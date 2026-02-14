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

#include <QByteArray>
#include <QChar>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSettings>
#include <QStandardPaths>
#include <QString>
#include <QVariant>
#include <QtGlobal>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	#include <QStringList>
#else
	#include <QList>
	using QStringList = QList<QString>;
#endif

#include "config.h"


Config* pConfig;


Config::Config()
{
	// Support for portable app - if the data path is specified in the configuration, use it.
	m_datapath = QCoreApplication::applicationDirPath() + QDir::separator() + "portable";
	bool portable = QFile( m_datapath ).exists();

	if ( portable )
	{
		QSettings::setDefaultFormat( QSettings::IniFormat );
		QSettings::setPath( QSettings::defaultFormat(), QSettings::UserScope, m_datapath );
		m_datapath += QDir::separator() + QString( "data" );
	}
	else
		m_datapath = QStandardPaths::writableLocation( QStandardPaths::GenericCacheLocation ) + "/" + "uchmviewer";

	QSettings settings;

	m_startupMode = ( Config::StartupMode ) settings.value( "settings/onstartup", STARTUP_DO_NOTHING ).toInt();
	m_onNewChmClick = ( Config::choose_action_t ) settings.value( "settings/onnewchm", ACTION_ASK_USER ).toInt();
	m_onExternalLinkClick = ( Config::choose_action_t ) settings.value( "settings/onexternal", ACTION_ASK_USER ).toInt();
	m_numOfRecentFiles = settings.value( "settings/maxrecentfiles", 10 ).toInt();
	m_HistoryStoreExtra = settings.value( "settings/extrahistory", true ).toBool();
	m_advUseInternalEditor = settings.value( "advanced/internaleditor", true ).toBool();
	m_advLayoutDirectionRL = settings.value( "advanced/layoutltr", false ).toBool();
	m_advExternalEditorPath = settings.value( "advanced/editorpath", "/usr/bin/kate" ).toString();
	m_toolbarMode = ( Config::ToolbarMode ) settings.value( "advanced/toolbarmode", TOOLBAR_LARGEICONSTEXT ).toInt();
	m_lastOpenedDir = settings.value( "advanced/lastopendir", "." ).toString();

	browser.enableJS = settings.value( "browser/enablejs", true ).toBool();
	browser.enableJava = settings.value( "browser/enablejava", false ).toBool();
	browser.enablePlugins = settings.value( "browser/enableplugins", true ).toBool();
	browser.enableImages  = settings.value( "browser/enableimages", true ).toBool();
	browser.enableOfflineStorage = settings.value( "browser/enableofflinestorage", false ).toBool();
	browser.enableLocalStorage = settings.value( "browser/enablelocalstorage", false ).toBool();
	m_browserEnableRemoteContent = settings.value( "browser/enableremotecontent", false ).toBool();
	browser.highlightSearchResults = settings.value( "browser/highlightsearchresults", true ).toBool();

	m_tocOpenAllEntries = settings.value( "browser/tocopenallentries", true ).toBool();
	m_tabUseSingleClick = settings.value( "browser/tabusesingleclick", true ).toBool();

	QDir dir;
	dir.setPath( m_datapath );

	if ( !dir.exists() && !dir.mkpath( m_datapath ) )
		qWarning( "Could not create directory %s", qPrintable( m_datapath ) );

	// If the old cache directory exists, copy its contents to the correct directory
	//  and remove the old directory.
	if ( !portable )
	{
		QDir oldDataDir = QDir( QDir::homePath() + "/" + ".uchmviewer" );
		QDir dataDir = QDir( m_datapath );

		if ( oldDataDir.exists() )
		{
			const QStringList& files = oldDataDir.entryList( {"*.idx", "*.uchmviewer"},
			                                                 QDir::Files | QDir::NoDot | QDir::NoDotDot );

			for ( const QString& file : files )
			{
				QFile::rename( oldDataDir.filePath( file ), dataDir.filePath( file ) );
			}

			oldDataDir.rmdir( oldDataDir.absolutePath() );
		}
	}
}

void Config::save( )
{
	QSettings settings;

	settings.setValue( "settings/onstartup", m_startupMode );
	settings.setValue( "settings/onnewchm", m_onNewChmClick );
	settings.setValue( "settings/onexternal", m_onExternalLinkClick );
	settings.setValue( "settings/maxrecentfiles", m_numOfRecentFiles );
	settings.setValue( "settings/extrahistory", m_HistoryStoreExtra );
	settings.setValue( "advanced/internaleditor", m_advUseInternalEditor );
	settings.setValue( "advanced/layoutltr", m_advLayoutDirectionRL );
	settings.remove( "advanced/autodetectenc" );
	settings.setValue( "advanced/editorpath", m_advExternalEditorPath );
	settings.setValue( "advanced/toolbarmode", m_toolbarMode );
	settings.setValue( "advanced/lastopendir", m_lastOpenedDir );

	settings.setValue( "browser/enablejs", browser.enableJS );
	settings.setValue( "browser/enablejava", browser.enableJava );
	settings.setValue( "browser/enableplugins", browser.enablePlugins );
	settings.setValue( "browser/enableimages", browser.enableImages );
	settings.setValue( "browser/enableofflinestorage", browser.enableOfflineStorage );
	settings.setValue( "browser/enablelocalstorage", browser.enableLocalStorage );
	settings.setValue( "browser/enableremotecontent", m_browserEnableRemoteContent );
	settings.setValue( "browser/highlightsearchresults", browser.highlightSearchResults );

	settings.setValue( "browser/tocopenallentries", m_tocOpenAllEntries );
	settings.setValue( "browser/tabusesingleclick", m_tabUseSingleClick );
}

QString Config::getEbookSettingFile( const QString& ebookfile ) const
{
	QFileInfo finfo( ebookfile );
	QString prefix = pConfig->m_datapath + QDir::separator() + finfo.completeBaseName();

	return prefix + ".uchmviewer";
}

QString Config::getEbookIndexFile( const QString& ebookfile ) const
{
	QFileInfo finfo( ebookfile );
	QString prefix = pConfig->m_datapath + "/" + finfo.completeBaseName();

	return prefix + ".idx";
}
