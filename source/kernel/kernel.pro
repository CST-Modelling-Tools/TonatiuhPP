TEMPLATE = lib
TARGET = TonatiuhKernel
DESTDIR = ..

include(../config.pri)
QT += xml opengl svg script
QT += concurrent printsupport


HEADERS += \
    $$files(auxiliary/*.h) \
    $$files(geometry/*.h) \
    $$files(gui/*.h) \
    $$files(photons/*.h) \
    $$files(raytracing/*.h) \
    $$files(statistics/*.h)

SOURCES += \
    $$files(auxiliary/*.cpp) \
    $$files(geometry/*.cpp) \
    $$files(gui/*.cpp) \
    $$files(photons/*.cpp) \
    $$files(raytracing/*.cpp) \
    $$files(statistics/*.cpp)
