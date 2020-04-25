TARGET = Tonatiuh
DESTDIR = ..

VERSION = 2.3.0
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

include(../config.pri)
QT += xml opengl svg script
QT += concurrent printsupport

LIBS += -lTonatiuhKernel

HEADERS += \
    $$files(actions/*.h) \
    $$files(calculator/*.h) \
    $$files(commands/*.h) \
    $$files(flux/*.h) \
    $$files(gui/*.h) \
    $$files(main/*.h) \
    $$files(parameters/*.h) \
    $$files(raytracing/*.h) \
    $$files(script/*.h) \
    $$files(tree/*.h) \
    $$files(view/*.h) \
    $$files(widgets/*.h)

SOURCES += \
    $$files(actions/*.cpp) \
    $$files(calculator/*.cpp) \
    $$files(commands/*.cpp) \
    $$files(flux/*.cpp) \
    $$files(gui/*.cpp) \
    $$files(main/*.cpp) \
    $$files(parameters/*.cpp) \
    $$files(raytracing/*.cpp) \
    $$files(script/*.cpp) \
    $$files(tree/*.cpp) \
    $$files(view/*.cpp) \
    $$files(widgets/*.cpp)

FORMS += \
    $$files(calculator/*.ui) \
    $$files(flux/*.ui) \
    $$files(gui/*.ui) \
    $$files(script/*.ui) \
    $$files(widgets/*.ui)

RESOURCES += resources.qrc
 
win32 { # icon in windows explorer
    RC_FILE = Tonatiuh.rc
}

