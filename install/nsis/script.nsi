!define COMPANYNAME "CyI"
!define APPNAME "Tonatiuh"
!define EDITIONNAME "Cyprus"
!define APPNAMEFULL "Tonatiuh (Cy)"
!define DESCRIPTION "Ray tracing for Solar Energy"
!define VERSIONMAJOR 0
!define VERSIONMINOR 1
!define VERSIONNAME "${VERSIONMAJOR}.${VERSIONMINOR}"
!define /date VERSIONDATE "%Y.%m.%d"
!define /date DATENAME "%d %B %Y"

#!define ISBUILDTEST 
# SetCompressor /SOLID lzma 

!include MUI2.nsh
!include nsDialogs.nsh

RequestExecutionLevel user
ManifestDPIAware true  
Unicode True
Name "${APPNAME}"
Caption "${APPNAME} - Installer"
BrandingText " "
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\orange-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\orange-uninstall.ico"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_RIGHT
!define MUI_HEADERIMAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Header\orange-r.bmp"
OutFile "${APPNAME}-Installer-Win64-v${VERSIONNAME}-${VERSIONDATE}.exe"
InstallDir "$LOCALAPPDATA\${COMPANYNAME}\${APPNAME}"
InstallDirRegKey HKCU "Software\${COMPANYNAME}\${APPNAME}" ""


!define MUI_WELCOMEPAGE_TITLE "Welcome to ${APPNAME} Installer"
!define MUI_WELCOMEPAGE_TEXT "\
Setup will guide you through the installation of ${APPNAME}.$\n\
$\n\
Click Next to continue.$\n\
$\n$\n$\n$\n$\n$\n$\n$\n\
Desription: ${DESCRIPTION}$\n$\n\
Edition: ${EDITIONNAME}$\n$\n\
Version: ${VERSIONNAME} (${DATENAME})\
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
#!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\${COMPANYNAME}\${APPNAME}" 
#!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
#!insertmacro MUI_PAGE_STARTMENU pageApplication $StartMenuFolder

Var RadioClassic
Var RadioClassic_State
Var RadioRising
Var RadioRising_State

Page custom nsDialogsPage nsDialogsPageLeave

Function .onInit
	strCpy $RadioClassic_State 1
	strCpy $RadioRising_State 0
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
FunctionEnd


!insertmacro MUI_PAGE_INSTFILES
!include "FileFunc.nsh" ; for ${GetSize} for EstimatedSize registry entry

#!define MUI_ABORTWARNING
#!define MUI_ABORTWARNING_TEXT "text"
#!define MUI_UNABORTWARNING
!define MUI_FINISHPAGE_NOAUTOCLOSE
!insertmacro MUI_PAGE_FINISH


UninstallCaption "${APPNAME} - Uninstaller"
!define MUI_WELCOMEPAGE_TITLE "Welcome to ${APPNAME} Uninstaller"
!define MUI_WELCOMEPAGE_TEXT "\
Setup will guide you through the uninstallation of ${APPNAME}.$\n\
$\n\
Before starting the uninstallation, make sure ${APPNAME} is not runnning.$\n\
$\n\
Click Next to continue.$\n\
$\n$\n$\n$\n$\n\
Desription: ${DESCRIPTION}$\n$\n\
Edition: ${EDITIONNAME}$\n$\n\
Version: ${VERSIONNAME} (${DATENAME})\
"
!insertmacro MUI_UNPAGE_WELCOME


!insertmacro MUI_UNPAGE_CONFIRM


!insertmacro MUI_UNPAGE_INSTFILES


!define MUI_UNFINISHPAGE_NOAUTOCLOSE
!insertmacro MUI_UNPAGE_FINISH

  
!insertmacro MUI_LANGUAGE "English" ; must be after pages
 

