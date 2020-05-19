#include "BoundingBox.h"

#include <cmath>
#include "Ray.h"
#include "gcf.h"
#include "Vector3D.h"


BoundingBox::BoundingBox():
    pMin(gcf::infinity, gcf::infinity, gcf::infinity),
    pMax(-gcf::infinity, -gcf::infinity, -gcf::infinity)
{
}

BoundingBox::BoundingBox(const Vector3D& p):
    pMin(p), pMax(p)
{
}

BoundingBox::BoundingBox(const Vector3D& pA, const Vector3D& pB)
{
    pMin = min(pA, pB);
    pMax = max(pA, pB);
}

int BoundingBox::maxDimension() const
{
    Vector3D d = pMax - pMin;
    if (d.x > d.y && d.x > d.z)
        return 0;
    else if (d.y > d.z)
        return 1;
    return 2;
}

double BoundingBox::volume() const
{
    Vector3D d = pMax - pMin;
    return d.x*d.y*d.z;
}

void BoundingBox::expand(double delta)
{
    Vector3D v(delta, delta, delta);
    pMin -= v;
    pMax += v;
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

bool BoundingBox::intersect(const Vector3D& p) const
{
    return pMin.x <= p.x && p.x <= pMax.x &&
           pMin.y <= p.y && p.y <= pMax.y &&
           pMin.z <= p.z && p.z <= pMax.z;
}

bool BoundingBox::intersect(const BoundingBox& b) const
{
    return pMin.x <= b.pMax.x && b.pMin.x <= pMax.x &&
           pMin.y <= b.pMax.y && b.pMin.y <= pMax.y &&
           pMin.z <= b.pMax.z && b.pMin.z <= pMax.z;
}

bool BoundingBox::intersect(const Ray& ray, double* t0, double* t1) const
{
    double trMin = ray.tMin;
    double trMax = ray.tMax;
    double tMin, tMax, tyMin, tyMax, tzMin, tzMax;

    double dInv = ray.invDirection().x;
    if (dInv >= 0.)
    {
        tMin = (pMin.x - ray.origin.x)*dInv;
        tMax = (pMax.x - ray.origin.x)*dInv;
    }
    else
    {
        tMin = (pMax.x - ray.origin.x)*dInv;
        tMax = (pMin.x - ray.origin.x)*dInv;
    }

    dInv = ray.invDirection().y;
    if (dInv >= 0.)
    {
        tyMin = (pMin.y - ray.origin.y)*dInv;
        tyMax = (pMax.y - ray.origin.y)*dInv;
    }
    else
    {
        tyMin = (pMax.y - ray.origin.y)*dInv;
        tyMax = (pMin.y - ray.origin.y)*dInv;
    }
    if (tyMin > tMax || tyMax < tMin) return false;
    if (tyMin > tMin) tMin = tyMin;
    if (tyMax < tMax) tMax = tyMax;

    dInv = ray.invDirection().z;
    if (dInv >= 0.)
    {
        tzMin = (pMin.z - ray.origin.z)*dInv;
        tzMax = (pMax.z - ray.origin.z)*dInv;
    }
    else
    {
        tzMin = (pMax.z - ray.origin.z)*dInv;
        tzMax = (pMin.z - ray.origin.z)*dInv;
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

BoundingBox Union(const BoundingBox& b, const Vector3D& p)
{
    return BoundingBox(
        min(b.pMin, p),
        max(b.pMax, p)
    );
}

BoundingBox Union(const BoundingBox& bA, const BoundingBox& bB)
{
    return BoundingBox(
        min(bA.pMin, bB.pMin),
        max(bA.pMax, bB.pMax)
    );
}

std::ostream& operator<<(std::ostream& os, const BoundingBox& b)
{
    os << "pMin: " << b.pMin << std::endl;
    os << "pMax: " << b.pMax << std::endl;
    return os;
}
