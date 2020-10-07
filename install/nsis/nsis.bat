@ECHO %time%
@SET NSIS="C:\Program Files (x86)\NSIS\makensis.exe" 
%NSIS% /V3 script.nsi
@REM %NSIS% /V3 /X"SetCompressor /FINAL lzma" script.nsi
@ECHO %time%
@PAUSE