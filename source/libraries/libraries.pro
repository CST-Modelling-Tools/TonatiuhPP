TEMPLATE = lib
TARGET = Tonatiuh-Libraries
DESTDIR = ..
DEFINES += TONATIUH_LIBRARIES_EXPORT
DEFINES += SUNPATHLIB_EXPORT

include(../config.pri)
CONFIG += object_parallel_to_source

QT += printsupport # for customplot

gcc {
    QMAKE_CXXFLAGS += -Wno-deprecated-declarations # for customplot
    QMAKE_CXXFLAGS += -Wno-attributes # for Eigen
}

INCLUDEPATH += $$PWD/../../libraries/eigen-3.4.0

HEADERS += \
    TonatiuhLibraries.h \
    auxiliary/tiny_obj_loader.h \
    auxiliary/Trace.h \
    Coin3D/ContainerEditorMFVec2.h \
    Coin3D/ContainerViewerMFVec2.h \
    Coin3D/DoubleValuesDelegate.h \
    Coin3D/FieldEditor.h \
    Coin3D/MFVec2.h \
    Coin3D/UserMField.h \
    Coin3D/UserSField.h \
    DistMesh/PolygonMesh.h \
    DistMesh/constants.h \
    DistMesh/delaunator.h \
    DistMesh/distance_function.h \
    DistMesh/distmesh.h \
    DistMesh/functional.h \
    DistMesh/helper.h \
    DistMesh/triangulation.h \
    DistMesh/utils.h \
    math/1D/Grid.h \
    math/1D/Interval.h \
    math/1D/IntervalPeriodic.h \
    math/2D/Box2D.h \
    math/2D/Interpolation2D.h \
    math/2D/Matrix2D.h \
    math/2D/vec2d.h \
    math/2D/vec2i.h \
    math/3D/Box3D.h \
    math/3D/Matrix4x4.h \
    math/3D/Ray.h \
    math/3D/Transform.h \
    math/3D/Transform3D.h \
    math/3D/vec3d.h \
    math/gcf.h \
    QCustomPlot/qcustomplot.h \
    sun/sunpos.h

SOURCES += \
    auxiliary/Trace.cpp \
    auxiliary/tiny_obj_loader.cpp \
    Coin3D/ContainerEditorMFVec2.cpp \
    Coin3D/ContainerViewerMFVec2.cpp \
    Coin3D/DoubleValuesDelegate.cpp \
    Coin3D/MFVec2.cpp \
    Coin3D/UserMField.cpp \
    Coin3D/UserSField.cpp \
    DistMesh/PolygonMesh.cpp \
    DistMesh/distance_function.cpp \
    DistMesh/distmesh.cpp \
    DistMesh/functional.cpp \
    DistMesh/triangulation.cpp \
    DistMesh/utils.cpp \
    math/1D/Grid.cpp \
    math/1D/Interval.cpp \
    math/1D/IntervalPeriodic.cpp \
    math/2D/Box2D.cpp \
    math/2D/vec2d.cpp \
    math/2D/vec2i.cpp \
    math/3D/Box3D.cpp \
    math/3D/Matrix4x4.cpp \
    math/3D/Transform.cpp \
    math/3D/Transform3D.cpp \
    math/3D/vec3d.cpp \
    math/gcf.cpp \
    QCustomPlot/qcustomplot.cpp \
    sun/sunpos.cpp

FORMS += \
    Coin3D/ContainerViewerMFVec2.ui

HEADERS += \
    SunPathLib/SunPathLib.h \
    SunPathLib/calculators/Location.h \
    SunPathLib/calculators/MB/SunPos.h \
    SunPathLib/calculators/NREL/spa.h \
    SunPathLib/calculators/RG/Sun_position_algorithms.h \
    SunPathLib/calculators/SunCalculator.h \
    SunPathLib/calculators/SunCalculatorET.h \
    SunPathLib/calculators/SunCalculatorMB.h \
    SunPathLib/calculators/SunCalculatorNREL.h \
    SunPathLib/calculators/SunCalculatorRG.h \
    SunPathLib/data/FormatTMY.h \
    SunPathLib/data/FormatWSN.h \
    SunPathLib/data/SkyModel.h \
    SunPathLib/data/SkyModelPI.h \
    SunPathLib/data/SunFunctor.h \
    SunPathLib/math/geometry/Interval.h \
    SunPathLib/math/geometry/vec2d.h \
    SunPathLib/math/geometry/vec3d.h \
    SunPathLib/math/math.h \
    SunPathLib/math/matrices/Matrix.h \
    SunPathLib/math/matrices/MatrixNR.h \
    SunPathLib/math/sampling/ErrorAnalysis.h \
    SunPathLib/math/sampling/Interpolator.h \
    SunPathLib/math/sampling/Summator.h \
    SunPathLib/samplers/SkySampler.h \
    SunPathLib/samplers/SunSpatial.h \
    SunPathLib/samplers/SunTemporal.h \
    SunPathLib/samplers/SkyKernels.h \
    SunPathLib/samplers/TimeSampler.h

SOURCES += \
    SunPathLib/calculators/Location.cpp \
    SunPathLib/calculators/MB/SunPos.cpp \
    SunPathLib/calculators/MB/SunPos2.cpp \
    SunPathLib/calculators/NREL/spa.cpp \
    SunPathLib/calculators/SunCalculator.cpp \
    SunPathLib/calculators/SunCalculatorET.cpp \
    SunPathLib/calculators/SunCalculatorMB.cpp \
    SunPathLib/calculators/SunCalculatorNREL.cpp \
    SunPathLib/calculators/SunCalculatorRG.cpp \
    SunPathLib/data/FormatTMY.cpp \
    SunPathLib/data/FormatWSN.cpp \
    SunPathLib/data/SkyModel.cpp \
    SunPathLib/data/SkyModelPI.cpp \
    SunPathLib/data/SunFunctor.cpp \
    SunPathLib/math/geometry/Interval.cpp \
    SunPathLib/math/geometry/vec2d.cpp \
    SunPathLib/math/geometry/vec3d.cpp \
    SunPathLib/math/math.cpp \
    SunPathLib/math/matrices/MatrixNR.cpp \
    SunPathLib/math/sampling/ErrorAnalysis.cpp \
    SunPathLib/math/sampling/Interpolator.cpp \
    SunPathLib/math/sampling/Summator.cpp \
    SunPathLib/samplers/SkySampler.cpp \
    SunPathLib/samplers/SunSpatial.cpp \
    SunPathLib/samplers/SunTemporal.cpp \
    SunPathLib/samplers/SkyKernels.cpp \
    SunPathLib/samplers/TimeSampler.cpp


