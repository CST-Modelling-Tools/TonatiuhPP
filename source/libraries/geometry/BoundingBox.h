#pragma once

#include "libraries/geometry/Vector3D.h"
class Ray;


struct TONATIUH_LIBRARIES BoundingBox
{
    BoundingBox();
    explicit BoundingBox(const Vector3D& p);
    BoundingBox(const Vector3D& pA, const Vector3D& pB);

    int maxDimension() const;
    double volume() const;

    void expand(double delta);
    void expand(const Vector3D& p);
    void expand(const BoundingBox& b);

    void operator<<(const Vector3D& p) {expand(p);}
    void operator<<(const BoundingBox& b) {expand(b);}

    bool intersect(const Vector3D& p) const;
    bool intersect(const BoundingBox& b) const;
    bool intersect(const Ray& ray, double* hitt0 = 0, double* hitt1 = 0) const;

    Vector3D pMin;
    Vector3D pMax;
};


TONATIUH_LIBRARIES BoundingBox Union(const BoundingBox& b, const Vector3D& p);
TONATIUH_LIBRARIES BoundingBox Union(const BoundingBox& bA, const BoundingBox& bB);

TONATIUH_LIBRARIES std::ostream& operator<<(std::ostream& os, const BoundingBox& b);
