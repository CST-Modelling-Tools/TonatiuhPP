#include "TroughModel.h"

#include "libraries/geometry/gcf.h"


// rotation around a from m to v
inline double findAngle(const Vector3D& a, const Vector3D& m, const Vector3D& v)
{
    return atan2(dot(a, cross(m, v)), dot(m, v));
}

TroughModel::TroughModel(
    const TrackingDrive& primary,
    const TrackingVertex& facet,
    double angle0
):
    primary(primary),
    facet(facet),
    angle0(angle0)
{
    Vector3D& a = this->primary.axis;
    Vector3D& n = this->facet.normal;
    n-= dot(n, a)*a;
}

Vector3D TroughModel::findFacetPoint(double angle)
{
    return primary.getTransform(angle).transformPoint(facet.shift);
}

double TroughModel::solveReflectionPrimary(const Vector3D& vSun, const Vector3D& rAim)
{
    const Vector3D& a = primary.axis;
    const Vector3D& n = facet.normal;

    Vector3D t = rAim - facet.shift;
    t -= dot(t, a)*a;
    t.normalize();
    Vector3D v0 = -t.reflected(n);

    Vector3D v = vSun - dot(vSun, a)*a;
    v.normalize();

    return findAngle(a, v0, v);
}

double TroughModel::solveReflectionGlobal(const Vector3D& vSun, const Vector3D& rAim)
{
    const Vector3D& a = primary.axis;
    const Vector3D& n0 = facet.normal;

    Vector3D v = vSun - dot(vSun, a)*a;
    v.normalize();

    int iMax = 5; // max iterations
    double deltaMin = 0.01; // accuracy in meters

    double ans;
    Vector3D rFacet = findFacetPoint(angle0);
    for (int i = 0; i < iMax; ++i)
    {
        Vector3D t = rAim - rFacet;
        t -= dot(t, a)*a;
        t.normalize();
        Vector3D n = (v + t).normalized();
        ans = findAngle(a, n0, n);
        rFacet = findFacetPoint(ans);
        double delta = dot(cross(t, a), rAim - rFacet);
        if (abs(delta) > deltaMin) continue;
    }
    return ans;
}

double TroughModel::selectSolution(double solution)
{
    double temp = primary.angles.normalizeAngle(solution);
    if (primary.angles.isInside(temp)) return temp;
    return angle0;
}
