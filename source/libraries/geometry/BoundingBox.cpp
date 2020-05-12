#include "BoundingBox.h"

#include <cmath>
#include "Ray.h"
#include "gc.h"
#include "Vector3D.h"
#include "Point3D.h"


BoundingBox::BoundingBox():
    pMin(gc::Infinity, gc::Infinity, gc::Infinity),
    pMax(-gc::Infinity, -gc::Infinity, -gc::Infinity)
{
}

BoundingBox::BoundingBox(const Point3D& point):
    pMin(point), pMax(point)
{
}

BoundingBox::BoundingBox(const Point3D& a, const Point3D& b)
{
    pMin = min(a, b);
    pMax = max(a, b);
}

bool BoundingBox::Overlaps(const BoundingBox& bbox) const
{
    return (pMax.x >= bbox.pMin.x) && (pMin.x <= bbox.pMax.x) &&
           (pMax.y >= bbox.pMin.y) && (pMin.y <= bbox.pMax.y) &&
           (pMax.z >= bbox.pMin.z) && (pMin.z <= bbox.pMax.z);
}

bool BoundingBox::Inside(const Point3D& point) const
{
    return (point.x >= pMin.x && point.x <= pMax.x &&
            point.y >= pMin.y && point.y <= pMax.y &&
            point.z >= pMin.z && point.z <= pMax.z);
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

void BoundingBox::expand(const BoundingBox& b)
{
    pMin = min(pMin, b.pMin);
    pMax = max(pMax, b.pMax);
}

double BoundingBox::Volume() const
{
    Vector3D diagonal = pMax - pMin;
    return diagonal.x * diagonal.y * diagonal.z;
}

int BoundingBox::MaximumExtent() const
{
    Vector3D diagonal = pMax - pMin;
    if (diagonal.x > diagonal.y && diagonal.x > diagonal.z)
        return 0;
    else if (diagonal.y > diagonal.z)
        return 1;
    return 2;
}

void BoundingBox::BoundingSphere(Point3D& center, double& radius) const
{
    center = Point3D( (pMin.x + pMax.x) * 0.5, (pMin.y + pMax.y) * 0.5, (pMin.z + pMax.z) * 0.5);
    radius = Distance(center, pMax);
}

bool BoundingBox::IntersectP(const Ray& ray, double* hitt0, double* hitt1) const
{
    double t0 = ray.tMin;
    double t1 = ray.tMax;
    double tmin, tmax, tymin, tymax, tzmin, tzmax;

    double invDirection = ray.invDirection().x;
    if (invDirection >= 0.)
    {
        tmin = (pMin.x - ray.origin.x) * invDirection;
        tmax = (pMax.x - ray.origin.x) * invDirection;
    }
    else
    {
        tmin = (pMax.x - ray.origin.x) * invDirection;
        tmax = (pMin.x - ray.origin.x) * invDirection;
    }

    invDirection = ray.invDirection().y;
    if (invDirection >= 0.)
    {
        tymin = (pMin.y - ray.origin.y) * invDirection;
        tymax = (pMax.y - ray.origin.y) * invDirection;
    }
    else
    {
        tymin = (pMax.y - ray.origin.y) * invDirection;
        tymax = (pMin.y - ray.origin.y) * invDirection;
    }

    if (tmin > tymax || tymin > tmax) return false;

    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    invDirection = ray.invDirection().z;
    if (invDirection >= 0.)
    {
        tzmin = (pMin.z - ray.origin.z) * invDirection;
        tzmax = (pMax.z - ray.origin.z) * invDirection;
    }
    else
    {
        tzmin = (pMax.z - ray.origin.z) * invDirection;
        tzmax = (pMin.z - ray.origin.z) * invDirection;
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

BoundingBox Union(const BoundingBox& bbox, const Point3D& point)
{
    BoundingBox unionBox;
    unionBox.pMin.x = (bbox.pMin.x < point.x) ? bbox.pMin.x : point.x;
    unionBox.pMin.y = (bbox.pMin.y < point.y) ? bbox.pMin.y : point.y;
    unionBox.pMin.z = (bbox.pMin.z < point.z) ? bbox.pMin.z : point.z;
    unionBox.pMax.x = (bbox.pMax.x > point.x) ? bbox.pMax.x : point.x;
    unionBox.pMax.y = (bbox.pMax.y > point.y) ? bbox.pMax.y : point.y;
    unionBox.pMax.z = (bbox.pMax.z > point.z) ? bbox.pMax.z : point.z;
    return unionBox;
}

BoundingBox Union(const BoundingBox& bbox1, const BoundingBox& bbox2)
{
    BoundingBox unionBox;
    unionBox.pMin.x = (bbox1.pMin.x < bbox2.pMin.x) ? bbox1.pMin.x : bbox2.pMin.x;
    unionBox.pMin.y = (bbox1.pMin.y < bbox2.pMin.y) ? bbox1.pMin.y : bbox2.pMin.y;
    unionBox.pMin.z = (bbox1.pMin.z < bbox2.pMin.z) ? bbox1.pMin.z : bbox2.pMin.z;
    unionBox.pMax.x = (bbox1.pMax.x > bbox2.pMax.x) ? bbox1.pMax.x : bbox2.pMax.x;
    unionBox.pMax.y = (bbox1.pMax.y > bbox2.pMax.y) ? bbox1.pMax.y : bbox2.pMax.y;
    unionBox.pMax.z = (bbox1.pMax.z > bbox2.pMax.z) ? bbox1.pMax.z : bbox2.pMax.z;
    return unionBox;
}

std::ostream& operator<<(std::ostream& os, const BoundingBox& bbox)
{
    os << "pMin: " << bbox.pMin << std::endl;
    os << "pMax: " << bbox.pMax << std::endl;
    return os;
}
