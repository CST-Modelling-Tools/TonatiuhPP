TEMPLATE = aux

# https://download.qt.io/official_releases/qt-installer-framework/
QIF = C:/Qt/QtIFW-3.2.2
CONFIG -= debug_and_release

# use scripts/deploy.bat to create install packages
INPUT = $$PWD/config.xml
INPUT += $$files($$PWD/packages/*.*, true)
OUTPUT = ../../install/qt/Tonatiuh-Installer

installer.input = INPUT
installer.output = $$OUTPUT
installer.commands = $$QIF/bin/binarycreator -c $$PWD/config.xml -p $$PWD/../../install/packages $$OUTPUT
installer.CONFIG += target_predeps no_link combine

QMAKE_EXTRA_COMPILERS += installer
