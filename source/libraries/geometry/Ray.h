#pragma once

#include "gc.h"
#include "Point3D.h"
#include "Vector3D.h"

class Ray
{
public:
    Ray() : tMin(gc::Epsilon), tMax(gc::Infinity)
    {
    }

    Ray(const Point3D& orig, const Vector3D& direc, double start = gc::Epsilon, double end = gc::Infinity)
        : origin(orig), tMin(start), tMax(end)
    {
        setDirection(direc);
    }

    Point3D operator()(double t) const
    {
        return origin + m_direction * t;
    }

    const Vector3D& direction() const
    {
        return m_direction;
    }

    const Vector3D& invDirection() const
    {
        return m_directionInv;
    }

    void setDirection(const Vector3D& direction)
    {
        m_direction = direction;
        m_directionInv.x = 1./m_direction.x;
        m_directionInv.y = 1./m_direction.y;
        m_directionInv.z = 1./m_direction.z;
    }

    bool operator==(const Ray& ray) const
    {
        if (this == &ray) return true;
        return ( (origin == ray.origin) && (m_direction == ray.m_direction) &&
                 !(fabs(tMin - ray.tMin) > gc::Epsilon) && !(fabs(tMax - ray.tMax) > gc::Epsilon) );
    }

    Point3D origin;
    mutable double tMin;
    mutable double tMax;

private:
    Vector3D m_direction;
    Vector3D m_directionInv;
};