!define REGUNINSTALL "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" ; keep space
Section "Tonatiuh" SectionTonatiuh
	sectionIn RO ; read only, always installed
	setOutPath $INSTDIR
	!ifdef ISBUILDTEST
		file /r "bin*"
	!else
		file /r "..\packages\main\data\bin*"
	!endif
	file /r "..\packages\main\data\images*"
		
    writeRegStr HKCU "Software\${COMPANYNAME}\${APPNAME}" "" $INSTDIR
	writeRegStr HKCU "${REGUNINSTALL}" "DisplayName" "${APPNAMEFULL}"
	writeRegStr HKCU "${REGUNINSTALL}" "DisplayVersion" "${VERSIONNAME}"
	writeRegStr HKCU "${REGUNINSTALL}" "Publisher" "${COMPANYNAME}"
	writeRegStr HKCU "${REGUNINSTALL}" "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
	writeRegStr HKCU "${REGUNINSTALL}" "QuietUninstallString" "$\"$INSTDIR\uninstall.exe$\" /S"	
	
	writeRegDWORD HKCU "${REGUNINSTALL}" "NoModify" 1
	writeRegDWORD HKCU "${REGUNINSTALL}" "NoRepair" 1
	writeUninstaller "$INSTDIR\uninstall.exe"
    
	writeRegStr HKCU "Software\Classes\.tnh" "" "tnhfile"
	writeRegStr HKCU "Software\Classes\.tnhs" "" "tnhfile"
	writeRegStr HKCU "Software\Classes\.tnpp" "" "tnhfile"	
	exec '$WinDir\SysNative\ie4uinit.exe -show'
	
	#!insertmacro MUI_STARTMENU_WRITE_BEGIN pageApplication
		strCpy $StartMenuFolder "."
		createDirectory "$SMPROGRAMS\$StartMenuFolder"
		setOutPath "$INSTDIR\bin"
		var /GLOBAL AppPath
		strCpy $AppPath "$INSTDIR\bin\${APPNAME}-Application.exe"
		# add -Application because Windows indexing shows old icon
		writeRegStr HKCU "Software\Classes\tnhfile\shell\Open\Command" "" "$\"$AppPath$\" -i=$\"%1$\""	
		${If} $RadioClassic_State == 0
			createShortcut "$SMPROGRAMS\$StartMenuFolder\${APPNAMEFULL}.lnk" $AppPath
			writeRegStr HKCU "Software\${COMPANYNAME}\${APPNAME}" "theme" "Cyprus"
			writeRegStr HKCU "Software\Classes\tnhfile\DefaultIcon" "" $AppPath 
			writeRegStr HKCU "${REGUNINSTALL}" "DisplayIcon" $AppPath
		${Else}
			createShortcut "$SMPROGRAMS\$StartMenuFolder\${APPNAMEFULL}.lnk" $AppPath "" $AppPath 1
			writeRegStr HKCU "Software\${COMPANYNAME}\${APPNAME}" "theme" ""
			writeRegStr HKCU "Software\Classes\tnhfile\DefaultIcon" "" "$AppPath,1"
			writeRegStr HKCU "${REGUNINSTALL}" "DisplayIcon" "$AppPath,1"
		${EndIf}
		setOutPath $INSTDIR
		#WriteINIStr "$SMPROGRAMS\$StartMenuFolder\support.url" "InternetShortcut" "URL" "https://scmt.cyi.ac.cy/bitbucket/projects/tnh/repos/main"
	#!insertmacro MUI_STARTMENU_WRITE_END
	
	${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2
	IntFmt $0 "0x%08X" $0
	WriteRegDWORD HKCU "${REGUNINSTALL}" "EstimatedSize" "$0"
	DetailPrint ""
SectionEnd
 
 
Section "Examples" SectionExamples
	setOutPath $INSTDIR
	!ifdef ISBUILDTEST
		file /r "examples*"
	!else
		file /r "..\packages\main\data\examples*"
	!endif

	#!insertmacro MUI_STARTMENU_WRITE_BEGIN pageApplication
	#	createShortcut "$SMPROGRAMS\$StartMenuFolder\examples.lnk" "$INSTDIR\examples"
	#!insertmacro MUI_STARTMENU_WRITE_END
	
	${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2
	IntFmt $0 "0x%08X" $0
	WriteRegDWORD HKCU "${REGUNINSTALL}" "EstimatedSize" "$0"
	DetailPrint ""
SectionEnd
 
 
Section "Uninstall"
	rmDir /r "$INSTDIR\bin"
	rmDir /r "$INSTDIR\examples"
	rmDir /r "$INSTDIR\images"	
	delete "$INSTDIR\uninstall.exe"
	rmDir "$INSTDIR"
	rmDir "$INSTDIR\.."
	
    #!insertmacro MUI_STARTMENU_GETFOLDER pageApplication $StartMenuFolder
	delete "$SMPROGRAMS\$StartMenuFolder\${APPNAMEFULL}.lnk"
    #delete "$SMPROGRAMS\$StartMenuFolder\examples.lnk"
    #delete "$SMPROGRAMS\$StartMenuFolder\support.url"
	rmDir "$SMPROGRAMS\$StartMenuFolder"
	
  	deleteRegKey HKCU "Software\Classes\.tnh" 
	deleteRegKey HKCU "Software\Classes\.tnhs"
	deleteRegKey HKCU "Software\Classes\.tnpp"
	deleteRegKey HKCU "Software\Classes\tnhfile"	
	exec '$WinDir\SysNative\ie4uinit.exe -show'
	
    deleteRegKey HKCU "Software\${COMPANYNAME}\${APPNAME}"
    deleteRegKey /ifempty HKCU "Software\${COMPANYNAME}"
	deleteRegKey HKCU "${REGUNINSTALL}"
SectionEnd


LangString DESC_SectionTonatiuh ${LANG_ENGLISH} "Description of section 1."
LangString DESC_SectionExamples ${LANG_ENGLISH} "Description of section 2."
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${SectionTonatiuh} $(DESC_SectionTonatiuh)
	!insertmacro MUI_DESCRIPTION_TEXT ${SectionExamples} $(DESC_SectionExamples)
!insertmacro MUI_FUNCTION_DESCRIPTION_END
 
 