#include "BoundingBox.h"

#include <cmath>
#include "Ray.h"
#include "gc.h"
#include "Vector3D.h"


BoundingBox::BoundingBox():
    pMin(gc::Infinity, gc::Infinity, gc::Infinity),
    pMax(-gc::Infinity, -gc::Infinity, -gc::Infinity)
{
}

BoundingBox::BoundingBox(const Vector3D& p):
    pMin(p), pMax(p)
{
}

BoundingBox::BoundingBox(const Vector3D& a, const Vector3D& b)
{
    pMin = min(a, b);
    pMax = max(a, b);
}

bool BoundingBox::isInside(const Vector3D& p) const
{
    return p.x >= pMin.x && p.x <= pMax.x &&
           p.y >= pMin.y && p.y <= pMax.y &&
           p.z >= pMin.z && p.z <= pMax.z;
}

bool BoundingBox::Overlaps(const BoundingBox& b) const
{
    return (pMax.x >= b.pMin.x) && (pMin.x <= b.pMax.x) &&
           (pMax.y >= b.pMin.y) && (pMin.y <= b.pMax.y) &&
           (pMax.z >= b.pMin.z) && (pMin.z <= b.pMax.z);
}

void BoundingBox::expand(double delta)
{
    pMin.x -= delta;
    pMin.y -= delta;
    pMin.z -= delta;

    pMax.x += delta;
    pMax.y += delta;
    pMax.z += delta;
}

void BoundingBox::expand(const Vector3D& p)
{
    pMin = min(pMin, p);
    pMax = max(pMax, p);
}

void BoundingBox::expand(const BoundingBox& b)
{
    pMin = min(pMin, b.pMin);
    pMax = max(pMax, b.pMax);
}

double BoundingBox::Volume() const
{
    Vector3D d = pMax - pMin;
    return d.x*d.y*d.z;
}

int BoundingBox::MaximumExtent() const
{
    Vector3D d = pMax - pMin;
    if (d.x > d.y && d.x > d.z)
        return 0;
    else if (d.y > d.z)
        return 1;
    return 2;
}

void BoundingBox::BoundingSphere(Vector3D& center, double& radius) const
{
    center = 0.5*(pMin + pMax);
    radius = (pMax - center).norm();
}

bool BoundingBox::IntersectP(const Ray& ray, double* hitt0, double* hitt1) const
{
    double t0 = ray.tMin;
    double t1 = ray.tMax;
    double tmin, tmax, tymin, tymax, tzmin, tzmax;

    double invDirection = ray.invDirection().x;
    if (invDirection >= 0.)
    {
        tmin = (pMin.x - ray.origin.x)*invDirection;
        tmax = (pMax.x - ray.origin.x)*invDirection;
    }
    else
    {
        tmin = (pMax.x - ray.origin.x)*invDirection;
        tmax = (pMin.x - ray.origin.x)*invDirection;
    }

    invDirection = ray.invDirection().y;
    if (invDirection >= 0.)
    {
        tymin = (pMin.y - ray.origin.y)*invDirection;
        tymax = (pMax.y - ray.origin.y)*invDirection;
    }
    else
    {
        tymin = (pMax.y - ray.origin.y)*invDirection;
        tymax = (pMin.y - ray.origin.y)*invDirection;
    }

    if (tmin > tymax || tymin > tmax) return false;

    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    invDirection = ray.invDirection().z;
    if (invDirection >= 0.)
    {
        tzmin = (pMin.z - ray.origin.z)*invDirection;
        tzmax = (pMax.z - ray.origin.z)*invDirection;
    }
    else
    {
        tzmin = (pMax.z - ray.origin.z)*invDirection;
        tzmax = (pMin.z - ray.origin.z)*invDirection;
    }

    if (tmin > tzmax || tzmin > tmax) return false;

    if (tzmin > tmin) tmin = tzmin;
    if (tzmax < tmax) tmax = tzmax;
    if (tmin < t1 && tmax > t0)
    {
        if (tmin < t0) tmin = t0;
        if (tmax > t1) tmax = t1;
        if (hitt0) *hitt0 = tmin;
        if (hitt1) *hitt1 = tmax;
        return true;
    }
    else return false;
}

BoundingBox Union(const BoundingBox& b, const Vector3D& p)
{
    BoundingBox ans;
    ans.pMin = min(b.pMin, p);
    ans.pMax = max(b.pMax, p);
    return ans;
}

BoundingBox Union(const BoundingBox& a, const BoundingBox& b)
{
    BoundingBox ans;
    ans.pMin = min(a.pMin, b.pMin);
    ans.pMax = max(a.pMax, b.pMax);
    return ans;
}

std::ostream& operator<<(std::ostream& os, const BoundingBox& bbox)
{
    os << "pMin: " << bbox.pMin << std::endl;
    os << "pMax: " << bbox.pMax << std::endl;
    return os;
}
