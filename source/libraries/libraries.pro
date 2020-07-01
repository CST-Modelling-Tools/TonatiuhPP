TEMPLATE = lib
TARGET = Tonatiuh-Libraries
DESTDIR = ..
DEFINES += TONATIUH_LIBRARIES_EXPORT

include(../config.pri)

QT += printsupport # for customplot

gcc {
    QMAKE_CXXFLAGS += -Wno-deprecated-declarations # for customplot
    QMAKE_CXXFLAGS += -Wno-attributes # for Eigen
}

INCLUDEPATH += $$PWD/../../libraries/eigen-3.3.7

HEADERS += \
    $$files(*.h) \
    $$files(auxiliary/*.h) \
    $$files(Coin3D/*.h) \
    $$files(math/*.h) \
    $$files(DistMesh/*.h) \
    $$files(QCustomPlot/*.h) \
    $$files(sun/*.h)
	
SOURCES += \
    $$files(auxiliary/*.cpp) \
    $$files(Coin3D/*.cpp) \
    $$files(math/*.cpp) \
    $$files(DistMesh/*.cpp) \
    $$files(QCustomPlot/*.cpp) \
    $$files(sun/*.cpp)

FORMS += \
    $$files(Coin3D/*.ui)
