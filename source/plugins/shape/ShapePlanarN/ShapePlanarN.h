#pragma once

#include <QSharedPointer>
#include <Inventor/fields/SoSFVec2i32.h>

#include "kernel/shape/ShapeRT.h"
#include "libraries/math/1D/Grid.h"
#include "libraries/math/2D/Matrix2D.h"
#include "libraries/math/3D/vec3d.h"


class ShapePlanarN: public ShapeRT
{
    SO_NODE_HEADER(ShapePlanarN);

public:
    static void initClass();
    ShapePlanarN();

    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* profile) const;

    SoSFVec2f xLimits;
    SoSFVec2f yLimits;
    SoSFVec2i32 dims;
    SoMFVec3f normals;

    NAME_ICON_FUNCTIONS("PlanarN", ":/ShapePlanarN.png")
    void updateShapeGL(TShapeKit* parent);

protected:
    Matrix2D<vec3d> m_matrixNormals;
    Grid m_gridX;
    Grid m_gridY;

    QSharedPointer<SoNodeSensor> m_sensor;
    static void onSensor(void* data, SoSensor*);
};



class ShapePlanarNFactory:
    public QObject, public ShapeFactoryT<ShapePlanarN>
{
    Q_OBJECT
    Q_INTERFACES(ShapeFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.ShapeFactory")
};
