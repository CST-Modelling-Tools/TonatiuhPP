#include "ProfileRegular.h"

#include "libraries/math/gcf.h"


SO_NODE_SOURCE(ProfileRegular)

void ProfileRegular::initClass()
{
     SO_NODE_INIT_CLASS(ProfileRegular, ProfileRT, "Profile");
}

ProfileRegular::ProfileRegular()
{
    SO_NODE_CONSTRUCTOR(ProfileRegular);
    SO_NODE_ADD_FIELD( n, (6) );
    SO_NODE_ADD_FIELD( r, (1.) );
}

Box3D ProfileRegular::getBox() const
{
    double rV = r.getValue();

    return Box3D(
        vec3d(-rV, -rV, 0.),
        vec3d(rV, rV, 0.)
    );
}

bool ProfileRegular::isInside(double u, double v) const
{
    double r2 = u*u + v*v;
    double rV = r.getValue();
    if (r2 > rV*rV) return false;

    int nV = n.getValue();
    double phiStep = gcf::TwoPi/nV;
    double rMin = rV*cos(phiStep/2.);
    if (r2 < rMin*rMin) return true;

    double phi = atan2(v, u) + phiStep/2. + 90.*gcf::degree;
    phi -= std::floor(phi/phiStep)*phiStep + phiStep/2.;
    return sqrt(r2)*cos(phi) < rMin;
}

QVector<vec2d> ProfileRegular::makeMesh(QSize& dims) const
{
    int nV = n.getValue();
    double phiStep = gcf::TwoPi/nV;
    const int iMax = nV + 1;
    int jMax = std::max(dims.width(), dims.height());
    dims = QSize(iMax, jMax);

    QVector<vec2d> ans;
    for (int i = 0; i <= iMax; ++i) {
        double u = i*phiStep - phiStep/2. - 90.*gcf::degree;
        for (int j = 0; j < jMax; ++j) {
            double vn = j/double(jMax - 1);
            double v = vn*r.getValue();
            ans << v*vec2d(cos(u), sin(u));
        }
    }
    return ans;
}
