TEMPLATE = lib
TARGET = Tonatiuh-Libraries
DESTDIR = ..
DEFINES += TONATIUH_LIBRARIES_EXPORT

include(../config.pri)

QT += printsupport # for customplot

gcc {
    QMAKE_CXXFLAGS += -Wno-deprecated-declarations # for customplot
}

INCLUDEPATH += $$PWD/../../libraries/eigen-3.3.7

HEADERS += \
    $$files(*.h) \
    $$files(auxiliary/*.h) \
    $$files(fields/*.h) \
    $$files(geometry/*.h) \
    $$files(DistMesh/*.h) \
    $$files(qcustomplot/*.h) \
    $$files(sun/*.h)
	
SOURCES += \
    $$files(auxiliary/*.cpp) \
    $$files(fields/*.cpp) \
    $$files(geometry/*.cpp) \
    $$files(DistMesh/*.cpp) \
    $$files(qcustomplot/*.cpp) \
    $$files(sun/*.cpp)

FORMS += \
    $$files(fields/*.ui)
