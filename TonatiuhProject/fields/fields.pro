TEMPLATE = lib

QT += widgets
CONFIG -= debug_and_release

QMAKE_CFLAGS_RELEASE -= -O2
QMAKE_CFLAGS_RELEASE += -O3 -march=native -mfpmath=sse
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -march=native -mfpmath=sse

DESTDIR = ../bin

COINDIR = C:/Users/Victor/Neo/Programming/Qt/Libraries/Coin/release
TONATIUH_ROOT = C:/Users/Victor/Neo/Programming/Qt/Projects/tonatiuh/TonatiuhProject

INCLUDEPATH += 	. \
                $$TONATIUH_ROOT/geometry \
                $$TONATIUH_ROOT/src \
                $$TONATIUH_ROOT/src/source \
                $$TONATIUH_ROOT/src/source/analyzer \
                $$TONATIUH_ROOT/src/source/application \
                $$TONATIUH_ROOT/src/source/auxiliary \
                $$TONATIUH_ROOT/src/source/geometry \
                $$TONATIUH_ROOT/src/source/gui \
                $$TONATIUH_ROOT/src/source/raytracing \
                $$TONATIUH_ROOT/src/source/statistics \
                $$COINDIR/include


DEFINES += COIN_NOT_DLL SOQT_NOT_DLL
LIBS += -L$$COINDIR/lib -lCoin -lSoQt

HEADERS += *.h
SOURCES += *.cpp


