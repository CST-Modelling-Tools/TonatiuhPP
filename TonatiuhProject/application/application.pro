TARGET = Tonatiuh
DESTDIR = ..

include(../config.pri)
QT += xml opengl svg script
QT += concurrent printsupport

INCLUDEPATH += $$TONATIUH_ROOT
LIBS += -lTonatiuhKernel

HEADERS += \
    $$files(application/*.h) \
    $$files(gui/*.h) \
    $$files(raytracing/*.h)

SOURCES += \
    $$files(application/*.cpp) \
    $$files(gui/*.cpp) \
    $$files(raytracing/*.cpp)

FORMS += $$files(gui/*.ui)

RESOURCES += tonatiuh.qrc
 
win32 {
    RC_FILE = Tonatiuh.rc
}

