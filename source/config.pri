# Tonatiuh
VERSION = 2.3.0
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
INCLUDEPATH += $$PWD
LIBS += -L..

# Coin3D
gcc {
COINDIR = $$PWD/../libraries/Coin3D
INCLUDEPATH += $$COINDIR/include
DEFINES += COIN_NOT_DLL SOQT_NOT_DLL
LIBS += -L$$COINDIR/lib # for linking
LIBS += -lCoin -lSoQt
LIBS += -L$$COINDIR/bin # for running

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -march=skylake # native?
}

msvc {
COINDIR = $$PWD/../libraries/Coin3D
INCLUDEPATH += $$COINDIR/include
DEFINES += COIN_DLL SOQT_DLL
LIBS += -L$$COINDIR/lib # for linking
LIBS += -lCoin4 -lSoQt1
LIBS += -L$$COINDIR/bin # for running

QMAKE_CXXFLAGS_RELEASE += /arch:AVX
}

CONFIG -= debug_and_release # separate folders for debug and release
CONFIG += skip_target_version_ext # do not append the dll names with version number

QT += widgets

DEFINES += CYE
CONFIG += CYE
