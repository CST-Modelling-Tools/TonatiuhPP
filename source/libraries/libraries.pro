TEMPLATE = lib
TARGET = Tonatiuh-Libraries
DESTDIR = ..
DEFINES += TONATIUH_LIBRARIES_EXPORT

include(../config.pri)

QT += printsupport # for customplot

QMAKE_CXXFLAGS += -Wno-deprecated-declarations # for customplot

HEADERS += \
    $$files(*.h) \
    $$files(auxiliary/*.h) \
    $$files(geometry/*.h) \
    $$files(qcustomplot/*.h) \
    $$files(fields/*.h)
	
SOURCES += \
    $$files(auxiliary/*.cpp) \
    $$files(geometry/*.cpp) \
    $$files(qcustomplot/*.cpp) \	
    $$files(fields/*.cpp)
