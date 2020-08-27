#pragma once

#include "kernel/trackers/TrackerArmature1A.h"


class TrackerSolver1A
{
public:
    TrackerSolver1A(TrackerArmature1A* armature);

    vec3d findFacetPoint(double angle);

    double solveRotation(const vec3d& v0, const vec3d& v);

    double solveFacetNormal(const vec3d& normal);
    double solveReflectionPrimary(const vec3d& vSun, const vec3d& rAim);
    double solveReflectionGlobal(const vec3d& vSun, const vec3d& rAim);

    double selectSolution(double solution);

private:
    TrackerArmature1A* m_armature;
};
