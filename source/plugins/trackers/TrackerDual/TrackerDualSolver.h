#pragma once

#include "kernel/trackers/TrackingDrive.h"
#include "libraries/math/2D/vec2d.h"

typedef vec2d Angles;


struct TrackerDualSolver
{
    TrackerDualSolver(
        const TrackingDrive& primary,
        const TrackingDrive& secondary,
        const TrackingVertex& facet,
        const Angles& angles0 = Angles(0., 0.)
    );

    TrackingDrive primary;
    TrackingDrive secondary;
    TrackingVertex facet;
    Angles angles0; // default angles

    vec3d findFacetPoint(const Angles& angles);

    QVector<Angles> solveRotation(const vec3d& v0, const vec3d& v);

    QVector<Angles> solveFacetNormal(const vec3d& normal);
    QVector<Angles> solveReflectionSecondary(const vec3d& vSun, const vec3d& rAim);
    QVector<Angles> solveReflectionGlobal(const vec3d& vSun, const vec3d& rAim);

    Angles selectSolution(const QVector<Angles>& solutions);
};
