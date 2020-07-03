#pragma once

#include <QSharedPointer>
#include <Inventor/fields/SoMFInt32.h>

#include "kernel/shape/ShapeRT.h"
#include "libraries/math/3D/Box3D.h"

class ShapeMesh: public ShapeRT
{
    SO_NODE_HEADER(ShapeMesh);

public:
    static void initClass();
    ShapeMesh();

    Box3D getBox(ProfileRT* profile) const;
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* profile) const;

    SoMFVec3f vertices;
    SoMFVec3f normals;
    SoMFInt32 facesVertices;
    SoMFInt32 facesNormals;

    SoSFString file;

    NAME_ICON_FUNCTIONS("Mesh", ":/ShapeMesh.png")
    void updateShapeGL(TShapeKit* parent);

protected:
    Box3D m_box;
    QSharedPointer<SoFieldSensor> m_sensor;
    static void onSensor(void* data, SoSensor*);
};



class ShapeMeshFactory:
    public QObject, public ShapeFactoryT<ShapeMesh>
{
    Q_OBJECT
    Q_INTERFACES(ShapeFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.ShapeFactory")
};

// https://en.wikipedia.org/wiki/Wavefront_.obj_file
