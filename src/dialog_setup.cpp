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

#include <QCheckBox>
#include <QMessageBox>
#include <QObject>      // QObject::connect
#include <QString>
#include <QUrl>
#include <QWidget>

#include "kde-qt.h" // KFileDialog or QFileDialog

#include "i18n.h"

#include "config.h"       // Config, pConfig
#include "dialog_setup.h" // DialogSetup, QDialog
#include "mainwindow.h"   // :mainWindow


DialogSetup::DialogSetup(QWidget* parent)
	: QDialog(parent), Ui::DialogSetup()
{
	setupUi( this );

	connect( btnBrowse, SIGNAL( clicked() ), this, SLOT( browseExternalEditor() ) );

	// Set up the parameters
	switch ( pConfig->m_startupMode )
	{
	case Config::STARTUP_DO_NOTHING:
		rbStartWithNothing->setChecked( true );
		break;

	case Config::STARTUP_LOAD_LAST_FILE:
		m_radioOnBeginOpenLast->setChecked( true );
		break;

	case Config::STARTUP_POPUP_OPENFILE:
		m_radioOnBeginOpenDialog->setChecked( true );
		break;
	}

	m_historySize->setValue ( pConfig->m_numOfRecentFiles );
	m_rememberHistoryInfo->setChecked ( pConfig->m_HistoryStoreExtra );

	m_radioExtLinkOpenAlways->setChecked ( pConfig->m_onExternalLinkClick == Config::ACTION_ALWAYS_OPEN );
	m_radioExtLinkAsk->setChecked ( pConfig->m_onExternalLinkClick == Config::ACTION_ASK_USER );
	m_radioExtLinkOpenNever->setChecked ( pConfig->m_onExternalLinkClick == Config::ACTION_DONT_OPEN );
	m_highlightSearchResults->setChecked( pConfig->browser.highlightSearchResults );

	m_radioNewChmOpenAlways->setChecked ( pConfig->m_onNewChmClick == Config::ACTION_ALWAYS_OPEN );
	m_radioNewChmAsk->setChecked ( pConfig->m_onNewChmClick == Config::ACTION_ASK_USER );
	m_radioNewChmOpenNever->setChecked ( pConfig->m_onNewChmClick == Config::ACTION_DONT_OPEN );

	m_advExternalProgramName->setText( pConfig->m_advExternalEditorPath );
	m_advViewSourceExternal->setChecked ( !pConfig->m_advUseInternalEditor );
	m_advViewSourceInternal->setChecked ( pConfig->m_advUseInternalEditor );

	m_numOfRecentFiles = pConfig->m_numOfRecentFiles;

	boxLayoutDirectionRL->setChecked( pConfig->m_advLayoutDirectionRL );

	// Browser settings
	m_enableImages->setChecked( pConfig->browser.enableImages );
	m_enableJS->setChecked( pConfig->browser.enableJS );
	m_enableJava->setChecked( pConfig->browser.enableJava );
	m_enablePlugins->setChecked( pConfig->browser.enablePlugins );
	m_enableOfflineStorage->setChecked( pConfig->browser.enableOfflineStorage );
	m_enableLocalStorage->setChecked( pConfig->browser.enableLocalStorage );
	m_openAllTOCEntries->setChecked( pConfig->m_tocOpenAllEntries );
	boxUseSingleClick->setChecked( pConfig->m_tabUseSingleClick );

#if defined (USE_WEBENGINE)
	m_enableOfflineStorage->setChecked( false );
	m_enableOfflineStorage->setEnabled( false );
	m_enableJava->setChecked( false );
	m_enableJava->setEnabled( false );
#endif

	switch ( pConfig->m_toolbarMode )
	{
	case Config::TOOLBAR_SMALLICONS:
		rbToolbarSmall->setChecked( true );
		break;

	case Config::TOOLBAR_LARGEICONS:
		rbToolbarLarge->setChecked( true );
		break;

	case Config::TOOLBAR_LARGEICONSTEXT:
		rbToolbarLargeText->setChecked( true );
		break;

	case Config::TOOLBAR_TEXTONLY:
		rbToolbarText->setChecked( true );
		break;
	}
}

DialogSetup::~DialogSetup()
{
}

static void inline Check_Need_Restart( QCheckBox* box, bool* confsetting, bool* need_restart )
{
	if ( *confsetting != box->isChecked() )
	{
		*need_restart = true;
		*confsetting = box->isChecked();
	}
}

