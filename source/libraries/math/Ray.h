#pragma once

#include "libraries/math/gcf.h"
#include "libraries/math/Vector3D.h"


class TONATIUH_LIBRARIES Ray
{
public:
    Ray():
        tMin(gcf::Epsilon), tMax(gcf::infinity)
    {
    }

    Ray(const Vector3D& orig, const Vector3D& direc, double start = gcf::Epsilon, double end = gcf::infinity):
        origin(orig), tMin(start), tMax(end)
    {
        setDirection(direc);
    }

    Vector3D point(double t) const {return origin + m_direction*t;}

    const Vector3D& direction() const {return m_direction;}

    const Vector3D& invDirection() const {return m_directionInv;}

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
        return origin == ray.origin && m_direction == ray.m_direction &&
               fabs(tMin - ray.tMin) < gcf::Epsilon && fabs(tMax - ray.tMax) < gcf::Epsilon;
    }

    Vector3D origin;
    mutable double tMin;
    mutable double tMax;

private:
    Vector3D m_direction;
    Vector3D m_directionInv;
};
