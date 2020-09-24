@echo %time%
"C:\Program Files (x86)\NSIS\makensis.exe" /V3 script.nsi
@rem "C:\Program Files (x86)\NSIS\makensis.exe" /V3 /X"SetCompressor /FINAL lzma" script.nsi
@echo %time%
@pause