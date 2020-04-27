TEMPLATE = lib
TARGET = TonatiuhKernel
DESTDIR = ..
DEFINES += TONATIUH_KERNEL_EXPORT

include(../config.pri)

LIBS += -lTonatiuhLibraries

HEADERS += \
    $$files(*.h) \
    $$files(air/*.h) \
    $$files(auxiliary/*.h) \
    $$files(geometry/*.h) \
    $$files(gui/*.h) \
    $$files(material/*.h) \
    $$files(photons/*.h) \
    $$files(random/*.h) \
    $$files(raytracing/*.h) \
    $$files(shape/*.h) \

SOURCES += \
    $$files(air/*.cpp) \
    $$files(auxiliary/*.cpp) \
    $$files(geometry/*.cpp) \
    $$files(gui/*.cpp) \
    $$files(material/*.cpp) \
    $$files(photons/*.cpp) \
    $$files(random/*.cpp) \
    $$files(raytracing/*.cpp) \
    $$files(shape/*.cpp)
