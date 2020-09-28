!define APP_NAME "Tonatiuh"
!define EDITION_NAME "Cyprus"
!define APP_NAME_FULL "${APP_NAME} (${EDITION_NAME})"
!define DESCRIPTION "Ray tracing for Solar Energy (Open Source)"
!define VERSION_MAJOR 0
!define VERSION_MINOR 1
!define VERSION_NAME "${VERSION_MAJOR}.${VERSION_MINOR}"
!define /date VERSION_DATE "%Y.%m.%d"
!define /date DATE_NAME "%d %B %Y"

#!define ISBUILDTEST 
# SetCompressor /SOLID lzma 

!include MUI2.nsh
!include nsDialogs.nsh ; for custom pages
!include "FileFunc.nsh" ; for estimated file size ${GetSize}


RequestExecutionLevel user
ManifestDPIAware true  
Unicode True
Name "${APP_NAME}"
Caption "${APP_NAME} - Installer"
#BrandingText "Ray tracing for Solar Energy (Open Source)"
BrandingText " "
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\orange-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\orange-uninstall.ico"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_RIGHT
!define MUI_HEADERIMAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Header\orange-r.bmp"
OutFile "${APP_NAME}-Installer-Win64-${EDITION_NAME}-v${VERSION_NAME}-${VERSION_DATE}.exe"
InstallDir "$LOCALAPPDATA\${APP_NAME}\${EDITION_NAME}"
InstallDirRegKey HKCU "Software\${APP_NAME}\${EDITION_NAME}" ""


!define MUI_WELCOMEPAGE_TITLE "Welcome to ${APP_NAME} Installer"
!define MUI_WELCOMEPAGE_TEXT "\
Setup will guide you through the installation of ${APP_NAME}.$\n\
$\n\
Click Next to continue.$\n\
$\n$\n$\n$\n$\n$\n$\n$\n\
Desription: ${DESCRIPTION}$\n$\n\
Edition: ${EDITION_NAME}$\n$\n\
Version: ${VERSION_NAME} (${DATE_NAME})\
"
!define MUI_WELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange-uninstall.bmp"
!insertmacro MUI_PAGE_WELCOME


!insertmacro MUI_PAGE_LICENSE "license.txt"


#!define MUI_TEXT_COMPONENTS_TITLE "Components"
#!define MUI_TEXT_COMPONENTS_SUBTITLE " "
#!define MUI_COMPONENTSPAGE_SMALLDESC
!define MUI_COMPONENTSPAGE_NODESC
#!define MUI_COMPONENTSPAGE_TEXT_TOP "top"
#!define MUI_COMPONENTSPAGE_TEXT_COMPLIST "complist"
#!define MUI_COMPONENTSPAGE_TEXT_INSTTYPE "insttype"
!insertmacro MUI_PAGE_COMPONENTS


#!define MUI_TEXT_DIRECTORY_TITLE "Location"
#!define MUI_TEXT_DIRECTORY_SUBTITLE " "
#!define MUI_DIRECTORYPAGE_TEXT_TOP "text"
#!define MUI_DIRECTORYPAGE_TEXT_DESTINATION "destination"
!insertmacro MUI_PAGE_DIRECTORY


Var StartMenuFolder
#!define MUI_STARTMENUPAGE_DEFAULTFOLDER "."
#!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
#!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\${APP_NAME}\${EDITION_NAME}" 
#!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
#!insertmacro MUI_PAGE_STARTMENU pageApplication $StartMenuFolder

Var RadioClassic
Var RadioClassic_State
Var RadioRising
Var RadioRising_State
Var AppNameFull

Page custom nsDialogsPage nsDialogsPageLeave

Function .onInit
	StrCpy $RadioClassic_State 1
	StrCpy $RadioRising_State 0
FunctionEnd

Function nsDialogsPage
	!insertmacro MUI_HEADER_TEXT "Preferences" "Select a preferred theme."
	nsDialogs::Create 1018
	Pop $0

	${If} $0 == error
		Abort
	${EndIf}
	
	${NSD_CreateRadioButton} 15u 12u 35u 20u "&Classic"
	Pop $RadioClassic
	

	${NSD_CreateIcon} 57u 11u 32u 32u "images\Tonatiuh.ico"
	Pop $0

	${NSD_CreateRadioButton} 15u 45u 35u 20u "&Rising"
	Pop $RadioRising
	
	${NSD_CreateIcon} 57u 43u 32u 32u "images\TonatiuhCy.ico"
	Pop $0

	${NSD_SetState} $RadioClassic $RadioClassic_State	
	${NSD_SetState} $RadioRising $RadioRising_State
	
	nsDialogs::Show
