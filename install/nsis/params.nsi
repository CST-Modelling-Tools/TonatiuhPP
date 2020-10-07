# Invoking NSIS run-time commands on compile-time
RequestExecutionLevel user
SilentInstall silent
Unicode True
OutFile "params.exe"


Function Trim
	Exch $R1 ; Original string
	Push $R2
Loop:
	StrCpy $R2 "$R1" 1
	StrCmp "$R2" "0" TrimLeft
	Goto Done
TrimLeft:	
	StrCpy $R1 "$R1" "" 1
	Goto Loop
Done:
	Pop $R2
	Exch $R1
FunctionEnd


!define Trim "!insertmacro Trim"
!macro Trim ResultVar String
  Push "${String}"
  Call Trim
  Pop "${ResultVar}"
!macroend


!define /date DATE_NAME "%d %B %Y" ; strftime()
Var DateName 
Var DateTrimmed 

Section
	StrCpy $DateName "${DATE_NAME}"
	StrCpy $DateTrimmed ""
	${Trim} $DateTrimmed $DateName 

	FileOpen $R0 "$EXEDIR\params.txt" w
	FileWrite $R0 '!define DATE_NAME "$DateTrimmed"'
	FileClose $R0
SectionEnd