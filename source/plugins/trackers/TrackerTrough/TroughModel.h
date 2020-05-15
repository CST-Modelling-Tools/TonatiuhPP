#pragma once

#include "kernel/trackers/TrackingDrive.h"


struct TroughModel
{
    TroughModel(
        const TrackingDrive& primary,
        const TrackingVertex& facet,
        double angle0 = 0.
    );

    TrackingDrive primary;
    TrackingVertex facet;
    double angle0; // default angle

    Vector3D findFacetPoint(double angle);

    double solveRotation(const Vector3D& v0, const Vector3D& v);

    double solveFacetNormal(const Vector3D& normal);
    double solveReflectionPrimary(const Vector3D& vSun, const Vector3D& rAim);
    double solveReflectionGlobal(const Vector3D& vSun, const Vector3D& rAim);

    double selectSolution(double solution);
};
