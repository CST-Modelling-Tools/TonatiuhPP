@REM cmd reference https://docs.microsoft.com/en-us/windows-server/administration/windows-commands/windows-commands

@ECHO OFF

@REM remove directory "portable" (with subdirectories, quiet mode)
RMDIR portable /s /q

SET PROJECT=%CD%\..
SET BUILD=%PROJECT%\build-Tonatiuh-Desktop_Qt_6_2_4_MinGW_64_bit-Release
SET PORTABLE=%PROJECT%\installers\portable
SET COIN=%PROJECT%\libraries\Coin3D-qt6.2-mingw-release\bin
SET QT=C:\Qt\6.2.4\mingw_64

MD %PORTABLE%\bin
COPY %BUILD%\*.exe %PORTABLE%\bin
COPY %BUILD%\*.dll %PORTABLE%\bin
FOR %%i IN (air, material, photons, random, shape, sun, trackers) DO (
	MD %PORTABLE%\bin\plugins\%%i 
	COPY %BUILD%\plugins\%%i\*.dll %PORTABLE%\bin\plugins\%%i
)

COPY %COIN%\*.dll %PORTABLE%\bin

SET A=Qt6Concurrent, Qt6Core, Qt6Gui, Qt6OpenGL, Qt6PrintSupport, Qt6Qml, Qt6Network, Qt6Widgets
SET B=libgcc_s_seh-1, libstdc++-6, libwinpthread-1
FOR %%i IN (%A%, %B%) DO (
	COPY "%QT%\bin\%%i.dll" %PORTABLE%\bin
)

SET A=libssl-1_1-x64, libcrypto-1_1-x64
FOR %%i IN (%A%) DO (
	COPY "%QT%\..\..\Tools\OpenSSL\Win_x64\bin\%%i.dll" %PORTABLE%\bin
)

MD %PORTABLE%\bin\platforms
COPY %QT%\plugins\platforms\qwindows.dll %PORTABLE%\bin\platforms\qwindows.dll
MD %PORTABLE%\bin\styles
COPY %QT%\plugins\styles\qwindowsvistastyle.dll %PORTABLE%\bin\styles\qwindowsvistastyle.dll
MD %PORTABLE%\bin\imageformats
COPY %QT%\plugins\imageformats\qico.dll %PORTABLE%\bin\imageformats\qico.dll
COPY %QT%\plugins\imageformats\qjpeg.dll %PORTABLE%\bin\imageformats\qjpeg.dll
MD %PORTABLE%\bin\tls
COPY %QT%\plugins\tls\qopensslbackend.dll %PORTABLE%\bin\tls\qopensslbackend.dll

MD %PORTABLE%\resources
@REM copy directory (with empty subdirectories)
XCOPY %PROJECT%\resources %PORTABLE%\resources /e

MD %PORTABLE%\examples
@REM copy directory (exclude python file)
XCOPY %PROJECT%\examples %PORTABLE%\examples /e /exclude:exclude.txt /q

MD %PORTABLE%\help\html
XCOPY %PROJECT%\help\html %PORTABLE%\help\html /e /q
XCOPY %PROJECT%\help\license.txt %PORTABLE%\help\

@ECHO ON
@PAUSE
