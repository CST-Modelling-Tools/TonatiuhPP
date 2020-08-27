#include "ArmatureJoint.h"


ArmatureJoint::ArmatureJoint(const vec3d& shift, const vec3d& axis, const IntervalPeriodic& angles):
    shift(shift),
    axis(axis.normalized()),
    angles(angles)
{

}

Transform ArmatureJoint::getTransform(double angle) const
{
    return Transform::translate(shift)*Transform::rotate(angle, axis);
}


ArmatureVertex::ArmatureVertex(const vec3d& point, const vec3d& normal):
    shift(point),
    normal(normal.normalized())
{

}
