SET ANACONDA="C:\ProgramData\Anaconda3"
SET KEY="HKEY_CURRENT_USER\SOFTWARE\Classes"

reg add "%KEY%\.ipynb" /t REG_SZ /d JupyterRun.bat
reg add "%KEY%\JupyterRun.bat\DefaultIcon" /t REG_SZ /d "%ANACONDA%\pythonw.exe"
reg add "%KEY%\JupyterRun.bat\shell\Open\Command" /t REG_SZ /d "\"%ANACONDA%\JupyterRun.bat\" \"%%1\""

ie4uinit.exe -show
@PAUSE