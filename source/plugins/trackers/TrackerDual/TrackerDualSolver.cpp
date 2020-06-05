#include "TrackerDualSolver.h"

#include "libraries/geometry/gcf.h"

#include <QVector>

// rotation around a from m to v
inline double findAngle(const Vector3D& a, const Vector3D& m, const Vector3D& v, double av)
{
    return atan2(dot(a, cross(m, v)), dot(m, v) - av*av);
}

TrackerDualSolver::TrackerDualSolver(
    const TrackingDrive& primary,
    const TrackingDrive& secondary,
    const TrackingVertex& facet,
    const Angles& angles0
):
    primary(primary),
    secondary(secondary),
    facet(facet),
    angles0(angles0)
{

}

Vector3D TrackerDualSolver::findFacetPoint(const Angles& angles)
{
    Vector3D r = secondary.getTransform(angles.y).transformPoint(facet.shift);
    return primary.getTransform(angles.x).transformPoint(r);
}

// rotate v0 to v
QVector<Angles> TrackerDualSolver::solveRotation(const Vector3D& v0, const Vector3D& v)
{
    const Vector3D& a = primary.axis;
    const Vector3D& b = secondary.axis;

    Vector3D k = cross(a, b);
    double k2 = k.norm2();
    double ab = dot(a, b);
    double det = 1. - ab*ab;
    if (std::abs(det) < 1e-8) return {};

    double av = dot(a, v);
    double bv0 = dot(b, v0);
    double ma = (av - ab*bv0)/det;
    double mb = (bv0 - ab*av)/det;
    double mk = 1. - ma*ma - mb*mb - 2.*ma*mb*ab;
    if (mk < 0.) return {};

    mk = sqrt(mk/k2);
    Vector3D m0 = ma*a + mb*b;
    QVector<Angles> ans;
    Vector3D m = m0 - mk*k;
    ans << Angles(findAngle(a, m, v, av), findAngle(b, v0, m, bv0));
    m = m0 + mk*k;
    ans << Angles(findAngle(a, m, v, av), findAngle(b, v0, m, bv0));
    return ans;
}

// rotate facet.normal to normal
QVector<Angles> TrackerDualSolver::solveFacetNormal(const Vector3D& normal)
{
    return solveRotation(facet.normal, normal);
}

QVector<Angles> TrackerDualSolver::solveReflectionSecondary(const Vector3D& vSun, const Vector3D& rAim)
{
    Vector3D vTarget0 = (rAim - facet.shift).normalized();
    Vector3D vSun0 = -vTarget0.reflected(facet.normal);
    return solveRotation(vSun0, vSun);
}

QVector<Angles> TrackerDualSolver::solveReflectionGlobal(const Vector3D& vSun, const Vector3D& rAim)
{
    QVector<Angles> ans;
    int iMax = 5; // max iterations
    double deltaMin = 0.01; // accuracy in meters

    for (int s = 0; s < 2; ++s) // solutions
    {
        Vector3D rFacet = findFacetPoint(angles0);
        for (int i = 0; i < iMax; ++i)
        {
            Vector3D vTarget = (rAim - rFacet).normalized();
            Vector3D normal = (vSun + vTarget).normalized();
            QVector<Angles> temp = solveFacetNormal(normal);
            if (temp.empty()) break;
            Angles& angles = temp[s];
            rFacet = findFacetPoint(angles);
            double delta = cross(rAim - rFacet, vTarget).norm();
            if (delta > deltaMin) continue;
            ans << angles;
            break;
        }
    }

    return ans;
}

Angles TrackerDualSolver::selectSolution(const QVector<Angles>& solutions)
{
    Angles ans;
    double zAns = gcf::infinity;

    for (const Angles& solution : solutions)
    {
        Angles temp;
        temp.x = primary.angles.normalizeAngle(solution.x);
        if (!primary.angles.isInside(temp.x)) continue;
        temp.y = secondary.angles.normalizeAngle(solution.y);
        if (!secondary.angles.isInside(temp.y)) continue;
        double z = (temp - angles0).norm2();
        if (z > zAns) continue;
        ans = temp;
        zAns = z;
    }

    if (zAns < gcf::infinity)
        return ans;
    return angles0;
}
