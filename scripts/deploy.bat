@ECHO OFF

SET PROJECT=%CD%\..
SET BUILD=%PROJECT%\build-Tonatiuh-Desktop_Qt_5_14_2_MinGW_64_bit-Release
SET PACKAGE=%PROJECT%\packages\main
SET COIN=%PROJECT%\..\..\Libraries\Coin\release\bin
SET QT=C:\Qt\Qt5.14.2\5.14.2\mingw73_64



SET DATA=%PACKAGE%\data
SET META=%PACKAGE%\meta
MD %DATA%
COPY %PROJECT%\source\installer\packages\main\data\* %DATA%
MD %META%
COPY %PROJECT%\source\installer\packages\main\meta\* %META%



MD %DATA%\bin
COPY %BUILD%\*.exe %DATA%\bin
COPY %BUILD%\*.dll %DATA%\bin
FOR %%i IN (air, material, photons, random, shape, sun, trackers) DO (
	MD %DATA%\bin\plugins\%%i 
	COPY %BUILD%\plugins\%%i\*.dll %DATA%\bin\plugins\%%i
)

COPY %COIN%\*.dll %DATA%\bin

SET A=Qt5Concurrent, Qt5Core, Qt5Gui, Qt5OpenGL, Qt5PrintSupport, Qt5Script, Qt5Widgets
SET B=libgcc_s_seh-1, libstdc++-6, libwinpthread-1
FOR %%i IN (%A%, %B%) DO (
	COPY "%QT%\bin\%%i.dll" %DATA%\bin
)
MD %DATA%\bin\platforms
COPY %QT%\plugins\platforms\qwindows.dll %DATA%\bin\platforms\qwindows.dll
MD %DATA%\bin\styles
COPY %QT%\plugins\styles\qwindowsvistastyle.dll %DATA%\bin\styles\qwindowsvistastyle.dll



MD %DATA%\examples
COPY %PROJECT%\examples\* %DATA%\examples



@ECHO ON
@PAUSE
