#include "TrackerSolver2A.h"

#include "libraries/math/gcf.h"

#include <QVector>

// rotation around a from m to v
inline double findAngle(const vec3d& a, const vec3d& m, const vec3d& v, double av)
{
    return atan2(dot(a, cross(m, v)), dot(m, v) - av*av);
}


TrackerSolver2A::TrackerSolver2A(TrackerArmature2A* armature)
{
    m_armature = armature;
}

vec3d TrackerSolver2A::findFacetPoint(const Angles& angles)
{
    vec3d r = m_armature->secondary.getTransform(angles.y).transformPoint(m_armature->facet.shift);
    return m_armature->primary.getTransform(angles.x).transformPoint(r);
}

// rotate v0 to v
QVector<Angles> TrackerSolver2A::solveRotation(const vec3d& v0, const vec3d& v)
{
    const vec3d& a = m_armature->primary.axis;
    const vec3d& b = m_armature->secondary.axis;

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
QVector<Angles> TrackerSolver2A::solveFacetNormal(const vec3d& normal)
{
    return solveRotation(m_armature->facet.normal, normal);
}

QVector<Angles> TrackerSolver2A::solveReflectionSecondary(const vec3d& vSun, const vec3d& rAim)
{
    vec3d vTarget0 = (rAim - m_armature->facet.shift).normalized();
    vec3d vSun0 = -vTarget0.reflected(m_armature->facet.normal);
    return solveRotation(vSun0, vSun);
}

QVector<Angles> TrackerSolver2A::solveReflectionGlobal(const vec3d& vSun, const vec3d& rAim)
{
    QVector<Angles> ans;
    int iMax = 5; // max iterations
    double deltaMin = 0.001; // accuracy in meters

    for (int s = 0; s < 2; ++s) // solutions
    {
        vec3d rFacet = findFacetPoint(m_armature->angles0);
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

Angles TrackerSolver2A::selectSolution(const QVector<Angles>& solutions)
{
    Angles ans;
    double zAns = gcf::infinity;

    for (const Angles& solution : solutions)
    {
        Angles temp;
        temp.x = m_armature->primary.angles.normalizeAngle(solution.x);
        if (!m_armature->primary.angles.isInside(temp.x)) continue;
        temp.y = m_armature->secondary.angles.normalizeAngle(solution.y);
        if (!m_armature->secondary.angles.isInside(temp.y)) continue;
        double z = (temp - m_armature->angles0).norm2();
        if (z > zAns) continue;
        ans = temp;
        zAns = z;
    }

    if (zAns < gcf::infinity)
        return ans;
    return m_armature->angles0;
}
