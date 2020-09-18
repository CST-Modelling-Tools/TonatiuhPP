TEMPLATE = lib
TARGET = Tonatiuh-Kernel
DESTDIR = ..
DEFINES += TONATIUH_KERNEL_EXPORT

include(../config.pri)

LIBS += -lTonatiuh-Libraries

HEADERS += \
    TonatiuhKernel.h \
    air/AirExponential.h \
    air/AirKit.h \
    air/AirPolynomial.h \
    air/AirTransmission.h \
    air/AirVacuum.h \
    component/ComponentFactory.h \
    material/MaterialAbsorber.h \
    material/MaterialFresnelUnpolarized.h \
    material/MaterialRT.h \
    material/MaterialTransparent.h \
    material/MaterialVirtual.h \
    node/TFactory.h \
    node/TNode.h \
    node/TonatiuhFunctions.h \
    node/TonatiuhTypes.h \
    photons/Photon.h \
    photons/PhotonsAbstract.h \
    photons/PhotonsBuffer.h \
    photons/PhotonsSettings.h \
    photons/PhotonsWidget.h \
    profiles/ProfileBox.h \
    profiles/ProfileCircular.h \
    profiles/ProfilePolygon.h \
    profiles/ProfileRT.h \
    profiles/ProfileRectangular.h \
    profiles/ProfileRegular.h \
    profiles/ProfileTriangle.h \
    random/Random.h \
    random/RandomParallel.h \
    random/RandomSTL.h \
    run/InstanceNode.h \
    run/RayTracer.h \
    scene/GridNode.h \
    scene/LocationNode.h \
    scene/MaterialGL.h \
    scene/TSFRotation.h \
    scene/TSceneKit.h \
    scene/TSeparatorKit.h \
    scene/TShapeKit.h \
    scene/TTransform.h \
    scene/TerrainKit.h \
    scene/WorldKit.h \
    shape/DifferentialGeometry.h \
    shape/ShapeCube.h \
    shape/ShapePlanar.h \
    shape/ShapeRT.h \
    sun/SunAperture.h \
    sun/SunKit.h \
    sun/SunPosition.h \
    sun/SunShape.h \
    sun/SunShapePillbox.h \
    trackers/ArmatureJoint.h \
    trackers/TrackerArmature.h \
    trackers/TrackerArmature1A.h \
    trackers/TrackerArmature2A.h \
    trackers/TrackerKit.h \
    trackers/TrackerSolver.h \
    trackers/TrackerSolver1A.h \
    trackers/TrackerSolver2A.h \
    trackers/TrackerTarget.h


SOURCES += \
    air/AirExponential.cpp \
    air/AirKit.cpp \
    air/AirPolynomial.cpp \
    air/AirTransmission.cpp \
    air/AirVacuum.cpp \
    material/MaterialAbsorber.cpp \
    material/MaterialFresnelUnpolarized.cpp \
    material/MaterialRT.cpp \
    material/MaterialTransparent.cpp \
    material/MaterialVirtual.cpp \
    node/TNode.cpp \
    node/TonatiuhFunctions.cpp \
    photons/Photon.cpp \
    photons/PhotonsAbstract.cpp \
    photons/PhotonsBuffer.cpp \
    photons/PhotonsSettings.cpp \
    photons/PhotonsWidget.cpp \
    profiles/ProfileBox.cpp \
    profiles/ProfileCircular.cpp \
    profiles/ProfilePolygon.cpp \
    profiles/ProfileRT.cpp \
    profiles/ProfileRectangular.cpp \
    profiles/ProfileRegular.cpp \
    profiles/ProfileTriangle.cpp \
    random/RandomParallel.cpp \
    random/RandomSTL.cpp \
    run/InstanceNode.cpp \
    run/RayTracer.cpp \
    scene/GridNode.cpp \
    scene/LocationNode.cpp \
    scene/MaterialGL.cpp \
    scene/TSFRotation.cpp \
    scene/TSceneKit.cpp \
    scene/TSeparatorKit.cpp \
    scene/TShapeKit.cpp \
    scene/TTransform.cpp \
    scene/TerrainKit.cpp \
    scene/WorldKit.cpp \
    shape/DifferentialGeometry.cpp \
    shape/ShapeCube.cpp \
    shape/ShapePlanar.cpp \
    shape/ShapeRT.cpp \
    sun/SunAperture.cpp \
    sun/SunKit.cpp \
    sun/SunPosition.cpp \
    sun/SunShape.cpp \
    sun/SunShapePillbox.cpp \
    trackers/ArmatureJoint.cpp \
    trackers/TrackerArmature.cpp \
    trackers/TrackerArmature1A.cpp \
    trackers/TrackerArmature2A.cpp \
    trackers/TrackerKit.cpp \
    trackers/TrackerSolver.cpp \
    trackers/TrackerSolver1A.cpp \
    trackers/TrackerSolver2A.cpp \
    trackers/TrackerTarget.cpp


RESOURCES += resources.qrc
