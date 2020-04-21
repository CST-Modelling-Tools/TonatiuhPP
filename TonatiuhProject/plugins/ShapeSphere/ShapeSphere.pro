TEMPLATE = lib
CONFIG += plugin

include( ../../config.pri)

INCLUDEPATH += . \
    src \
    $$TONATIUH_ROOT/plugins \
    $$TONATIUH_ROOT/src

HEADERS = $$files(src/*.h) \
    $$TONATIUH_ROOT/src/source/raytracing/DifferentialGeometry.h \
    $$TONATIUH_ROOT/src/source/raytracing/TMaterial.h \
    $$TONATIUH_ROOT/src/source/raytracing/trt.h \
    $$TONATIUH_ROOT/src/source/raytracing/TShape.h \
    $$TONATIUH_ROOT/src/source/raytracing/TShapeKit.h

SOURCES = $$files(src/*.cpp)  \
    $$TONATIUH_ROOT/src/source/raytracing/DifferentialGeometry.cpp \
    $$TONATIUH_ROOT/src/source/raytracing/TMaterial.cpp \
    $$TONATIUH_ROOT/src/source/raytracing/TShape.cpp \
    $$TONATIUH_ROOT/src/source/raytracing/TShapeKit.cpp

RESOURCES += src/ShapeSphere.qrc
 
