!makensis "params.nsi"
!system "params.exe"
!include "params.txt"

!delfile "params.exe"
!delfile "params.txt"

!include "main.nsi"