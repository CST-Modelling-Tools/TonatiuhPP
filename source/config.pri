# Tonatiuh
VERSION = 2.3.0
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
INCLUDEPATH += $$PWD
LIBS += -L..

# Coin3D
gcc {
COINDIR = C:/Users/Victor/Neo/Programming/Qt/Libraries/Coin/release
INCLUDEPATH += $$COINDIR/include
DEFINES += COIN_NOT_DLL SOQT_NOT_DLL
LIBS += -L$$COINDIR/lib # for linking
LIBS += -lCoin -lSoQt
LIBS += -L$$COINDIR/bin # for running

QMAKE_CFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CFLAGS_RELEASE += -O3 -march=native
QMAKE_CXXFLAGS_RELEASE += -O3 -march=native

#QMAKE_CFLAGS += -pg
#QMAKE_CXXFLAGS += -pg
#QMAKE_LFLAGS_WINDOWS += -pg
}

msvc {
COINDIR = C:/Users/Victor/Neo/Programming/Qt/Libraries/Coin/msvs
INCLUDEPATH += $$COINDIR/include
DEFINES += COIN_DLL SOQT_DLL
LIBS += -L$$COINDIR/lib # for linking
LIBS += -lCoin4 -lSoQt1
LIBS += -L$$COINDIR/bin # for running
}

CONFIG -= debug_and_release # separate folders for debug and release
CONFIG += skip_target_version_ext # do not append the dll names with version number

QT += widgets
