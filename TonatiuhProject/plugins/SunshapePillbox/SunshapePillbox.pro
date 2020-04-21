TEMPLATE = lib
CONFIG += plugin
CONFIG -= debug_and_release

include( ../../config.pri)

INCLUDEPATH += . \
			src \
            $$(TONATIUH_ROOT)/plugins \
			$$(TONATIUH_ROOT)/src

# Input
HEADERS = $$files(src/*.h) \
            $$TONATIUH_ROOT/src/source/raytracing/trt.h \
                $$TONATIUH_ROOT/src/source/raytracing/TSunShape.h


SOURCES = $$files(src/*.cpp)  \
                $$TONATIUH_ROOT/src/source/raytracing/TSunShape.cpp

RESOURCES += src/SunshapePillbox.qrc	

LIBS += -L../../bin
DESTDIR = ../../bin/plugins
