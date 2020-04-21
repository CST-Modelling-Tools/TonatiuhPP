TEMPLATE = lib
TARGET = TonatiuhLibraries
DESTDIR = ..

CONFIG -= debug_and_release
QT += widgets

QMAKE_CFLAGS_RELEASE -= -O2
QMAKE_CFLAGS_RELEASE += -O3 -march=native
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -march=native

COINDIR = C:/Users/Victor/Neo/Programming/Qt/Libraries/Coin/release
INCLUDEPATH += $$COINDIR/include
LIBS += -L$$COINDIR/lib -lCoin -lSoQt
DEFINES += COIN_NOT_DLL SOQT_NOT_DLL

HEADERS += \
    $$files(geometry/*.h) \
    $$files(fields/*.h)

SOURCES += \
    $$files(geometry/*.cpp) \
    $$files(fields/*.cpp)
