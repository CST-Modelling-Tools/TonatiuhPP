
TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( ../../config.pri )

INCLUDEPATH += . \
			src \
			$$(TONATIUH_ROOT)/plugins \
            $$(TONATIUH_ROOT)/src

# Input
HEADERS = src/*.h \     	
           	$$(TONATIUH_ROOT)/src/source/raytracing/DifferentialGeometry.h \
           	$$(TONATIUH_ROOT)/src/source/raytracing/TMaterial.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/trt.h \
           	$$(TONATIUH_ROOT)/src/source/raytracing/TShape.h \ 
           	$$(TONATIUH_ROOT)/src/source/raytracing/TShapeKit.h

SOURCES = src/*.cpp  \  	
           	$$(TONATIUH_ROOT)/src/source/raytracing/DifferentialGeometry.cpp \
           	$$(TONATIUH_ROOT)/src/source/raytracing/TMaterial.cpp \
           	$$(TONATIUH_ROOT)/src/source/raytracing/TShape.cpp \ 
           	$$(TONATIUH_ROOT)/src/source/raytracing/TShapeKit.cpp

RESOURCES += src/ShapeSphericalPolygon.qrc
TARGET        = ShapeSphericalPolygon
 
CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/ShapeSphericalPolygon	
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/ShapeSphericalPolygon
}
