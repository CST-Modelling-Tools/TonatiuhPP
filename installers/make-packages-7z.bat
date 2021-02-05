SET QIF=C:\QtOnline\Tools\QtInstallerFramework\4.0\bin
SET ROOT=%CD%

@ECHO OFF

RMDIR packages-7z /s /q 

CD %ROOT%\packages\main\data
%QIF%\archivegen %ROOT%\packages-7z\main\data\main.7z *
XCOPY %ROOT%\packages\main\meta\* %ROOT%\packages-7z\main\meta\ 

CD %ROOT%\packages\examples\data
%QIF%\archivegen %ROOT%\packages-7z\examples\data\examples.7z *
XCOPY %ROOT%\packages\examples\meta\* %ROOT%\packages-7z\examples\meta\ 

@ECHO ON
@PAUSE