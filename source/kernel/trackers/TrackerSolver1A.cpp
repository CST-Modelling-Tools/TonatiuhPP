#include "TrackerSolver1A.h"

#include "libraries/math/gcf.h"


// rotation around a from m to v
inline double findAngle(const vec3d& a, const vec3d& m, const vec3d& v)
{
    return atan2(dot(a, cross(m, v)), dot(m, v));
}

TrackerSolver1A::TrackerSolver1A(TrackerArmature1A* armature)
{
    m_armature = armature;
}

vec3d TrackerSolver1A::findFacetPoint(double angle)
{
    return m_armature->primary.getTransform(angle).transformPoint(m_armature->facet.shift);
}

double TrackerSolver1A::solveReflectionPrimary(const vec3d& vSun, const vec3d& rAim)
{
    const vec3d& a = m_armature->primary.axis;
    const vec3d& n = m_armature->facet.normal;

    vec3d t = rAim - m_armature->facet.shift;
    t -= dot(t, a)*a;
    t.normalize();
    vec3d v0 = -t.reflected(n);

    vec3d v = vSun - dot(vSun, a)*a;
    v.normalize();

    return findAngle(a, v0, v);
}

double TrackerSolver1A::solveReflectionGlobal(const vec3d& vSun, const vec3d& rAim)
{
    const vec3d& a = m_armature->primary.axis;
    const vec3d& n0 = m_armature->facet.normal;

    vec3d v = vSun - dot(vSun, a)*a;
    v.normalize();

    int iMax = 5; // max iterations
    double deltaMin = 0.001; // accuracy in meters

    double ans;
    vec3d rFacet = findFacetPoint(m_armature->angle0);
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

double TrackerSolver1A::selectSolution(double solution)
{
    double temp = m_armature->primary.angles.normalizeAngle(solution);
    if (m_armature->primary.angles.isInside(temp)) return temp;
    return m_armature->angle0;
}

