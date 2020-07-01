#include "ProfilePolygon.h"
#include "kernel/TonatiuhFunctions.h"
#include "libraries/math/gcf.h"

#include <Inventor/sensors/SoNodeSensor.h>


SO_NODE_SOURCE(ProfilePolygon)

void ProfilePolygon::initClass()
{
     SO_NODE_INIT_CLASS(ProfilePolygon, ProfileRT, "Profile");
}

ProfilePolygon::ProfilePolygon()
{
    SO_NODE_CONSTRUCTOR(ProfilePolygon);

    SO_NODE_ADD_FIELD( points, (0., 0.) );
    SbVec2f vs[] = {
        SbVec2f(1., 0.),
        SbVec2f(0., 1.),
        SbVec2f(-1., 0.),
        SbVec2f(0., -1.)
    };
    points.setValues(0, 4, vs);
    points.setNames({"x", "y"});

    m_sensor = new SoNodeSensor(onSensor, this);
    m_sensor->attach(this);
    onSensor(this, 0);
}

Box3D ProfilePolygon::getBox() const
{
    QRectF rect = m_polygon.boundingRect();
    Box3D box(
        vec3d(rect.left(), rect.top(), 0.),
        vec3d(rect.right(), rect.bottom(), 0.)
    );
    double zMax = 0.01*box.extent().max();
    box.pMin.z = -zMax;
    box.pMax.z = zMax;
    return box;
}

bool ProfilePolygon::isInside(double u, double v) const
{
    return m_polygon.containsPoint(QPointF(u, v), Qt::OddEvenFill);
}

// use profile field of Coin ShapeKit
QVector<vec2d> ProfilePolygon::makeMesh(QSize& dims) const
{
    const int iMax = dims.width();
    const int jMax = dims.height();
    QVector<vec2d> ans;

    QRectF rect = m_polygon.boundingRect();
    vec2d v0(rect.left(), rect.top());
    vec2d dv(rect.width(), rect.height());
    for (int i = 0; i < iMax; ++i) {
        double un = i/double(iMax - 1);
        for (int j = 0; j < jMax; ++j) {
            double vn = j/double(jMax - 1);
            ans << v0 + vec2d(un, vn)*dv;
        }
    }
    return ans;
}

ProfilePolygon::~ProfilePolygon()
{
    delete m_sensor;
}

void ProfilePolygon::onSensor(void* data, SoSensor*)
{
    ProfilePolygon* profile = (ProfilePolygon*) data;
    MFVec2& points = profile->points;
    QPolygonF& polygon = profile->m_polygon;

    polygon.clear();
    for (int n = 0; n < points.getNum(); ++n) {
       const SbVec2f& v = *points.getValues(n);
        polygon << QPointF(v[0], v[1]);
    }
}
