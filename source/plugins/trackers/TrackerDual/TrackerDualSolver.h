#pragma once

#include "kernel/trackers/TrackingDrive.h"
#include "libraries/math/Vector2D.h"

typedef Vector2D Angles;


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

    Vector3D findFacetPoint(const Angles& angles);

    QVector<Angles> solveRotation(const Vector3D& v0, const Vector3D& v);

    QVector<Angles> solveFacetNormal(const Vector3D& normal);
    QVector<Angles> solveReflectionSecondary(const Vector3D& vSun, const Vector3D& rAim);
    QVector<Angles> solveReflectionGlobal(const Vector3D& vSun, const Vector3D& rAim);

    Angles selectSolution(const QVector<Angles>& solutions);
};
