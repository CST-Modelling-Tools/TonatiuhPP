#pragma once

#include "kernel/trackers/TrackerArmature2A.h"

typedef vec2d Angles;


class TrackerSolver2A
{
public:
    TrackerSolver2A(TrackerArmature2A* armature);

    vec3d findFacetPoint(const Angles& angles);

    QVector<Angles> solveRotation(const vec3d& v0, const vec3d& v);

    QVector<Angles> solveFacetNormal(const vec3d& normal);
    QVector<Angles> solveReflectionSecondary(const vec3d& vSun, const vec3d& rAim);
    QVector<Angles> solveReflectionGlobal(const vec3d& vSun, const vec3d& rAim);

    Angles selectSolution(const QVector<Angles>& solutions);

private:
    TrackerArmature2A* m_armature;
};
