#include "Box3D.h"

#include <cmath>
#include "Ray.h"
#include "gcf.h"
#include "Vector3D.h"

const Box3D Box3D::UnitCube(
    -Vector3D(0.5, 0.5, 0.5),
    Vector3D(0.5, 0.5, 0.5)
);

Box3D::Box3D():
    pMin(gcf::infinity, gcf::infinity, gcf::infinity),
    pMax(-gcf::infinity, -gcf::infinity, -gcf::infinity)
{
}

Box3D::Box3D(const Vector3D& p):
    pMin(p), pMax(p)
{
}

Box3D::Box3D(const Vector3D& pA, const Vector3D& pB)
{
    pMin = min(pA, pB);
    pMax = max(pA, pB);
}



int Box3D::maxDimension() const
{
    Vector3D d = pMax - pMin;
    if (d.x > d.y && d.x > d.z)
        return 0;
    else if (d.y > d.z)
        return 1;
    return 2;
}

double Box3D::volume() const
{
    Vector3D d = pMax - pMin;
    return d.x*d.y*d.z;
}

Vector3D Box3D::absMin() const
{
    Vector3D ans = min(pMin.abs(), pMax.abs());
    if ((pMin.x > 0.) != (pMax.x > 0.)) ans.x = 0.;
    if ((pMin.y > 0.) != (pMax.y > 0.)) ans.y = 0.;
    if ((pMin.z > 0.) != (pMax.z > 0.)) ans.z = 0.;
    return ans;
}

void Box3D::expand(double delta)
{
    Vector3D v(delta, delta, delta);
    pMin -= v;
    pMax += v;
}

void Box3D::expand(const Vector3D& p)
{
    pMin = min(pMin, p);
    pMax = max(pMax, p);
}

void Box3D::expand(const Box3D& b)
{
    pMin = min(pMin, b.pMin);
    pMax = max(pMax, b.pMax);
}

bool Box3D::intersect(const Vector3D& p) const
{
    return pMin.x <= p.x && p.x <= pMax.x &&
           pMin.y <= p.y && p.y <= pMax.y &&
           pMin.z <= p.z && p.z <= pMax.z;
}

bool Box3D::intersect(const Box3D& b) const
{
    return pMin.x <= b.pMax.x && b.pMin.x <= pMax.x &&
           pMin.y <= b.pMax.y && b.pMin.y <= pMax.y &&
           pMin.z <= b.pMax.z && b.pMin.z <= pMax.z;
}

bool Box3D::intersect(const Ray& ray, double* t0, double* t1) const
{
    const Vector3D& rayO = ray.origin;
    const Vector3D& rayI = ray.invDirection();
    double trMin = ray.tMin;
    double trMax = ray.tMax;
    double tMin, tMax, tyMin, tyMax, tzMin, tzMax;

    if (rayI.x >= 0.)
    {
        tMin = (pMin.x - rayO.x)*rayI.x;
        tMax = (pMax.x - rayO.x)*rayI.x;
    }
    else
    {
        tMin = (pMax.x - rayO.x)*rayI.x;
        tMax = (pMin.x - rayO.x)*rayI.x;
    }

    if (rayI.y >= 0.)
    {
        tyMin = (pMin.y - rayO.y)*rayI.y;
        tyMax = (pMax.y - rayO.y)*rayI.y;
    }
    else
    {
        tyMin = (pMax.y - rayO.y)*rayI.y;
        tyMax = (pMin.y - rayO.y)*rayI.y;
    }
    if (tyMin > tMax || tyMax < tMin) return false;
    if (tyMin > tMin) tMin = tyMin;
    if (tyMax < tMax) tMax = tyMax;

    if (rayI.z >= 0.)
    {
        tzMin = (pMin.z - rayO.z)*rayI.z;
        tzMax = (pMax.z - rayO.z)*rayI.z;
    }
    else
    {
        tzMin = (pMax.z - rayO.z)*rayI.z;
        tzMax = (pMin.z - rayO.z)*rayI.z;
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

Box3D Union(const Box3D& b, const Vector3D& p)
{
    return Box3D(
        min(b.pMin, p),
        max(b.pMax, p)
    );
}

Box3D Union(const Box3D& bA, const Box3D& bB)
{
    return Box3D(
        min(bA.pMin, bB.pMin),
        max(bA.pMax, bB.pMax)
    );
}

std::ostream& operator<<(std::ostream& os, const Box3D& b)
{
    os << "pMin: " << b.pMin << std::endl;
    os << "pMax: " << b.pMax << std::endl;
    return os;
}
