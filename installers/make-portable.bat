@ECHO OFF

RMDIR /s /q portable

SET PROJECT=%CD%\..
SET BUILD=%PROJECT%\build-Tonatiuh-Desktop_Qt_5_14_2_MinGW_64_bit-Release
SET PORTABLE=%PROJECT%\install\portable
SET COIN=%PROJECT%\libraries\Coin3D\bin
SET QT=C:\Qt\Qt5.14.2\5.14.2\mingw73_64

MD %PORTABLE%\bin
COPY %BUILD%\*.exe %PORTABLE%\bin
COPY %BUILD%\*.dll %PORTABLE%\bin
FOR %%i IN (air, material, photons, random, shape, sun, trackers) DO (
	MD %PORTABLE%\bin\plugins\%%i 
	COPY %BUILD%\plugins\%%i\*.dll %PORTABLE%\bin\plugins\%%i
)

COPY %COIN%\*.dll %PORTABLE%\bin

SET A=Qt5Concurrent, Qt5Core, Qt5Gui, Qt5OpenGL, Qt5PrintSupport, Qt5Script, Qt5Widgets
SET B=libgcc_s_seh-1, libstdc++-6, libwinpthread-1
FOR %%i IN (%A%, %B%) DO (
	COPY "%QT%\bin\%%i.dll" %PORTABLE%\bin
)

MD %PORTABLE%\bin\platforms
COPY %QT%\plugins\platforms\qwindows.dll %PORTABLE%\bin\platforms\qwindows.dll
MD %PORTABLE%\bin\styles
COPY %QT%\plugins\styles\qwindowsvistastyle.dll %PORTABLE%\bin\styles\qwindowsvistastyle.dll
MD %PORTABLE%\bin\imageformats
COPY %QT%\plugins\imageformats\qico.dll %PORTABLE%\bin\imageformats\qico.dll
COPY %QT%\plugins\imageformats\qjpeg.dll %PORTABLE%\bin\imageformats\qjpeg.dll

MD %PORTABLE%\resources
XCOPY %PROJECT%\resources %PORTABLE%\resources /e

MD %PORTABLE%\examples
XCOPY %PROJECT%\examples %PORTABLE%\examples /e /exclude:exclude.txt

MD %PORTABLE%\help\html
XCOPY %PROJECT%\help\html %PORTABLE%\help\html /e
XCOPY %PROJECT%\help\license.txt %PORTABLE%\help

@ECHO ON
@PAUSE
