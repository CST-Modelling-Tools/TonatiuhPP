#include "Box3D.h"

#include <cmath>
#include "math/gcf.h"
#include "Ray.h"

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

void Box3D::setLimits(const vec3d& a, const vec3d& b)
{
    m_a = vec3d::min(a, b);
    m_b = vec3d::max(a, b);
}

double Box3D::volume() const
{
    vec3d d = m_b - m_a;
    return d.x*d.y*d.z;
}

void Box3D::expandLimits(double delta)
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

bool Box3D::intersect(const Ray& ray, double* t0, double* t1) const
{
    const vec3d& rayO = ray.origin;
    const vec3d& rayI = ray.invDirection();
    double trMin = ray.tMin;
    double trMax = ray.tMax;
    double tMin, tMax, tyMin, tyMax, tzMin, tzMax;

    vec3d a = (m_a - rayO)*rayI;
    vec3d b = (m_b - rayO)*rayI;

    if (rayI.x >= 0.) {
        tMin = a.x;
        tMax = b.x;
    } else {
        tMin = b.x;
        tMax = a.x;
    }

    if (rayI.y >= 0.) {
        tyMin = a.y;
        tyMax = b.y;
    } else {
        tyMin = b.y;
        tyMax = a.y;
    }
    if (tyMin > tMax || tyMax < tMin) return false;
    if (tyMin > tMin) tMin = tyMin;
    if (tyMax < tMax) tMax = tyMax;

    if (rayI.z >= 0.) {
        tzMin = a.z;
        tzMax = b.z;
    } else {
        tzMin = b.z;
        tzMax = a.z;
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
