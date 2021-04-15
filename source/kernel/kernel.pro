TEMPLATE = lib
TARGET = Tonatiuh-Kernel
DESTDIR = ..
DEFINES += TONATIUH_KERNEL_EXPORT

include(../config.pri)

LIBS += -lTonatiuh-Libraries

win32 {
LIBS += -lopengl32
}

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
    material/MaterialRough.h \
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
    scene/TArrayKit.h \
    scene/TCameraKit.h \
#    scene/TIndexedFaceSet.h \
    scene/TPerspectiveCamera.h \
    scene/TSFDoubleUnit.h \
    scene/TSFRotation.h \
    scene/TSceneKit.h \
    scene/TSeparatorKit.h \
    scene/TShapeKit.h \
    scene/TTransform.h \
#    scene/TVertexArrayIndexer.h \
    scene/TerrainKit.h \
    scene/WorldKit.h \
    shape/DifferentialGeometry.h \
    shape/ShapeCone.h \
    shape/ShapeCube.h \
    shape/ShapeCylinder.h \
    shape/ShapeParabolic.h \
    shape/ShapePlanar.h \
    shape/ShapeRT.h \
    shape/ShapeSphere.h \
    sun/SunAperture.h \
    sun/SunKit.h \
    sun/SunPosition.h \
    sun/SunShape.h \
    sun/SunShapePillbox.h \
    trackers/ArmatureJoint.h \
    trackers/TrackerArmature.h \
    trackers/TrackerArmature1A.h \
    trackers/TrackerArmature2A.h \
    trackers/TrackerArmature2AwD.h \
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
    material/MaterialRough.cpp \
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
    scene/TArrayKit.cpp \
    scene/TCameraKit.cpp \
#    scene/TIndexedFaceSet.cpp \
    scene/TPerspectiveCamera.cpp \
    scene/TSFDoubleUnit.cpp \
    scene/TSFRotation.cpp \
    scene/TSceneKit.cpp \
    scene/TSeparatorKit.cpp \
    scene/TShapeKit.cpp \
    scene/TTransform.cpp \
#    scene/TVertexArrayIndexer.cpp \
    scene/TerrainKit.cpp \
    scene/WorldKit.cpp \
    shape/DifferentialGeometry.cpp \
    shape/ShapeCone.cpp \
    shape/ShapeCube.cpp \
    shape/ShapeCylinder.cpp \
    shape/ShapeParabolic.cpp \
    shape/ShapePlanar.cpp \
    shape/ShapeRT.cpp \
    shape/ShapeSphere.cpp \
    sun/SunAperture.cpp \
    sun/SunKit.cpp \
    sun/SunPosition.cpp \
    sun/SunShape.cpp \
    sun/SunShapePillbox.cpp \
    trackers/ArmatureJoint.cpp \
    trackers/TrackerArmature.cpp \
    trackers/TrackerArmature1A.cpp \
    trackers/TrackerArmature2A.cpp \
    trackers/TrackerArmature2AwD.cpp \
    trackers/TrackerKit.cpp \
    trackers/TrackerSolver.cpp \
    trackers/TrackerSolver1A.cpp \
    trackers/TrackerSolver2A.cpp \
    trackers/TrackerTarget.cpp


RESOURCES += resources.qrc

