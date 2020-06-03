#pragma once

#include "libraries/geometry/Vector3D.h"
class Ray;


struct TONATIUH_LIBRARIES Box3D
{
    Box3D();
    explicit Box3D(const Vector3D& p);
    Box3D(const Vector3D& pA, const Vector3D& pB);

    Vector3D extent() const {return pMax - pMin;}
    int maxDimension() const;
    double volume() const;
    Vector3D absMax() const {return max(pMin.abs(), pMax.abs());}

    void expand(double delta);
    void expand(const Vector3D& p);
    void expand(const Box3D& b);

    void operator<<(const Vector3D& p) {expand(p);}
    void operator<<(const Box3D& b) {expand(b);}

    bool intersect(const Vector3D& p) const;
    bool intersect(const Box3D& b) const;
    bool intersect(const Ray& ray, double* hitt0 = 0, double* hitt1 = 0) const;

    Vector3D pMin;
    Vector3D pMax;

    static const Box3D UnitCube;
};


TONATIUH_LIBRARIES Box3D Union(const Box3D& b, const Vector3D& p);
TONATIUH_LIBRARIES Box3D Union(const Box3D& bA, const Box3D& bB);

TONATIUH_LIBRARIES std::ostream& operator<<(std::ostream& os, const Box3D& b);
