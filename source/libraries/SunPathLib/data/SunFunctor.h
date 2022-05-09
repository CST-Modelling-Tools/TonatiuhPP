#pragma once

#include "SunPathLib/data/SkyModelPI.h"
#include "SunPathLib/calculators/SunCalculator.h"

namespace sp {


struct SUNPATHLIB SunFunctor
{
    virtual double operator()(const vec3d& s) const = 0;
};


struct SUNPATHLIB SunFunctorOne: SunFunctor
{
    double operator()(const vec3d& /*s*/) const {return 1.;}
};

struct SUNPATHLIB SunFunctorPanelCos: SunFunctor
{
    vec3d n;
    double operator()(const vec3d& s) const;
};

struct SUNPATHLIB SunFunctorHeliostatCos: SunFunctor
{
    vec3d t;
    double operator()(const vec3d& s) const;
};

struct SUNPATHLIB SunFunctorHeliostatFieldOld: SunFunctor
{
    vec3d t;
    double operator()(const vec3d& s) const;
};

struct SUNPATHLIB SunFunctorHeliostatField: SunFunctor
{
    vec3d t;
    double operator()(const vec3d& s) const;
};

struct SUNPATHLIB SunFunctorDNI: SunFunctor
{
    SkyModelPI model;
    double operator()(const vec3d& s) const;
};

struct SUNPATHLIB SunFunctorWeighted: SunFunctor
{
    SunFunctor* sw;
    SunFunctor* sf;
    double operator()(const vec3d& s) const;
};


} // namespace sp
