#include "TrackerSingleSolver.h"

#include "libraries/math/gcf.h"


// rotation around a from m to v
inline double findAngle(const vec3d& a, const vec3d& m, const vec3d& v)
{
    return atan2(dot(a, cross(m, v)), dot(m, v));
}

TrackerSingleSolver::TrackerSingleSolver(
    const TrackingDrive& primary,
    const TrackingVertex& facet,
    double angle0
):
    primary(primary),
    facet(facet),
    angle0(angle0)
{
    vec3d& a = this->primary.axis;
    vec3d& n = this->facet.normal;
    n-= dot(n, a)*a;
}

vec3d TrackerSingleSolver::findFacetPoint(double angle)
{
    return primary.getTransform(angle).transformPoint(facet.shift);
}

double TrackerSingleSolver::solveReflectionPrimary(const vec3d& vSun, const vec3d& rAim)
{
    const vec3d& a = primary.axis;
    const vec3d& n = facet.normal;

    vec3d t = rAim - facet.shift;
    t -= dot(t, a)*a;
    t.normalize();
    vec3d v0 = -t.reflected(n);

    vec3d v = vSun - dot(vSun, a)*a;
    v.normalize();

    return findAngle(a, v0, v);
}

double TrackerSingleSolver::solveReflectionGlobal(const vec3d& vSun, const vec3d& rAim)
{
    const vec3d& a = primary.axis;
    const vec3d& n0 = facet.normal;

    vec3d v = vSun - dot(vSun, a)*a;
    v.normalize();

    int iMax = 5; // max iterations
    double deltaMin = 0.001; // accuracy in meters

    double ans;
    vec3d rFacet = findFacetPoint(angle0);
    for (int i = 0; i < iMax; ++i)
    {
        vec3d t = rAim - rFacet;
        t -= dot(t, a)*a;
        t.normalize();
        vec3d n = (v + t).normalized();
        ans = findAngle(a, n0, n);
        rFacet = findFacetPoint(ans);
        double delta = dot(cross(t, a), rAim - rFacet);
        if (std::abs(delta) > deltaMin) continue;
    }
    return ans;
}

double TrackerSingleSolver::selectSolution(double solution)
{
    double temp = primary.angles.normalizeAngle(solution);
    if (primary.angles.isInside(temp)) return temp;
    return angle0;
}
