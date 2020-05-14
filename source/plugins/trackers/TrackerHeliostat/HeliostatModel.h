#pragma once

#include "HeliostatDrive.h"

typedef Vector2D Angles;

struct HeliostatModel
{
    HeliostatModel(
        const HeliostatDrive& primary,
        const HeliostatDrive& secondary,
        const Vertex3D& tracking,
        const Angles& angles0 = Angles(0., 0.)
    );

    HeliostatDrive primary;
    HeliostatDrive secondary;
    Vertex3D tracking;
    Vector2D angles0;

    Vector3D findTrackingPoint(const Angles& angles);

    QVector<Angles> solveRotation(const Vector3D& v0, const Vector3D& v);

    QVector<Angles> solveTrackingNormal(const Vector3D& normal);
    QVector<Angles> solveReflectionLocal(const Vector3D& vSun, const Vector3D& rAim);
    QVector<Angles> solveReflectionGlobal(const Vector3D& vSun, const Vector3D& rAim);

    Angles selectSolution(const QVector<Angles>& solutions);
};
