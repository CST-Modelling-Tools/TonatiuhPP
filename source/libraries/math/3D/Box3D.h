#pragma once

#include "libraries/math/3D/vec3d.h"
class Ray;

// like Interval3D
struct TONATIUH_LIBRARIES Box3D
{
    Box3D();
    explicit Box3D(const vec3d& p);
    Box3D(const vec3d& pA, const vec3d& pB);

    vec3d extent() const {return pMax - pMin;}
    int maxDimension() const;
    double volume() const;
    vec3d absMin() const;
    vec3d absMax() const {return max(pMin.abs(), pMax.abs());}

    void expand(double delta);
    void expand(const vec3d& p);
    void expand(const Box3D& b);

    void operator<<(const vec3d& p) {expand(p);}
    void operator<<(const Box3D& b) {expand(b);}

    bool intersect(const vec3d& p) const;
    bool intersect(const Box3D& b) const;
    bool intersect(const Ray& ray, double* hitt0 = 0, double* hitt1 = 0) const;

    vec3d pMin;
    vec3d pMax;

    static const Box3D UnitCube;
};


TONATIUH_LIBRARIES Box3D Union(const Box3D& b, const vec3d& p);
TONATIUH_LIBRARIES Box3D Union(const Box3D& bA, const Box3D& bB);

TONATIUH_LIBRARIES std::ostream& operator<<(std::ostream& os, const Box3D& b);
