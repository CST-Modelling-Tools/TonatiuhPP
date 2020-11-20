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
    SO_NODE_ADD_FIELD( r, (1.) );
    SO_NODE_ADD_FIELD( n, (6) );
}

Box2D ProfileRegular::getBox() const
{
    double rV = r.getValue();
    return Box2D(
        -vec2d(rV, rV),
        vec2d(rV, rV)
    );
}

bool ProfileRegular::isInside(double u, double v) const
{
    double r2 = u*u + v*v;
    double rOut = r.getValue();
    if (r2 > rOut*rOut) return false;

    double phiStep = gcf::TwoPi/n.getValue();
    double rIn = rOut*cos(phiStep/2.);
    if (r2 < rIn*rIn) return true;

    double phi = atan2(v, u) + phiStep/2. + 90.*gcf::degree;
    phi -= std::floor(phi/phiStep)*phiStep + phiStep/2.;
    return sqrt(r2)*cos(phi) < rIn;
}

QVector<vec2d> ProfileRegular::makeMesh(QSize& dims) const
{
    int nV = n.getValue();
    double phiStep = gcf::TwoPi/nV;
    const int iMax = nV + 1;
//    int jMax = std::max(dims.width(), dims.height());
    int jMax = dims.width();
    dims = QSize(iMax, jMax);

    QVector<vec2d> ans;
    double u0 = phiStep/2. + 90.*gcf::degree;
    double rStep = r.getValue()/(jMax - 1);
    for (int i = 0; i <= iMax; ++i) {
        double u = i*phiStep - u0;
        vec2d rho(cos(u), sin(u));
        for (int j = 0; j < jMax; ++j) {
            double v = j*rStep;
            ans << v*rho;
        }
    }
    return ans;
}
