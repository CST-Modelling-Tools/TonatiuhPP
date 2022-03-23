@REM cmd reference https://docs.microsoft.com/en-us/windows-server/administration/windows-commands/windows-commands

@ECHO OFF

@REM remove directory "packages" (with subdirectories, quiet mode)
RMDIR packages /s /q 

SET PROJECT=%CD%\..
SET BUILD=%PROJECT%\build-Tonatiuh-Desktop_Qt_6_1_2_MinGW_64_bit-Release
SET PACKAGES_IN=%PROJECT%\source\installer\packages
SET PACKAGES_OUT=%PROJECT%\installers\packages
SET COIN=%PROJECT%\libraries\Coin3D-qt6-mingw-release\bin
SET QT=C:\Qt\6.1.2\mingw81_64

SET PACKAGE=main
SET PACKAGE_IN=%PACKAGES_IN%\%PACKAGE%
SET PACKAGE_OUT=%PACKAGES_OUT%\%PACKAGE%
SET DATA=%PACKAGE_OUT%\data
SET META=%PACKAGE_OUT%\meta
MD %DATA%
COPY %PACKAGE_IN%\data\* %DATA%
MD %META%
COPY %PACKAGE_IN%\meta\* %META%
REM without *?

MD %DATA%\bin
COPY %BUILD%\*.exe %DATA%\bin
COPY %BUILD%\*.dll %DATA%\bin
FOR %%i IN (air, material, photons, random, shape, sun, trackers) DO (
	MD %DATA%\bin\plugins\%%i 
	COPY %BUILD%\plugins\%%i\*.dll %DATA%\bin\plugins\%%i
)

COPY %COIN%\*.dll %DATA%\bin

SET A=Qt6Concurrent, Qt6Core, Qt6Gui, Qt6OpenGL, Qt6PrintSupport, Qt6Qml, Qt6Network, Qt6Widgets
SET B=libgcc_s_seh-1, libstdc++-6, libwinpthread-1
FOR %%i IN (%A%, %B%) DO (
	COPY "%QT%\bin\%%i.dll" %DATA%\bin
)
MD %DATA%\bin\platforms
COPY %QT%\plugins\platforms\qwindows.dll %DATA%\bin\platforms\qwindows.dll
MD %DATA%\bin\styles
COPY %QT%\plugins\styles\qwindowsvistastyle.dll %DATA%\bin\styles\qwindowsvistastyle.dll
MD %DATA%\bin\imageformats
COPY %QT%\plugins\imageformats\qico.dll %DATA%\bin\imageformats\qico.dll
COPY %QT%\plugins\imageformats\qjpeg.dll %DATA%\bin\imageformats\qjpeg.dll

MD %DATA%\resources
@REM copy directory (with empty subdirectories)
XCOPY %PROJECT%\resources %DATA%\resources /e

MD %DATA%\help\html
XCOPY %PROJECT%\help\html %DATA%\help\html /e /q
XCOPY %PROJECT%\help\license.txt %DATA%\help\

SET PACKAGE=examples
SET PACKAGE_IN=%PACKAGES_IN%\%PACKAGE%
SET PACKAGE_OUT=%PACKAGES_OUT%\%PACKAGE%
SET DATA=%PACKAGE_OUT%\data
SET META=%PACKAGE_OUT%\meta
MD %DATA%
COPY %PACKAGE_IN%\data\* %DATA%
MD %META%
COPY %PACKAGE_IN%\meta\* %META%

MD %DATA%\examples
@REM copy directory (exclude python files)
XCOPY %PROJECT%\examples %DATA%\examples /e /exclude:exclude.txt /q

@ECHO ON
@PAUSE
