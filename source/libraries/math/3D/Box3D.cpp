#include "Box3D.h"

#include <cmath>
#include "Ray.h"
#include "math/gcf.h"
#include "vec3d.h"

const Box3D Box3D::UnitPositive(
    vec3d::Zero,
    vec3d::One
);

const Box3D Box3D::UnitCentered(
    -vec3d::One/2,
    vec3d::One/2
);


Box3D::Box3D():
    m_a(gcf::infinity, gcf::infinity, gcf::infinity),
    m_b(-gcf::infinity, -gcf::infinity, -gcf::infinity)
{
}

Box3D::Box3D(const vec3d& p):
    m_a(p), m_b(p)
{
}

Box3D::Box3D(const vec3d& pA, const vec3d& pB)
{
    m_a = vec3d::min(pA, pB);
    m_b = vec3d::max(pA, pB);
}

double Box3D::volume() const
{
    vec3d d = m_b - m_a;
    return d.x*d.y*d.z;
}

void Box3D::addMargin(double delta)
{
    vec3d v(delta, delta, delta);
    m_a -= v;
    m_b += v;
}

void Box3D::expand(const vec3d& p)
{
    m_a = vec3d::min(m_a, p);
    m_b = vec3d::max(m_b, p);
}

void Box3D::expand(const Box3D& b)
{
    m_a = vec3d::min(m_a, b.m_a);
    m_b = vec3d::max(m_b, b.m_b);
}

bool Box3D::intersect(const Box3D& b) const
{
    return m_a <= b.m_b && b.m_a <= m_b;
}

bool Box3D::intersect(const Ray& ray, double* t0, double* t1) const
{
    const vec3d& rayO = ray.origin;
    const vec3d& rayI = ray.invDirection();
    double trMin = ray.tMin;
    double trMax = ray.tMax;
    double tMin, tMax, tyMin, tyMax, tzMin, tzMax;

    if (rayI.x >= 0.)
    {
        tMin = (m_a.x - rayO.x)*rayI.x;
        tMax = (m_b.x - rayO.x)*rayI.x;
    }
    else
    {
        tMin = (m_b.x - rayO.x)*rayI.x;
        tMax = (m_a.x - rayO.x)*rayI.x;
    }

    if (rayI.y >= 0.)
    {
        tyMin = (m_a.y - rayO.y)*rayI.y;
        tyMax = (m_b.y - rayO.y)*rayI.y;
    }
    else
    {
        tyMin = (m_b.y - rayO.y)*rayI.y;
        tyMax = (m_a.y - rayO.y)*rayI.y;
    }
    if (tyMin > tMax || tyMax < tMin) return false;
    if (tyMin > tMin) tMin = tyMin;
    if (tyMax < tMax) tMax = tyMax;

    if (rayI.z >= 0.)
    {
        tzMin = (m_a.z - rayO.z)*rayI.z;
        tzMax = (m_b.z - rayO.z)*rayI.z;
    }
    else
    {
        tzMin = (m_b.z - rayO.z)*rayI.z;
        tzMax = (m_a.z - rayO.z)*rayI.z;
    }
    if (tzMin > tMax || tzMax < tMin) return false;
    if (tzMin > tMin) tMin = tzMin;
    if (tzMax < tMax) tMax = tzMax;

    if (tMin > trMax || tMax < trMin) return false;
    if (tMin < trMin) tMin = trMin;
    if (tMax > trMax) tMax = trMax;

    if (t0) *t0 = tMin;
    if (t1) *t1 = tMax;
    return true;
}

std::ostream& operator<<(std::ostream& os, const Box3D& b)
{
    os << "pMin: " << b.min() << std::endl;
    os << "pMax: " << b.max() << std::endl;
    return os;
}
