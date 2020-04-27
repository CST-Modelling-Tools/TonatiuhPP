TEMPLATE = lib
TARGET = TonatiuhLibraries
DESTDIR = ..
DEFINES += TONATIUH_LIBRARIES_EXPORT

include(../config.pri)

HEADERS += \
    $$files(*.h) \
    $$files(auxiliary/*.h) \
    $$files(geometry/*.h) \
    $$files(fields/*.h)
	
SOURCES += \
    $$files(auxiliary/*.cpp) \
    $$files(geometry/*.cpp) \
    $$files(fields/*.cpp)
