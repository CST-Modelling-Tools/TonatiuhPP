#pragma once

#include "SunPathLib/calculators/SunCalculator.h"
#include "SunPathLib/math/geometry/Interval.h"

namespace sp {


class SunSpatial;


struct SUNPATHLIB SkyNode
{
    SkyNode() {}
    SkyNode(vec3d v, double sigma):
        v(v), sigma(sigma), coeff(1./pow2(sigma)) {}

    vec3d v; // vector to sun
    double sigma;
    double coeff; // coefficient for kernel

    double kernel(const vec3d& r) const; // kernel function
};


class SUNPATHLIB SkySampler
{
public:
    SkySampler(SunSpatial* sunSpatial);

    bool sample(double rho, double sigma = 0., bool symEW = false);

protected:
    Interval findDeclinations();
    Interval findHours(double delta);

protected:
    SunSpatial* m_sunSpatial;
};


} // namespace sp
