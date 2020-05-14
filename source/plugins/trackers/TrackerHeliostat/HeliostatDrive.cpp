#include "HeliostatDrive.h"


HeliostatDrive::HeliostatDrive(const Vector3D& shift, const Vector3D& axis, const IntervalAngular& angles):
    shift(shift),
    axis(axis.normalized()),
    angles(angles)
{

}

Transform HeliostatDrive::getTransform(double angle) const
{
    return Transform::translate(shift)*Transform::rotate(angle, axis);
}


Vertex3D::Vertex3D(const Vector3D& point, const Vector3D& normal):
    point(point),
    normal(normal.normalized())
{

}
