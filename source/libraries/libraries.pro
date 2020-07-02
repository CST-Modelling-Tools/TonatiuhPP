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
    TonatiuhLibraries.h \
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
    math/1D/MatrixGrid.h \
    math/2D/Box2D.h \
    math/2D/Grid2D.h \
    math/2D/Matrix2D.h \
    math/2D/MatrixGrid2D.h \
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
    math/1D/MatrixGrid.cpp \
    math/2D/Box2D.cpp \
    math/2D/Grid2D.cpp \
    math/2D/MatrixGrid2D.cpp \
    math/2D/vec2d.cpp \
    math/2D/vec2i.cpp \
    math/3D/Box3D.cpp \
    math/3D/Matrix4x4.cpp \
    math/3D/Transform.cpp \
    math/3D/Transform3D.cpp \
    math/3D/vec3d.cpp \
    math/gcf.cpp \
    QCustomPlot/qcustomplot.cpp \
    sun\sunpos.cpp

FORMS += \
    Coin3D/ContainerViewerMFVec2.ui
