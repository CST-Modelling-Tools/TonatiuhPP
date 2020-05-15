#include "TrackingDrive.h"


TrackingDrive::TrackingDrive(const Vector3D& shift, const Vector3D& axis, const IntervalAngular& angles):
    shift(shift),
    axis(axis.normalized()),
    angles(angles)
{

}

Transform TrackingDrive::getTransform(double angle) const
{
    return Transform::translate(shift)*Transform::rotate(angle, axis);
}


TrackingVertex::TrackingVertex(const Vector3D& point, const Vector3D& normal):
    shift(point),
    normal(normal.normalized())
{

}
