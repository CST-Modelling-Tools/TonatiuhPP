#include "TrackerDualSolver.h"

#include "libraries/math/gcf.h"

#include <QVector>

// rotation around a from m to v
inline double findAngle(const vec3d& a, const vec3d& m, const vec3d& v, double av)
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

vec3d TrackerDualSolver::findFacetPoint(const Angles& angles)
{
    vec3d r = secondary.getTransform(angles.y).transformPoint(facet.shift);
    return primary.getTransform(angles.x).transformPoint(r);
}

// rotate v0 to v
QVector<Angles> TrackerDualSolver::solveRotation(const vec3d& v0, const vec3d& v)
{
    const vec3d& a = primary.axis;
    const vec3d& b = secondary.axis;

    vec3d k = cross(a, b);
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
    vec3d m0 = ma*a + mb*b;
    QVector<Angles> ans;
    vec3d m = m0 - mk*k;
    ans << Angles(findAngle(a, m, v, av), findAngle(b, v0, m, bv0));
    m = m0 + mk*k;
    ans << Angles(findAngle(a, m, v, av), findAngle(b, v0, m, bv0));
    return ans;
}

// rotate facet.normal to normal
QVector<Angles> TrackerDualSolver::solveFacetNormal(const vec3d& normal)
{
    return solveRotation(facet.normal, normal);
}

QVector<Angles> TrackerDualSolver::solveReflectionSecondary(const vec3d& vSun, const vec3d& rAim)
{
    vec3d vTarget0 = (rAim - facet.shift).normalized();
    vec3d vSun0 = -vTarget0.reflected(facet.normal);
    return solveRotation(vSun0, vSun);
}

QVector<Angles> TrackerDualSolver::solveReflectionGlobal(const vec3d& vSun, const vec3d& rAim)
{
    QVector<Angles> ans;
    int iMax = 5; // max iterations
    double deltaMin = 0.01; // accuracy in meters

    for (int s = 0; s < 2; ++s) // solutions
    {
        vec3d rFacet = findFacetPoint(angles0);
        for (int i = 0; i < iMax; ++i)
        {
            vec3d vTarget = (rAim - rFacet).normalized();
            vec3d normal = (vSun + vTarget).normalized();
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
