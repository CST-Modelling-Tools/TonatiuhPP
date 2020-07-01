#include "TrackingDrive.h"


TrackingDrive::TrackingDrive(const vec3d& shift, const vec3d& axis, const IntervalAngular& angles):
    shift(shift),
    axis(axis.normalized()),
    angles(angles)
{

}

Transform TrackingDrive::getTransform(double angle) const
{
    return Transform::translate(shift)*Transform::rotate(angle, axis);
}


TrackingVertex::TrackingVertex(const vec3d& point, const vec3d& normal):
    shift(point),
    normal(normal.normalized())
{

}