FunctionEnd

Function nsDialogsPageLeave
	${NSD_GetState} $RadioClassic $RadioClassic_State	
	${NSD_GetState} $RadioRising $RadioRising_State
	
	${If} $RadioClassic_State == 1
		StrCpy $AppNameFull "${APP_NAME}++"
	${Else}
		StrCpy $AppNameFull "${APP_NAME} (${EDITION_NAME})"
	${EndIf}
FunctionEnd


!insertmacro MUI_PAGE_INSTFILES


#!define MUI_ABORTWARNING
#!define MUI_ABORTWARNING_TEXT "text"
#!define MUI_UNABORTWARNING
!define MUI_FINISHPAGE_RUN "$INSTDIR\bin\${APP_NAME}-Application.exe"
!define MUI_FINISHPAGE_RUN_TEXT "Run ${APP_NAME} now"
!define MUI_FINISHPAGE_NOAUTOCLOSE
!insertmacro MUI_PAGE_FINISH


UninstallCaption "${APP_NAME} - Uninstaller"
!define MUI_WELCOMEPAGE_TITLE "Welcome to ${APP_NAME} Uninstaller"
!define MUI_WELCOMEPAGE_TEXT "\
Setup will guide you through the uninstallation of ${APP_NAME}.$\n\
$\n\
Before starting the uninstallation, make sure ${APP_NAME} is not runnning.$\n\
$\n\
Click Next to continue.$\n\
$\n$\n$\n$\n$\n\
Desription: ${DESCRIPTION}$\n$\n\
Edition: ${EDITION_NAME}$\n$\n\
Version: ${VERSION_NAME} (${DATE_NAME})\
"
!insertmacro MUI_UNPAGE_WELCOME


!insertmacro MUI_UNPAGE_CONFIRM


!insertmacro MUI_UNPAGE_INSTFILES


!define MUI_UNFINISHPAGE_NOAUTOCLOSE
!insertmacro MUI_UNPAGE_FINISH

  
!insertmacro MUI_LANGUAGE "English" ; must be after pages
 
!define REG_KEY_UNINSTALL "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME_FULL}" ; keep space		
Var AppPath

InstType "Full"
InstType "Minimal"


