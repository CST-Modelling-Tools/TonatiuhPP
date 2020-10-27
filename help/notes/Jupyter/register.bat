SET ANACONDA="C:\ProgramData\Anaconda3"
SET KEY="HKEY_CURRENT_USER\SOFTWARE\Classes"

reg add "%KEY%\.ipynb" /t REG_SZ /d Jupyter.bat
reg add "%KEY%\Jupyter.bat\DefaultIcon" /t REG_SZ /d "%ANACONDA%\pythonw.exe"
reg add "%KEY%\Jupyter.bat\shell\Open\Command" /t REG_SZ /d "\"%ANACONDA%\Jupyter.bat\" \"%%1\""

ie4uinit.exe -show
@PAUSE