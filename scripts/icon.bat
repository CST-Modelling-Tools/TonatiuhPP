reg add "HKEY_CLASSES_ROOT\.tnh"  /t REG_SZ /d tnhfile
reg add "HKEY_CLASSES_ROOT\.tnhs"  /t REG_SZ /d tnhfile
reg add "HKEY_CLASSES_ROOT\.tnpp"  /t REG_SZ /d tnhfile

reg add "HKEY_CLASSES_ROOT\tnhfile\DefaultIcon" /t REG_SZ /d C:\Users\Victor\Neo\Programming\Qt\Projects\Tonatiuh2020\documentation\Tonatiuh.png

ie4uinit.exe -show

@pause
