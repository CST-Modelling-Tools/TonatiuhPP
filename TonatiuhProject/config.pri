VERSION = 2.2.4

# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

COINDIR = C:/Users/Victor/Neo/Programming/Qt/Libraries/Coin/release
TONATIUH_ROOT = C:/Users/Victor/Neo/Programming/Qt/Projects/tonatiuh/TonatiuhProject


QT += widgets


INCLUDEPATH += 	. \
                $$TONATIUH_ROOT/geometry \
                $$TONATIUH_ROOT/fields \
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

LIBS += -L$$OUT_PWD/../bin -lgeometry -lfields
LIBS += -L$$COINDIR/lib -lCoin -lSoQt
		 
QMAKE_CFLAGS_RELEASE -= -O2
QMAKE_CFLAGS_RELEASE += -O3 -march=native
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -march=native
		


contains( CONFIG, plugin ){  	
#    QMAKE_CLAGS+= -fPIC
#    QMAKE_CXXLAGS+= -fPIC
    INCLUDEPATH += $$(TONATIUH_ROOT)/plugin
    LIBS += -L../../bin
    DESTDIR = ../../bin/plugins
    CONFIG += skip_target_version_ext # do not append the dll names with version number
}
CONFIG -= debug_and_release # separate folders for debug and release


