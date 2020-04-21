TEMPLATE = lib
CONFIG += plugin
CONFIG -= debug_and_release

include( ../../config.pri)

INCLUDEPATH += . \
    src \
    $$TONATIUH_ROOT/plugins \
    $$TONATIUH_ROOT/src

# Input
HEADERS = $$files(src/*.h) \
#            $$TONATIUH_ROOT/src/source/gui/InstanceNode.h \
#                         $$TONATIUH_ROOT/src/source/gui/PathWrapper.h \
            $$TONATIUH_ROOT/src/source/gui/PhotonMapExport.h \
            $$TONATIUH_ROOT/src/source/gui/PhotonMapExportFactory.h \
            $$TONATIUH_ROOT/src/source/gui/PhotonMapExportParametersWidget.h\
#                         $$TONATIUH_ROOT/src/source/gui/SceneModel.h \
#                         $$TONATIUH_ROOT/src/source/raytracing/DifferentialGeometry.h \
#            $$TONATIUH_ROOT/src/source/raytracing/Photon.h \
#                         $$TONATIUH_ROOT/src/source/raytracing/TCube.h \
#                         $$TONATIUH_ROOT/src/source/raytracing/TDefaultMaterial.h\
#                         $$TONATIUH_ROOT/src/source/raytracing/TDefaultSunShape.h \
#                         $$TONATIUH_ROOT/src/source/raytracing/TDefaultTracker.h \
#                         $$TONATIUH_ROOT/src/source/raytracing/TDefaultTransmissivity.h \
#                         $$TONATIUH_ROOT/src/source/raytracing/TLightKit.h \
#                         $$TONATIUH_ROOT/src/source/raytracing/TLightShape.h \
#                         $$TONATIUH_ROOT/src/source/raytracing/TMaterial.h \
#            $$TONATIUH_ROOT/src/source/raytracing/TSceneKit.h \
#            $$TONATIUH_ROOT/src/source/raytracing/TSceneTracker.h \
#            $$TONATIUH_ROOT/src/source/raytracing/TSeparatorKit.h \
#            $$TONATIUH_ROOT/src/source/raytracing/TShape.h \
#            $$TONATIUH_ROOT/src/source/raytracing/TShapeKit.h \
#                         $$TONATIUH_ROOT/src/source/raytracing/TSunShape.h \
#            $$TONATIUH_ROOT/src/source/raytracing/TTracker.h \
#            $$TONATIUH_ROOT/src/source/raytracing/TTrackerForAiming.h \
#            $$TONATIUH_ROOT/src/source/raytracing/TTransmissivity.h \
#            $$TONATIUH_ROOT/src/source/raytracing/TTransmissivity.h

SOURCES = $$files(src/*.cpp)  \
#                         $$TONATIUH_ROOT/src/source/raytracing/DifferentialGeometry.cpp \
#            $$TONATIUH_ROOT/src/source/gui/InstanceNode.cpp \
#                         $$TONATIUH_ROOT/src/source/gui/PathWrapper.cpp \
            $$TONATIUH_ROOT/src/source/gui/PhotonMapExport.cpp \
            $$TONATIUH_ROOT/src/source/gui/PhotonMapExportParametersWidget.cpp \
#                         $$TONATIUH_ROOT/src/source/gui/SceneModel.cpp \
#            $$TONATIUH_ROOT/src/source/raytracing/Photon.cpp \
#                         $$TONATIUH_ROOT/src/source/raytracing/TCube.cpp \
#                         $$TONATIUH_ROOT/src/source/raytracing/TDefaultMaterial.cpp \
#                         $$TONATIUH_ROOT/src/source/raytracing/TDefaultSunShape.cpp \
#                         $$TONATIUH_ROOT/src/source/raytracing/TDefaultTracker.cpp \
#                         $$TONATIUH_ROOT/src/source/raytracing/TDefaultTransmissivity.cpp \
#                         $$TONATIUH_ROOT/src/source/raytracing/TLightKit.cpp \
#                         $$TONATIUH_ROOT/src/source/raytracing/TLightShape.cpp \
#                         $$TONATIUH_ROOT/src/source/raytracing/TMaterial.cpp \
#            $$TONATIUH_ROOT/src/source/raytracing/TSceneKit.cpp \
#            $$TONATIUH_ROOT/src/source/raytracing/TSceneTracker.cpp \
#            $$TONATIUH_ROOT/src/source/raytracing/TSeparatorKit.cpp \
#            $$TONATIUH_ROOT/src/source/raytracing/TShape.cpp \
#            $$TONATIUH_ROOT/src/source/raytracing/TShapeKit.cpp \
#                         $$TONATIUH_ROOT/src/source/raytracing/TSunShape.cpp \
#            $$TONATIUH_ROOT/src/source/raytracing/TTracker.cpp \
#            $$TONATIUH_ROOT/src/source/raytracing/TTrackerForAiming.cpp \
#            $$TONATIUH_ROOT/src/source/raytracing/TTransmissivity.cpp


