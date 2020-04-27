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
    $$files(component/*.h) \
    $$files(gui/*.h) \
    $$files(material/*.h) \
    $$files(photons/*.h) \
    $$files(random/*.h) \
    $$files(raytracing/*.h) \
    $$files(shape/*.h) \
    $$files(sun/*.h) \
    $$files(tracker/*.h)

SOURCES += \
    $$files(*.cpp) \
    $$files(air/*.cpp) \
    $$files(auxiliary/*.cpp) \
#    $$files(component/*.cpp) \
    $$files(gui/*.cpp) \
    $$files(material/*.cpp) \
    $$files(photons/*.cpp) \
    $$files(random/*.cpp) \
    $$files(raytracing/*.cpp) \
    $$files(shape/*.cpp) \
    $$files(sun/*.cpp) \
    $$files(tracker/*.cpp)
