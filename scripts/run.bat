
@SET PROJECT=%CD%\..
@SET BUILD=%PROJECT%\build-Tonatiuh-Desktop_Qt_5_14_2_MinGW_64_bit-Release
@SET COIN=%PROJECT%\..\..\Libraries\Coin\release\bin
@SET QT=C:\Qt\Qt5.14.2\5.14.2\mingw73_64\bin
@SET PATH=%COIN%;%QT%;%PATH%

@CD %BUILD%
Tonatiuh > log.txt

@PAUSE