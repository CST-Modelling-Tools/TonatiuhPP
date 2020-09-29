#include "ProfilePolygon.h"

#include <Inventor/sensors/SoNodeSensor.h>

#include "kernel/node/TonatiuhFunctions.h"
#include "libraries/math/gcf.h"


SO_NODE_SOURCE(ProfilePolygon)

void ProfilePolygon::initClass()
{
     SO_NODE_INIT_CLASS(ProfilePolygon, ProfileRT, "Profile");
}

ProfilePolygon::ProfilePolygon()
{
    SO_NODE_CONSTRUCTOR(ProfilePolygon);

//    SO_NODE_ADD_FIELD( points, (0., 0.) ); // single value only
//    float vs[][2] = { // rhombus
//        {0.5, 0.},
//        {0., 0.5},
//        {-0.5, 0.},
//        {0., -0.5}
//    };
    float vs[][2] = { // chevron
        {0., 0.},
        {0.5, 0.},
        {0.5, 0.5},
        {-0.5, 0.5},
        {-0.5, -0.5},
        {0., -0.5}
    };
    points.setValues(0, 4, vs);
    points.setContainer(this);
    fieldData->addField(this, "points", &points);
//    points.setNames({"x", "y"}); // for UserField

    m_sensor = QSharedPointer<SoNodeSensor>::create(onSensor, this);
    m_sensor->attach(this);
    onSensor(this, 0);
}

Box2D ProfilePolygon::getBox() const
{
    return m_box;
}

bool ProfilePolygon::isInside(double u, double v) const
{
    return m_polygon.containsPoint(QPointF(u, v), Qt::OddEvenFill);
}

QVector<vec2d> ProfilePolygon::makeMesh(QSize& dims) const //?
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

void ProfilePolygon::onSensor(void* data, SoSensor*)
{
    ProfilePolygon* profile = (ProfilePolygon*) data;
    SoMFVec2f& points = profile->points;
    QPolygonF& polygon = profile->m_polygon;

    polygon.clear();
    profile->m_box = Box2D();
    for (int n = 0; n < points.getNum(); ++n) {
       const SbVec2f& v = *points.getValues(n);
       polygon << QPointF(v[0], v[1]);
       profile->m_box << vec2d(v[0], v[1]);
    }
}
