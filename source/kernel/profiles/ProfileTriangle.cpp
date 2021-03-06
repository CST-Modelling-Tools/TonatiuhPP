#include "ProfileTriangle.h"
#include "kernel/node/TonatiuhFunctions.h"
#include "libraries/math/gcf.h"

#include <Inventor/sensors/SoNodeSensor.h>


SO_NODE_SOURCE(ProfileTriangle)

void ProfileTriangle::initClass()
{
     SO_NODE_INIT_CLASS(ProfileTriangle, ProfileRT, "Profile");
}

ProfileTriangle::ProfileTriangle()
{
    SO_NODE_CONSTRUCTOR(ProfileTriangle);
    SO_NODE_ADD_FIELD( a, (0.5, -0.5) );
    SO_NODE_ADD_FIELD( b, (-0.5, 0.5) );
    SO_NODE_ADD_FIELD( c, (-0.5, -0.5) );

    m_sensor = new SoNodeSensor(onSensor, this);
    m_sensor->attach(this);
    onSensor(this, 0);
}

Box2D ProfileTriangle::getBox() const
{
    Box2D box;
    box << tgf::makeVector2D(a.getValue());
    box << tgf::makeVector2D(b.getValue());
    box << tgf::makeVector2D(c.getValue());
    return box;
}

void ProfileTriangle::setBox(const Box2D& box)
{
    a = SbVec2f(box.max().x, box.min().y);
    b = SbVec2f(box.min().x, box.max().y);
    c = SbVec2f(box.min().x, box.min().y);
}

bool ProfileTriangle::isInside(double u, double v) const
{
    if (gcf::eqz(m_det)) return false;

    vec2d p(u, v);
    p -= m_pC;
    double alpha = cross(p, m_pBC)/m_det;
    if (alpha < 0.) return false;
    double beta = cross(m_pAC, p)/m_det;
    if (beta < 0.) return false;
    if (alpha + beta > 1.) return false;
    return true;
}

QVector<vec2d> ProfileTriangle::makeMesh(QSize& dims) const
{
    const int iMax = dims.width();
    const int jMax = dims.height();
    QVector<vec2d> ans;

    for (int i = 0; i < iMax; ++i) {
        double un = i/double(iMax - 1);
        for (int j = 0; j < jMax; ++j) {
            double vn = j/double(jMax - 1);
            ans << m_pC + un*m_pAC + vn*(1. - un)*m_pBC;
        }
    }
    return ans;
}

ProfileTriangle::~ProfileTriangle()
{
    delete m_sensor;
}

void ProfileTriangle::onSensor(void* data, SoSensor*)
{
    ProfileTriangle* profile = (ProfileTriangle*) data;

    vec2d pAC = tgf::makeVector2D(profile->a.getValue());
    vec2d pBC = tgf::makeVector2D(profile->b.getValue());
    vec2d pC = tgf::makeVector2D(profile->c.getValue());
    pAC -= pC;
    pBC -= pC;

    profile->m_pAC = pAC;
    profile->m_pBC = pBC;
    profile->m_pC = pC;
    profile->m_det = cross(pAC, pBC);
}
