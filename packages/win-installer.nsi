; NSIS script file


;==============================;
; Configuration                ;
;==============================;

!ifndef APP_ARCH
    !define APP_ARCH x86_64
!endif

!ifndef INSTALLER_FILE
    !define INSTALLER_FILE uChmViewer_${APP_ARCH}_Setup.exe
!endif

; Directories

!define APP_BIN_DIR bin
!define PKG_BIN_DIR ${APP_BIN_DIR}
!define APP_I18N_DIR locale
!define PKG_I18N_DIR ${APP_I18N_DIR}
!define APP_ICONS_DIR icons
!define PKG_ICONS_DIR ${APP_ICONS_DIR}


;==============================;
; General                      ;
;==============================;

    Unicode true
    Name "uChmViewer"
    OutFile "${INSTALLER_FILE}"

!if ${APP_ARCH} == "x86_64"
    InstallDir "$PROGRAMFILES64\uChmViewer"
!else
    InstallDir "$PROGRAMFILES32\uChmViewer"
!endif
    
    InstallDirRegKey HKEY_LOCAL_MACHINE "SOFTWARE\uChmViewer" ""
;   SetCompressor lzma


; Modern UI

    !include "MUI.nsh"
    !define MUI_ABORTWARNING

; The icon for the installer and uninstaller.
   !define MUI_ICON icons/uchmviewer.ico
;  !define MUI_UNICON icons/uchmviewer.ico


;==============================;
; Pages                        ;
;==============================;
 
    Var STARTMENU_FOLDER

    !insertmacro MUI_PAGE_WELCOME
    !insertmacro MUI_PAGE_LICENSE LICENSE.txt
    !insertmacro MUI_PAGE_DIRECTORY

    ;Start Menu Folder Page Configuration
    !define MUI_STARTMENUPAGE_REGISTRY_ROOT HKEY_LOCAL_MACHINE
    !define MUI_STARTMENUPAGE_REGISTRY_KEY "SOFTWARE\uChmViewer"
    !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
    !insertmacro MUI_PAGE_STARTMENU Application $STARTMENU_FOLDER

    !insertmacro MUI_PAGE_INSTFILES
    !insertmacro MUI_PAGE_FINISH

    ; Uninstaller pages
    !insertmacro MUI_UNPAGE_WELCOME
    !insertmacro MUI_UNPAGE_CONFIRM
    !insertmacro MUI_UNPAGE_INSTFILES
    !insertmacro MUI_UNPAGE_FINISH


;==============================;
; Languages                    ;
;==============================;

    !insertmacro MUI_LANGUAGE "English"
    !insertmacro MUI_LANGUAGE "Catalan"
    !insertmacro MUI_LANGUAGE "Czech"
    !insertmacro MUI_LANGUAGE "Dutch"
    !insertmacro MUI_LANGUAGE "Finnish"
    !insertmacro MUI_LANGUAGE "German"
    !insertmacro MUI_LANGUAGE "Polish"
    !insertmacro MUI_LANGUAGE "Russian"
    !insertmacro MUI_LANGUAGE "Spanish"


;==============================;
; Main section                 ;
;==============================;

Section "-Install"
    SetOutPath "$INSTDIR"
    File LICENSE.txt

    SetOutPath "$INSTDIR\${PKG_BIN_DIR}"
    File /oname=uChmViewer.exe ${APP_BIN_DIR}\uchmviewer.exe
    SetOutPath "$INSTDIR\${PKG_ICONS_DIR}"
    File /r "${APP_ICONS_DIR}\*.ico"

    ;Store installation folder
    WriteRegStr HKEY_LOCAL_MACHINE "SOFTWARE\uChmViewer" "" "$INSTDIR"

    ;Create uninstaller
    WriteUninstaller "$INSTDIR\uninst.exe"

    !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\uChmViewer.lnk" "$INSTDIR\${PKG_BIN_DIR}\uChmViewer.exe"
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall.lnk" "$INSTDIR\uninst.exe"
    !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section "Uninstall"
    Push $R0
    !insertmacro MUI_STARTMENU_GETFOLDER Application $R0
    Delete "$SMPROGRAMS\$R0\Uninstall.lnk"
    Delete "$SMPROGRAMS\$R0\uChmViewer.lnk"
    RMDir  "$SMPROGRAMS\$R0"
    Pop $R0

    DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\uChmViewer"
    DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\uChmViewer"

    Delete "$INSTDIR\${PKG_ICONS_DIR}\*.ico"
    Delete "$INSTDIR\${PKG_BIN_DIR}\uChmViewer.exe"
    Delete "$INSTDIR\LICENSE.txt"
    Delete "$INSTDIR\uninst.exe"

    RMDir "$INSTDIR\${PKG_ICONS_DIR}"
    RMDir "$INSTDIR\${PKG_BIN_DIR}"
    RMDir "$INSTDIR"
SectionEnd


;==============================;
; Internationalisation         ;
;==============================;

Section "-Install i18n"
    SetOutPath "$INSTDIR\${PKG_I18N_DIR}"
    File /r "${APP_I18N_DIR}\*.mo"
SectionEnd

Section "un.Install i18n"
    RMDir /r "$INSTDIR\${PKG_I18N_DIR}"
    RMDir "$INSTDIR"
SectionEnd


;==============================;
; dependencies                    ;
;==============================;

Section "-Install Dependencies"
    SetOutPath "$INSTDIR\${PKG_BIN_DIR}"
    File ${APP_BIN_DIR}\qt.conf
    File /nonfatal /r "${APP_BIN_DIR}\*.dll"
    File /nonfatal /r "${APP_BIN_DIR}\*.qm"
SectionEnd

Section "un.Install Dependencies"
    RMDir /r "$INSTDIR\${PKG_BIN_DIR}"
    RMDir "$INSTDIR"
SectionEnd