Section "Tonatiuh" SectionTonatiuh
	SectionIn RO ; read only, always installed
	#SectionIn 1 2
	SetOutPath $INSTDIR
	!ifdef ISBUILDTEST
		File /r "bin*"
	!else
		File /r "..\packages\main\data\bin*"
	!endif
	File /r "..\packages\main\data\images*"

    WriteRegStr HKCU "Software\${APP_NAME}\${EDITION_NAME}" "" $INSTDIR
	WriteRegStr HKCU "${REG_KEY_UNINSTALL}" "DisplayName" $AppNameFull
	WriteRegStr HKCU "${REG_KEY_UNINSTALL}" "DisplayVersion" "${VERSION_NAME}"
	WriteRegStr HKCU "${REG_KEY_UNINSTALL}" "Publisher" "${EDITION_NAME}"
	WriteRegStr HKCU "${REG_KEY_UNINSTALL}" "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
	WriteRegStr HKCU "${REG_KEY_UNINSTALL}" "QuietUninstallString" "$\"$INSTDIR\uninstall.exe$\" /S"	
	
	WriteRegDWORD HKCU "${REG_KEY_UNINSTALL}" "NoModify" 1
	WriteRegDWORD HKCU "${REG_KEY_UNINSTALL}" "NoRepair" 1
	WriteUninstaller "$INSTDIR\uninstall.exe"
    
	WriteRegStr HKCU "Software\Classes\.tnh" "" "tnhfile"
	WriteRegStr HKCU "Software\Classes\.tnpp" "" "tnhfile"	
	WriteRegStr HKCU "Software\Classes\.tnhs" "" "tnhsfile"
	
	#!insertmacro MUI_STARTMENU_WRITE_BEGIN pageApplication
		StrCpy $StartMenuFolder "."
		CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
		SetOutPath "$INSTDIR\bin"
		StrCpy $AppPath "$INSTDIR\bin\${APP_NAME}-Application.exe"
		# add -Application because Windows indexing shows old icon
		WriteRegStr HKCU "Software\Classes\tnhfile\shell\Open\Command" "" "$\"$AppPath$\" -i=$\"%1$\""	
		WriteRegStr HKCU "Software\Classes\tnhsfile\shell\Open\Command" "" "$\"$AppPath$\" -i=$\"%1$\" -w"	
		${If} $RadioClassic_State == 1
			CreateShortCut "$SMPROGRAMS\$StartMenuFolder\$AppNameFull.lnk" $AppPath "" $AppPath 1
			WriteRegStr HKCU "Software\${APP_NAME}\${EDITION_NAME}" "theme" ""
			WriteRegStr HKCU "Software\Classes\tnhfile\DefaultIcon" "" "$AppPath,1"
			WriteRegStr HKCU "Software\Classes\tnhsfile\DefaultIcon" "" "$AppPath,1" 
			WriteRegStr HKCU "${REG_KEY_UNINSTALL}" "DisplayIcon" "$AppPath,1"
		${Else}
			CreateShortCut "$SMPROGRAMS\$StartMenuFolder\$AppNameFull.lnk" $AppPath
			WriteRegStr HKCU "Software\${APP_NAME}\${EDITION_NAME}" "theme" "Cyprus"
			WriteRegStr HKCU "Software\Classes\tnhfile\DefaultIcon" "" $AppPath 
			WriteRegStr HKCU "Software\Classes\tnhsfile\DefaultIcon" "" "$AppPath,2" 
			WriteRegStr HKCU "${REG_KEY_UNINSTALL}" "DisplayIcon" $AppPath
		${EndIf}
		SetOutPath $INSTDIR
		#WriteINIStr "$SMPROGRAMS\$StartMenuFolder\support.url" "InternetShortcut" "URL" "https://scmt.cyi.ac.cy/bitbucket/projects/tnh/repos/main"
	#!insertmacro MUI_STARTMENU_WRITE_END
	
	${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2
	IntFmt $0 "0x%08X" $0
	WriteRegDWORD HKCU "${REG_KEY_UNINSTALL}" "EstimatedSize" "$0"
	#DetailPrint ""
	Exec '$WinDir\SysNative\ie4uinit.exe -show' ; refresh icon cash on Windows 10
SectionEnd
 
 
Section "Examples" SectionExamples
	SectionIn 1
	SetOutPath $INSTDIR
	!ifdef ISBUILDTEST
		File /r "examples*"
	!else
		File /r "..\packages\main\data\examples*"
	!endif

	#!insertmacro MUI_STARTMENU_WRITE_BEGIN pageApplication
	#	createShortcut "$SMPROGRAMS\$StartMenuFolder\examples.lnk" "$INSTDIR\examples"
	#!insertmacro MUI_STARTMENU_WRITE_END
	
	${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2
	IntFmt $0 "0x%08X" $0
	WriteRegDWORD HKCU "${REG_KEY_UNINSTALL}" "EstimatedSize" "$0"
	#DetailPrint ""
SectionEnd
 
 
Section "Uninstall"
	ReadRegStr $AppNameFull HKCU "${REG_KEY_UNINSTALL}" "DisplayName"
		  
	RMDir /r "$INSTDIR\bin"
	RMDir /r "$INSTDIR\examples"
	RMDir /r "$INSTDIR\images"	
	Delete "$INSTDIR\uninstall.exe"
	RMDir "$INSTDIR"
	RMDir "$INSTDIR\.."
	
    #!insertmacro MUI_STARTMENU_GETFOLDER pageApplication $StartMenuFolder
	Delete "$SMPROGRAMS\$StartMenuFolder\$AppNameFull.lnk"
    #Delete "$SMPROGRAMS\$StartMenuFolder\examples.lnk"
    #Delete "$SMPROGRAMS\$StartMenuFolder\support.url"
	RMDir "$SMPROGRAMS\$StartMenuFolder"
	
  	DeleteRegKey HKCU "Software\Classes\.tnh" 
	DeleteRegKey HKCU "Software\Classes\.tnhs"
	DeleteRegKey HKCU "Software\Classes\.tnpp"
	DeleteRegKey HKCU "Software\Classes\tnhfile"	
	DeleteRegKey HKCU "Software\Classes\tnhsfile"	
	Exec '$WinDir\SysNative\ie4uinit.exe -show'
	
    DeleteRegKey HKCU "Software\${APP_NAME}\${EDITION_NAME}"
    DeleteRegKey /ifempty HKCU "Software\${APP_NAME}"
	DeleteRegKey HKCU "${REG_KEY_UNINSTALL}"
SectionEnd


LangString DESC_SectionTonatiuh ${LANG_ENGLISH} "Description of section 1."
LangString DESC_SectionExamples ${LANG_ENGLISH} "Description of section 2."
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${SectionTonatiuh} $(DESC_SectionTonatiuh)
	!insertmacro MUI_DESCRIPTION_TEXT ${SectionExamples} $(DESC_SectionExamples)
!insertmacro MUI_FUNCTION_DESCRIPTION_END
 
 