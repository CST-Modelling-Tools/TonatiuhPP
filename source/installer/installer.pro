TEMPLATE = aux

# https://download.qt.io/official_releases/qt-installer-framework/
QIF = "C:/QtOnline/Tools/QtInstallerFramework/4.0"
CONFIG -= debug_and_release

# use installers/make-packages.bat to create install packages
# use installers/make-packages-7z.bat to create install 7z-packages
INPUT = $$PWD/config.xml
INPUT += $$files($$PWD/packages/*.*, true)
OUTPUT = ../../installers/Qt/Tonatiuh-Installer

installer.input = INPUT
installer.output = $$OUTPUT
installer.commands = $$QIF/bin/binarycreator --offline-only -c $$PWD/config.xml -p $$PWD/../../installers/packages-7z $$OUTPUT
installer.CONFIG += target_predeps no_link combine

QMAKE_EXTRA_COMPILERS += installer
