#include "ProfileCircular.h"

#include "libraries/math/gcf.h"


SO_NODE_SOURCE(ProfileCircular)

void ProfileCircular::initClass()
{
     SO_NODE_INIT_CLASS(ProfileCircular, ProfileRT, "Profile");
}

ProfileCircular::ProfileCircular()
{
    SO_NODE_CONSTRUCTOR(ProfileCircular);
    isBuiltIn = TRUE;
    SO_NODE_ADD_FIELD( rMin, (0.) );
    SO_NODE_ADD_FIELD( rMax, (1.) );
    SO_NODE_ADD_FIELD( phiMin, (-180.) );
    SO_NODE_ADD_FIELD( phiMax, (180.) );
}

Box2D ProfileCircular::getBox() const
{
    double rMinV = rMin.getValue();
    double rMaxV = rMax.getValue();
    double phiMinV = phiMin.getValue()*gcf::degree;
    double phiMaxV = phiMax.getValue()*gcf::degree;

    double xMin = cos(phiMinV);
    double xMax = cos(phiMaxV);
    double yMin = sin(phiMinV);
    double yMax = sin(phiMaxV);
    if (xMin > xMax) std::swap(xMin, xMax);
    if (yMin > yMax) std::swap(yMin, yMax);

    if (phiMinV <= 0. && 0. <= phiMaxV)
        xMax = 1.;
    if (phiMinV <= -gcf::pi || phiMaxV >= gcf::pi)
        xMin = -1.;
    if (phiMinV <= 0.5*gcf::pi && 0.5*gcf::pi <= phiMaxV)
        yMax = 1.;
    if (phiMinV <= -0.5*gcf::pi && -0.5*gcf::pi <= phiMaxV)
        yMin = -1.;

    xMax *= xMax > 0. ? rMaxV : rMinV;
    xMin *= xMin > 0. ? rMinV : rMaxV;

    yMax *= yMax > 0. ? rMaxV : rMinV;
    yMin *= yMin > 0. ? rMinV : rMaxV;

    return Box2D(
        vec2d(xMin, yMin),
        vec2d(xMax, yMax)
    );
}

bool ProfileCircular::isInside(double u, double v) const
{
    double r2 = u*u + v*v;
    if (r2 < gcf::pow2(rMin.getValue())) return false;
    if (r2 > gcf::pow2(rMax.getValue())) return false;
    double phi = atan2(v, u)/gcf::degree;
    if (phi < phiMin.getValue()) return false;
    if (phi > phiMax.getValue()) return false;
    return true;
}

QVector<vec2d> ProfileCircular::makeMesh(QSize& dims) const
{
    double s = (phiMax.getValue() - phiMin.getValue())/360;
    int iMax = 1 + ceil(48*s);
//    int jMax = std::max(dims.width(), dims.height());//?
    int jMax = dims.width();
    dims = QSize(iMax, jMax);

    QVector<vec2d> ans;
    for (int i = 0; i < iMax; ++i) {
        double u = gcf::lerp(phiMin.getValue(), phiMax.getValue(), i/double(iMax - 1))*gcf::degree;
        vec2d rho(cos(u), sin(u));
        for (int j = 0; j < jMax; ++j) {
            double v = gcf::lerp(rMin.getValue(), rMax.getValue(), j/double(jMax - 1));
            ans << v*rho;
        }
    }
    return ans;
}
