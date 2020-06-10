TEMPLATE = lib
TARGET = Tonatiuh-Libraries
DESTDIR = ..
DEFINES += TONATIUH_LIBRARIES_EXPORT

include(../config.pri)

QT += printsupport # for customplot

gcc {
    QMAKE_CXXFLAGS += -Wno-deprecated-declarations # for customplot
}

HEADERS += \
    $$files(*.h) \
    $$files(auxiliary/*.h) \
    $$files(fields/*.h) \
    $$files(geometry/*.h) \
    $$files(qcustomplot/*.h) \
    $$files(sun/*.h)
	
SOURCES += \
    $$files(auxiliary/*.cpp) \
    $$files(fields/*.cpp) \
    $$files(geometry/*.cpp) \
    $$files(qcustomplot/*.cpp) \
    $$files(sun/*.cpp)

FORMS += \
    $$files(fields/*.ui)
