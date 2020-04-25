# Coin3D
COINDIR = C:/Users/Victor/Neo/Programming/Qt/Libraries/Coin/release
INCLUDEPATH += $$COINDIR/include
DEFINES += COIN_NOT_DLL SOQT_NOT_DLL
LIBS += -L$$COINDIR/lib -lCoin -lSoQt
LIBS += -L$$COINDIR/bin

CONFIG -= debug_and_release # separate folders for debug and release
CONFIG += skip_target_version_ext # do not append the dll names with version number
QT += widgets

QMAKE_CFLAGS_RELEASE -= -O2
QMAKE_CFLAGS_RELEASE += -O3 -march=native
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -march=native

INCLUDEPATH += $$PWD

LIBS += -L$$OUT_PWD/.. -lTonatiuhLibraries
