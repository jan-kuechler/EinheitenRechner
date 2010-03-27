# Modern UI
!include MUI2.nsh

# General Settings
Name "~V_NAME~ Installer"

OutFile "Setup\~V_NAME~_Setup_~V_STRING_NICE~.exe"
InstallDir $PROGRAMFILES\~V_NAME~

# Interface Settings
!define MUI_BGCOLOR DDDDDD
!define MUI_HEADERIMAGE

# Installer Pages
!define MUI_PAGE_HEADER_TEXT "~V_NAME~"
!define MUI_PAGE_HEADRE_SUBTEXT "Version ~V_STRING~"

!define MUI_WELCOMEPAGE_TITLE "~V_NAME~ - ~V_STRING~"
!define MUI_WELCOMEPAGE_TEXT "Willkommen zur Installation von ~V_NAME~"

#!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES

#!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_DIRECTORY
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

Section
	SetOutPath $INSTDIR
	
	File Release\~V_FILE~
	File rules
	
	File Setup\Libs\*.dll
	
	WriteUninstaller $INSTDIR\uninstall.exe
SectionEnd

Section "Start Menu Shortcuts"
	CreateDirectory "$SMPROGRAMS\EinheitenRechner"
	createShortCut "$SMPROGRAMS\EinheitenRechner\EinheitenRechner.lnk" "$INSTDIR\EinheitenRechner.exe"
	createShortCut "$SMPROGRAMS\EinheitenRechner\Deinstallieren.lnk" "$INSTDIR\uninstall.exe"
SectionEnd

Section "Uninstall"
	Delete "$SMPROGRAMS\EinheitenRechner\*"
	RMDir "$SMPROGRAMS\EinheitenRechner"
	
	Delete $INSTDIR\*
	RMDir $INSTDIR\*
	RMDir /r  $INSTDIR
SectionEnd
