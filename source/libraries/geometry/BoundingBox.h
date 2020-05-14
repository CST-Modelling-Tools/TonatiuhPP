#pragma once

#include "Vector3D.h"

class Ray;

struct TONATIUH_LIBRARIES BoundingBox
{
    BoundingBox();
    explicit BoundingBox(const Vector3D& p);
    BoundingBox(const Vector3D& a, const Vector3D& b);

    bool isInside(const Vector3D& p) const;
    bool Overlaps(const BoundingBox& b) const;

    void expand(double delta);
    void expand(const Vector3D& p);
    void expand(const BoundingBox& b);
    void operator<<(const Vector3D& p) {expand(p);}
    void operator<<(const BoundingBox& b) {expand(b);}

    double Volume() const;
    int MaximumExtent() const;
    void BoundingSphere(Vector3D& center, double& radius) const;
    bool IntersectP(const Ray& ray, double* hitt0 = 0, double* hitt1 = 0) const;

    Vector3D pMin;
    Vector3D pMax;
};

TONATIUH_LIBRARIES BoundingBox Union(const BoundingBox& b, const Vector3D& p);
TONATIUH_LIBRARIES BoundingBox Union(const BoundingBox& a, const BoundingBox& b);
TONATIUH_LIBRARIES std::ostream& operator<<(std::ostream& os, const BoundingBox& bbox);
