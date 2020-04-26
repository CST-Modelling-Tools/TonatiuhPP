@ECHO OFF

SET BUILD=..\build-Tonatiuh-Desktop_Qt_5_14_2_MinGW_64_bit-Profile
SET COIN=C:\Users\Victor\Neo\Programming\Qt\Libraries\Coin\release\bin
SET QT=C:\Qt\Qt5.14.2\5.14.2\mingw73_64

CD ..
MD bin
CD bin

ECHO root
COPY %BUILD%\*.exe 
COPY %BUILD%\*.dll 
FOR %%i IN (air, material, photon, random, shape, sun) DO (
	ECHO %%i
	MD plugins\%%i 
	COPY %BUILD%\plugins\%%i\*.dll plugins\%%i
)

ECHO coin
COPY %COIN%\*.dll


ECHO qt
SET A=Qt5Concurrent, Qt5Core, Qt5Gui, Qt5OpenGL, Qt5PrintSupport, Qt5Script, Qt5Widgets
SET B=libgcc_s_seh-1, libstdc++-6, libwinpthread-1
FOR %%i IN (%A%, %B%) DO (
	COPY "%QT%\bin\%%i.dll"
)
MD platforms
COPY %QT%\plugins\platforms\qwindows.dll platforms\qwindows.dll
MD styles
COPY %QT%\plugins\styles\qwindowsvistastyle.dll styles\qwindowsvistastyle.dll

@ECHO ON
@PAUSE
