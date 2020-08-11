TEMPLATE = lib
TARGET = Tonatiuh-Kernel
DESTDIR = ..
DEFINES += TONATIUH_KERNEL_EXPORT

include(../config.pri)

LIBS += -lTonatiuh-Libraries

HEADERS += \
    TonatiuhFunctions.h \
    TonatiuhKernel.h \
    TonatiuhTypes.h \
    air/AirVacuum.h \
    scene/GridNode.h \
    sun/SunKitW.h \
    sun/SunPosition.h \
    trf.h \
    air/AirExponential.h \
    air/AirKit.h \
    air/AirPolynomial.h \
    air/AirTransmission.h \
    profiles/ProfileBox.h \
    profiles/ProfileCircular.h \
    profiles/ProfilePolygon.h \
    profiles/ProfileRectangular.h \
    profiles/ProfileRegular.h \
    profiles/ProfileRT.h \
    profiles/ProfileTriangle.h \
    component/ComponentFactory.h \
    scene/LocationNode.h \
    scene/TAbstract.h \
    scene/TerrainKit.h \
    scene/TFactory.h \
    scene/TSceneKit.h \
    scene/TSeparatorKit.h \
    scene/TShapeKit.h \
    scene/WorldKit.h \
    material/MaterialAbsorber.h \
    material/MaterialRT.h \
    material/MaterialVirtual.h \
    photons/Photon.h \
    photons/PhotonsAbstract.h \
    photons/PhotonsBuffer.h \
    photons/PhotonsSettings.h \
    photons/PhotonsWidget.h \
    random/Random.h \
    random/RandomParallel.h \
    random/RandomSTL.h \
    run/InstanceNode.h \
    run/RayTracer.h \
    shape/DifferentialGeometry.h \
    shape/ShapeCube.h \
    shape/ShapePlanar.h \
    shape/ShapeRT.h \
    sun/SunAperture.h \
    sun/SunKit.h \
    sun/SunPillbox.h \
    sun/SunShape.h \
    trackers/Tracker.h \
    trackers/TrackingDrive.h


SOURCES += \
    TonatiuhFunctions.cpp \
    air/AirVacuum.cpp \
    scene/GridNode.cpp \
    sun/SunKitW.cpp \
    sun/SunPosition.cpp \
    trf.cpp \
    air/AirExponential.cpp \
    air/AirKit.cpp \
    air/AirPolynomial.cpp \
    air/AirTransmission.cpp \
    profiles/ProfileBox.cpp \
    profiles/ProfileCircular.cpp \
    profiles/ProfilePolygon.cpp \
    profiles/ProfileRectangular.cpp \
    profiles/ProfileRegular.cpp \
    profiles/ProfileRT.cpp \
    profiles/ProfileTriangle.cpp \
    scene/LocationNode.cpp \
    scene/TerrainKit.cpp \
    scene/TSceneKit.cpp \
    scene/TSeparatorKit.cpp \
    scene/TShapeKit.cpp \
    scene/WorldKit.cpp \
    material/MaterialAbsorber.cpp \
    material/MaterialRT.cpp \
    material/MaterialVirtual.cpp \
    photons/Photon.cpp \
    photons/PhotonsAbstract.cpp \
    photons/PhotonsBuffer.cpp \
    photons/PhotonsSettings.cpp \
    photons/PhotonsWidget.cpp \
    random/RandomParallel.cpp \
    random/RandomSTL.cpp \
    run/InstanceNode.cpp \
    run/RayTracer.cpp \
    shape/DifferentialGeometry.cpp \
    shape/ShapeCube.cpp \
    shape/ShapePlanar.cpp \
    shape/ShapeRT.cpp \
    sun/SunAperture.cpp \
    sun/SunKit.cpp \
    sun/SunPillbox.cpp \
    sun/SunShape.cpp \
    trackers/Tracker.cpp \
    trackers/TrackingDrive.cpp


RESOURCES += resources.qrc
