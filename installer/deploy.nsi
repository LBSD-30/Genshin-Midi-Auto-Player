RequestExecutionLevel admin

!define APP_NAME "Genshin Midi Auto Player" 
!define APP_VERSION "1.0.3"
!define APP_PUBLISHER "LBSD" 
!define APP_EXE "Genshin_Midi_Auto_Player.exe" 
!define SETUP_ICON "appicon.ico"
!define APP_ICON "appicon.ico"

Outfile "${APP_NAME} ${APP_VERSION} Setup.exe"
SetCompressor lzma
InstallDir "$PROGRAMFILES64\${APP_NAME}"

!ifdef SETUP_ICON
Icon "${SETUP_ICON}"
UninstallIcon "${SETUP_ICON}" 
!endif

!include "MUI2.nsh"

!define MUI_ABORTWARNING
!define MUI_ICON "${SETUP_ICON}"
!define MUI_UNICON "${SETUP_ICON}"
!define MUI_FINISHPAGE_RUN "$INSTDIR\${APP_EXE}"
!define MUI_FINISHPAGE_RUN_TEXT "Run ${APP_NAME} now"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
      
!insertmacro MUI_LANGUAGE "English"

Section "Main" SecMain
  SectionIn RO

  SetOutPath "$INSTDIR"
  
  File /r "*.*"
  Delete "$INSTDIR\*.nsi"

  File "appicon.ico"

  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "DisplayName" "${APP_NAME} ${APP_VERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "DisplayVersion" "${APP_VERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "Publisher" "${APP_PUBLISHER}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "UninstallString" '"$INSTDIR\Uninstall.exe"' 
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "NoModify" 1  
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "NoRepair" 1 
  !ifdef APP_ICON
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "DisplayIcon" '"$INSTDIR\${APP_ICON}"'
  !else
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "DisplayIcon" '"$INSTDIR\${APP_EXE}"'
  !endif
 
  WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

Section "StartMenuShortcut" SecStartMenuShortcut

  CreateDirectory "$SMPROGRAMS\${APP_NAME}"

  !ifdef APP_ICON
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\${APP_NAME}.lnk" "$INSTDIR\${APP_EXE}" "" "$INSTDIR\${APP_ICON}" 0
  !else
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\${APP_NAME}.lnk" "$INSTDIR\${APP_EXE}" "" "$INSTDIR\${APP_EXE}" 0
  !endif
  
  CreateShortCut "$SMPROGRAMS\${APP_NAME}\Uninstall ${APP_NAME}.lnk" "$INSTDIR\Uninstall.exe"
SectionEnd

Section /o "DesktopShortcut" SecDesktopShortcut
  !ifdef APP_ICON
    CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${APP_EXE}" "" "$INSTDIR\${APP_ICON}" 0
  !else
    CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${APP_EXE}" "" "$INSTDIR\${APP_EXE}" 0
  !endif
SectionEnd

Section "Uninstall"

  RMDir /r /REBOOTOK "$INSTDIR" 

  RMDir /r /REBOOTOK "$SMPROGRAMS\${APP_NAME}"

  Delete /REBOOTOK "$DESKTOP\${APP_NAME}.lnk"

  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"
SectionEnd