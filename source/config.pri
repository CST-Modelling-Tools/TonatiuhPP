# Tonatiuh
VERSION = 0.1.8.0
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
INCLUDEPATH += $$PWD
LIBS += -L..

# Coin3D
gcc {
    CONFIG(debug, debug|release) {
#        COINDIR = $$PWD/../libraries/Coin3D-qt5-mingw-debug
        COINDIR = $$PWD/../libraries/Coin3D-qt6-mingw-debug
        LIBS += -lCoind -lSoQtd
    } else {
        COINDIR = $$PWD/../libraries/Coin3D-qt5-mingw-release
#        COINDIR = $$PWD/../libraries/Coin3D-qt6-mingw-release
        LIBS += -lCoin -lSoQt
    }
    INCLUDEPATH += $$COINDIR/include
    DEFINES += COIN_NOT_DLL SOQT_NOT_DLL
    LIBS += -L$$COINDIR/lib # for linking
    LIBS += -L$$COINDIR/bin # for running

    QMAKE_CXXFLAGS_RELEASE -= -O2
    QMAKE_CXXFLAGS_RELEASE += -O3 -march=skylake # native?
}

msvc {
    COINDIR = $$PWD/../libraries/Coin3D
    LIBS += -lCoin4 -lSoQt1

    INCLUDEPATH += $$COINDIR/include
    DEFINES += COIN_DLL SOQT_DLL
    LIBS += -L$$COINDIR/lib # for linking
    LIBS += -L$$COINDIR/bin # for running

    QMAKE_CXXFLAGS_RELEASE += /arch:AVX
}

CONFIG -= debug_and_release # separate folders for debug and release
CONFIG += skip_target_version_ext # do not append the dll names with version number

QT += widgets
