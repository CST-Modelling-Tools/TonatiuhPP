#include "ProfileTriangle.h"
#include "kernel/TonatiuhFunctions.h"
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
    SO_NODE_ADD_FIELD( a, (1., 0.) );
    SO_NODE_ADD_FIELD( b, (0., 1.) );
    SO_NODE_ADD_FIELD( c, (0., 0.) );

    m_sensor = new SoNodeSensor(onSensor, this);
    m_sensor->attach(this);
    onSensor(this, 0);
}

Box3D ProfileTriangle::getBox() const
{
    Box3D box;
    box << Vector3D(tgf::makeVector2D(a.getValue()));
    box << Vector3D(tgf::makeVector2D(b.getValue()));
    box << Vector3D(tgf::makeVector2D(c.getValue()));
    return box;
}

bool ProfileTriangle::isInside(double u, double v) const
{
    if (gcf::eqz(m_det)) return false;

    Vector2D p(u, v);
    p -= m_pC;
    double alpha = cross(p, m_pBC)/m_det;
    if (alpha < 0.) return false;
    double beta = cross(m_pAC, p)/m_det;
    if (beta < 0.) return false;
    if (alpha + beta > 1.) return false;
    return true;
}

QVector<Vector2D> ProfileTriangle::makeMesh(QSize& dims) const
{
    const int iMax = dims.width();
    const int jMax = dims.height();
    QVector<Vector2D> ans;

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

    Vector2D pAC = tgf::makeVector2D(profile->a.getValue());
    Vector2D pBC = tgf::makeVector2D(profile->b.getValue());
    Vector2D pC = tgf::makeVector2D(profile->c.getValue());
    pAC -= pC;
    pBC -= pC;

    profile->m_pAC = pAC;
    profile->m_pBC = pBC;
    profile->m_pC = pC;
    profile->m_det = cross(pAC, pBC);
}
