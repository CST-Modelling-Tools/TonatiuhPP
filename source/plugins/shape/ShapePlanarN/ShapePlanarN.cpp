#include "ShapePlanarN.h"

#include <Inventor/sensors/SoNodeSensor.h>

#include "kernel/TonatiuhFunctions.h"
#include "kernel/profiles/ProfileRT.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/math/2D/Interpolation2D.h"
#include "libraries/math/3D/Box3D.h"
#include "libraries/math/3D/Ray.h"

SO_NODE_SOURCE(ShapePlanarN)


void ShapePlanarN::initClass()
{
    SO_NODE_INIT_CLASS(ShapePlanarN, ShapeRT, "ShapeRT");
}

ShapePlanarN::ShapePlanarN()
{
    SO_NODE_CONSTRUCTOR(ShapePlanarN);

    SO_NODE_ADD_FIELD( xLimits, (-0.5f, 0.5f) );
    SO_NODE_ADD_FIELD( yLimits, (-0.5f, 0.5f) );
    SO_NODE_ADD_FIELD( dims, (2, 2) );

    float ns[][3] = {
        {0., 0., 1.},
        {0., 0., 1.},
        {0., 0., 1.},
        {0., 0., 1.}
    };
    normals.setValues(0, 4, ns);
    normals.setContainer(this);
    fieldData->addField(this, "normals", &normals);

    m_sensor = QSharedPointer<SoNodeSensor>::create(onSensor, this);
    m_sensor->attach(this);
    onSensor(this, 0);
}

bool ShapePlanarN::intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* profile) const
{
    // r0_z + d_z*t = 0
    double t = -ray.origin.z*ray.invDirection().z;

    if (t < ray.tMin + 1e-5 || t > ray.tMax) return false;

    vec3d pHit = ray.point(t);
    if (!profile->isInside(pHit.x, pHit.y)) return false;

    if (tHit == 0 && dg == 0)
        return true;
    else if (tHit == 0 || dg == 0)
        gcf::SevereError("ShapePlanar::intersect");

    vec2d p(
        m_gridX.toNormalized(pHit.x),
        m_gridY.toNormalized(pHit.y)
    );
    vec3d vN = interpolateLinear<vec3d>(m_matrixNormals, p);
    vN.normalize();

    vec3d vU(vN.z, 0., -vN.x);
    vU.normalize();
    vec3d vV = cross(vN, vU);

    *tHit = t;
    dg->point = pHit;
    dg->u = pHit.x;
    dg->v = pHit.y;
    dg->dpdu = vU;
    dg->dpdv = vV;
    dg->normal = vN;
    dg->shape = this;
    dg->isFront = dot(dg->normal, ray.direction()) <= 0.;
    return true;
}

void ShapePlanarN::updateShapeGL(TShapeKit* parent)
{
    makeQuadMesh(parent, QSize(2, 2));
}

void ShapePlanarN::onSensor(void* data, SoSensor*)
{
    ShapePlanarN* shape = (ShapePlanarN*) data;

    shape->m_gridX = Grid(Interval(
        shape->xLimits.getValue()[0],
        shape->xLimits.getValue()[1]
    ), shape->dims.getValue()[0] - 1);

    shape->m_gridY = Grid(Interval(
        shape->yLimits.getValue()[0],
        shape->yLimits.getValue()[1]
    ), shape->dims.getValue()[1] - 1);

    Matrix2D<vec3d>& matrix = shape->m_matrixNormals;
    matrix.resize(shape->m_gridX.divisions() + 1, shape->m_gridY.divisions() + 1);
    SoMFVec3f& normals = shape->normals;
    for (int n = 0; n < normals.getNum(); ++n)
        matrix.data()[n] = tgf::makeVector3D(*normals.getValues(n));
}
