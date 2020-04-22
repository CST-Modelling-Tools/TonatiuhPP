TARGET = Tonatiuh
DESTDIR = ..

VERSION = 2.2.4
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

include(../config.pri)
QT += xml opengl svg script
QT += concurrent printsupport

INCLUDEPATH += $$TONATIUH_ROOT
LIBS += -lTonatiuhKernel

HEADERS += \
    $$files(application/*.h) \
    $$files(gui/*.h) \
    $$files(actions/*.h) \
    $$files(commands/*.h) \
    $$files(raytracing/*.h)

SOURCES += \
    $$files(application/*.cpp) \
    $$files(gui/*.cpp) \
    $$files(actions/*.cpp) \
    $$files(commands/*.cpp) \
    $$files(raytracing/*.cpp)

FORMS += $$files(gui/*.ui)

RESOURCES += resources.qrc
 
win32 {
    RC_FILE = Tonatiuh.rc
}

