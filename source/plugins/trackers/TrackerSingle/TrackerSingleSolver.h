#pragma once

#include "kernel/trackers/TrackingDrive.h"


struct TrackerSingleSolver
{
    TrackerSingleSolver(
        const TrackingDrive& primary,
        const TrackingVertex& facet,
        double angle0 = 0.
    );

    TrackingDrive primary;
    TrackingVertex facet;
    double angle0; // default angle

    vec3d findFacetPoint(double angle);

    double solveRotation(const vec3d& v0, const vec3d& v);

    double solveFacetNormal(const vec3d& normal);
    double solveReflectionPrimary(const vec3d& vSun, const vec3d& rAim);
    double solveReflectionGlobal(const vec3d& vSun, const vec3d& rAim);

    double selectSolution(double solution);
};
