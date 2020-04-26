TEMPLATE = lib
TARGET = TonatiuhLibraries
DESTDIR = ..
DEFINES += TONATIUH_LIBRARIES_EXPORT

include(../config.pri)

HEADERS += \
    $$files(*.h) \
    $$files(geometry/*.h) \
    $$files(fields/*.h)

SOURCES += \
    $$files(geometry/*.cpp) \
    $$files(fields/*.cpp)
