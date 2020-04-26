TEMPLATE = lib
TARGET = TonatiuhKernel
DESTDIR = ..
DEFINES += TONATIUH_KERNEL_EXPORT

include(../config.pri)

LIBS += -lTonatiuhLibraries

HEADERS += \
    $$files(*.h) \
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
