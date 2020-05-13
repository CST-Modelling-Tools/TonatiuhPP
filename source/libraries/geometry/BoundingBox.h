#pragma once

#include "Point3D.h"

class Ray;

struct TONATIUH_LIBRARIES BoundingBox
{
    BoundingBox();
    explicit BoundingBox(const Point3D& point);
    BoundingBox(const Point3D& a, const Point3D& b);

    bool isInside(const Point3D& p) const;
    bool Overlaps(const BoundingBox& b) const;

    void expand(double delta);
    void expand(const Point3D& p);
    void expand(const BoundingBox& b);
    void operator<<(const Point3D& p) {expand(p);}
    void operator<<(const BoundingBox& b) {expand(b);}

    double Volume() const;
    int MaximumExtent() const;
    void BoundingSphere(Point3D& center, double& radius) const;
    bool IntersectP(const Ray& ray, double* hitt0 = 0, double* hitt1 = 0) const;

    Point3D pMin;
    Point3D pMax;
};

TONATIUH_LIBRARIES BoundingBox Union(const BoundingBox& b, const Point3D& p);
TONATIUH_LIBRARIES BoundingBox Union(const BoundingBox& a, const BoundingBox& b);
TONATIUH_LIBRARIES std::ostream& operator<<(std::ostream& os, const BoundingBox& bbox);
