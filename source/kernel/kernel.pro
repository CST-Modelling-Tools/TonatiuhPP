TEMPLATE = lib
TARGET = TonatiuhKernel
DESTDIR = ..
DEFINES += TONATIUH_KERNEL_EXPORT

include(../config.pri)

LIBS += -lTonatiuhLibraries

HEADERS += \
    $$files(*.h) \
    $$files(air/*.h) \
    $$files(component/*.h) \
    $$files(scene/*.h) \
    $$files(material/*.h) \
    $$files(photons/*.h) \
    $$files(random/*.h) \
    $$files(run/*.h) \
    $$files(shape/*.h) \
    $$files(sun/*.h) \
    $$files(trackers/*.h)

SOURCES += \
    $$files(*.cpp) \
    $$files(air/*.cpp) \
#    $$files(component/*.cpp) \
    $$files(scene/*.cpp) \
    $$files(material/*.cpp) \
    $$files(photons/*.cpp) \
    $$files(random/*.cpp) \
    $$files(run/*.cpp) \
    $$files(shape/*.cpp) \
    $$files(sun/*.cpp) \
    $$files(trackers/*.cpp)

RESOURCES += resources.qrc

DISTFILES += \
    images/AirPolynomial.png \
    images/AirVacuum.png
