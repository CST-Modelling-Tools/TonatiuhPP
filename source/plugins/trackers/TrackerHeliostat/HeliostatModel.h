#pragma once

#include "HeliostatDrive.h"


struct HeliostatModel
{
    HeliostatModel(
        const HeliostatDrive& primary,
        const HeliostatDrive& secondary,
        const Vertex3D& tracking,
        const Vector3D& angles0 = Vector3D(0., 0., 0.)
    );

    HeliostatDrive primary;
    HeliostatDrive secondary;
    Vertex3D tracking;
    Vector3D angles0;

    Vector3D findTrackingPoint(const Vector3D& angles);

    QVector<Vector3D> solveRotation(const Vector3D& v0, const Vector3D& v);

    QVector<Vector3D> solveTrackingNormal(const Vector3D& normal);
    QVector<Vector3D> solveReflectionLocal(const Vector3D& vSun, const Vector3D& rAim);
    QVector<Vector3D> solveReflectionGlobal(const Vector3D& vSun, const Vector3D& rAim);
};

