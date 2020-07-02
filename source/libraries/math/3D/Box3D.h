#pragma once

#include "libraries/math/3D/vec3d.h"
class Ray;


struct TONATIUH_LIBRARIES Box3D
{
    Box3D();
    explicit Box3D(const vec3d& p);
    Box3D(const vec3d& pA, const vec3d& pB);

    const vec3d& min() const {return m_a;}
    const vec3d& max() const {return m_b;}
    vec3d size() const {return m_b - m_a;}
    vec3d center() const {return (m_a + m_b)/2.;}
    double volume() const;

    void addMargin(double delta);
    void expand(const vec3d& p);
    void expand(const Box3D& b);
    void operator<<(const vec3d& p) {expand(p);}
    void operator<<(const Box3D& b) {expand(b);}

    bool intersect(const vec3d& p) const {return m_a <= p && p <= m_b;}
    bool intersect(const Box3D& b) const;
    bool intersect(const Ray& ray, double* t0 = 0, double* t1 = 0) const;

    static const Box3D UnitPositive;
    static const Box3D UnitCentered;

protected:
    vec3d m_a;
    vec3d m_b;
};

TONATIUH_LIBRARIES std::ostream& operator<<(std::ostream& os, const Box3D& b);
