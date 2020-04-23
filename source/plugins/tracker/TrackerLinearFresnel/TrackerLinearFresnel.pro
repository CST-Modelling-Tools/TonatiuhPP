TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( ../../config.pri )

INCLUDEPATH +=  . \
				src  \
                $$(TONATIUH_ROOT)/src/source/geometry \
                $$(TONATIUH_ROOT)/src/source/raytracing

# Input
HEADERS = src/*.h \
            $$(TONATIUH_ROOT)/src/source/geometry/tgf.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TDefaultTracker.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TDefaultTransmissivity.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/trt.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TSceneKit.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TSeparatorKit.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTracker.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTrackerForAiming.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTrackerFactory.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTransmissivity.h


SOURCES = src/*.cpp \
            $$(TONATIUH_ROOT)/src/source/geometry/tgf.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TDefaultTracker.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TDefaultTransmissivity.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TSceneKit.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TSeparatorKit.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTracker.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTrackerForAiming.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTransmissivity.cpp

RESOURCES += src/TrackerLinearFresnel.qrc


TARGET        = TrackerLinearFresnel


LIBS +=-L$$(TDE_ROOT)/local/lib -lCoin -lSoQt
    
CONFIG(debug, debug|release) {
    DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/TrackerLinearFresnel    
}
else { 
    DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/TrackerLinearFresnel
}   