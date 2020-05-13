#include "HeliostatModel.h"

#include <QVector>

// rotation around a from m to v
inline double findAngle(const Vector3D& a, const Vector3D& m, const Vector3D& v, double av)
{
    return atan2(dot(a, cross(m, v)), dot(m, v) - av*av);
}

Vector3D findAngles(
    const Vector3D& a, const Vector3D& b,
    const Vector3D& v, const Vector3D& m, const Vector3D& v0,
    double av, double bv0
)
{
    double alpha = findAngle(a, m, v, av);
    double beta = findAngle(b, v0, m, bv0);
    return Vector3D(alpha, beta, abs(alpha) + abs(beta));
}


HeliostatModel::HeliostatModel(const HeliostatDrive& primary, const HeliostatDrive& secondary, const Vertex3D& tracking, const Vector3D& angles0):
    primary(primary),
    secondary(secondary),
    tracking(tracking),
    angles0(angles0)
{

}

Vector3D HeliostatModel::findTrackingPoint(const Vector3D& angles)
{
    Vector3D r = secondary.getTransform(angles.y).transformPoint(tracking.point);
    return primary.getTransform(angles.x).transformPoint(r);
}

// rotate v0 to v
QVector<Vector3D> HeliostatModel::solveRotation(const Vector3D& v0, const Vector3D& v)
{
    const Vector3D& a = primary.axis;
    const Vector3D& b = secondary.axis;

    Vector3D k = cross(a, b);
    double k2 = k.norm2();
    double ab = dot(a, b);
    double det = 1. - ab*ab;
    if (fabs(det) < 1e-8) return {};

    double av = dot(a, v);
    double bv0 = dot(b, v0);
    double ma = (av - ab*bv0)/det;
    double mb = (bv0 - ab*av)/det;
    double mk = 1. - ma*ma - mb*mb - 2.*ma*mb*ab;
    if (mk < 0.) return {};

    mk = sqrt(mk/k2);
    Vector3D m0 = ma*a + mb*b;
    QVector<Vector3D> ans;
    ans << findAngles(a, b, v, m0 - mk*k, v0, av, bv0);
    ans << findAngles(a, b, v, m0 + mk*k, v0, av, bv0);
    return ans;
}

// rotate tracking.normal to normal
QVector<Vector3D> HeliostatModel::solveTrackingNormal(const Vector3D& normal)
{
    return solveRotation(tracking.normal, normal);
}

// vSun and rAim are local
QVector<Vector3D> HeliostatModel::solveReflectionLocal(const Vector3D& vSun, const Vector3D& rAim)
{
    Vector3D vTarget0 = (rAim - tracking.point).normalized();
    Vector3D vSun0 = -vTarget0.reflected(tracking.normal);
    return solveRotation(vSun0, vSun);
}

// vSun and rAim are local
QVector<Vector3D> HeliostatModel::solveReflectionGlobal(const Vector3D& vSun, const Vector3D& rAim)
{
    QVector<Vector3D> ans;
    int iterationsMax = 5; // max iterations
    double deltaMin = 0.01; // accuracy in meters

    for (int s = 0; s < 2; ++s) // solutions
    {
        Vector3D rMirror = findTrackingPoint(angles0);
        for (int iteration = 0; iteration < iterationsMax; ++iteration)
        {
            Vector3D vTarget = (rAim - rMirror).normalized();
            Vector3D vNormal = (vSun + vTarget).normalized();
            QVector<Vector3D> temp = solveTrackingNormal(vNormal);
            if (temp.empty()) break;
            Vector3D& angles = temp[s];
            rMirror = findTrackingPoint(angles);
            double delta = cross(rAim - rMirror, vTarget).norm();
            if (delta > deltaMin) continue;
            ans << angles;
            break;
        }
    }

    return ans;
}