void DialogSetup::accept()
{
	if ( rbStartWithNothing->isChecked() )
		pConfig->m_startupMode = Config::STARTUP_DO_NOTHING;
	else if ( m_radioOnBeginOpenLast->isChecked() )
		pConfig->m_startupMode = Config::STARTUP_LOAD_LAST_FILE;
	else
		pConfig->m_startupMode = Config::STARTUP_POPUP_OPENFILE;

	pConfig->m_numOfRecentFiles = m_historySize->value();
	pConfig->m_HistoryStoreExtra = m_rememberHistoryInfo->isChecked();
	pConfig->browser.highlightSearchResults = m_highlightSearchResults->isChecked();
	pConfig->m_tocOpenAllEntries = m_openAllTOCEntries->isChecked();

	if ( m_radioExtLinkOpenAlways->isChecked () )
		pConfig->m_onExternalLinkClick = Config::ACTION_ALWAYS_OPEN;
	else if ( m_radioExtLinkAsk->isChecked () )
		pConfig->m_onExternalLinkClick = Config::ACTION_ASK_USER;
	else
		pConfig->m_onExternalLinkClick = Config::ACTION_DONT_OPEN;

	if ( m_radioNewChmOpenAlways->isChecked () )
		pConfig->m_onNewChmClick = Config::ACTION_ALWAYS_OPEN;
	else if ( m_radioNewChmAsk->isChecked () )
		pConfig->m_onNewChmClick = Config::ACTION_ASK_USER;
	else
		pConfig->m_onNewChmClick = Config::ACTION_DONT_OPEN;

	// Check the changes
	bool need_restart = false;

	Check_Need_Restart( m_enableImages, &pConfig->browser.enableImages, &need_restart );
	Check_Need_Restart( m_enableJS, &pConfig->browser.enableJS, &need_restart );
	Check_Need_Restart( m_enableJava, &pConfig->browser.enableJava, &need_restart );
	Check_Need_Restart( m_enablePlugins, &pConfig->browser.enablePlugins, &need_restart );
	Check_Need_Restart( m_enableOfflineStorage, &pConfig->browser.enableOfflineStorage, &need_restart );
	Check_Need_Restart( m_enableLocalStorage, &pConfig->browser.enableLocalStorage, &need_restart );
	Check_Need_Restart( boxUseSingleClick, &pConfig->m_tabUseSingleClick, &need_restart );

	Config::ToolbarMode newmode;

	if ( rbToolbarSmall->isChecked() )
		newmode = Config::TOOLBAR_SMALLICONS;
	else if ( rbToolbarLarge->isChecked() )
		newmode = Config::TOOLBAR_LARGEICONS;
	else if ( rbToolbarLargeText->isChecked() )
		newmode = Config::TOOLBAR_LARGEICONSTEXT;
	else
		newmode = Config::TOOLBAR_TEXTONLY;

	if ( newmode != pConfig->m_toolbarMode )
	{
		pConfig->m_toolbarMode = newmode;
		::mainWindow->updateToolbars();
	}

	pConfig->m_advExternalEditorPath = m_advExternalProgramName->text();
	pConfig->m_advUseInternalEditor = m_advViewSourceExternal->isChecked();
	pConfig->m_advUseInternalEditor = m_advViewSourceInternal->isChecked();

	if ( pConfig->m_numOfRecentFiles != m_numOfRecentFiles )
		need_restart = true;

	// Layout direction management
	bool layout_rl = boxLayoutDirectionRL->isChecked();

	if ( layout_rl != pConfig->m_advLayoutDirectionRL )
	{
		pConfig->m_advLayoutDirectionRL = layout_rl;
		need_restart = true;
	}

	pConfig->save();

	if ( need_restart )
		QMessageBox::information( this,
		                          QCoreApplication::applicationName(),
		                          i18n( "Changing those options requires restarting the application to take effect." )  );

	QDialog::accept();
}

void DialogSetup::browseExternalEditor()
{
#if defined (USE_KDE)
	QString exec = KFileDialog::getOpenFileName( QUrl(), i18n("*|Executables"), this, i18n("Choose an editor executable"));
#else
	QString exec = QFileDialog::getOpenFileName(this,
	                                            i18n("Choose an editor executable"),
	                                            QString(),
	                                            i18n( "Executables (*)") );
#endif

	if ( !exec.isEmpty() )
		m_advExternalProgramName->setText( exec );
}
