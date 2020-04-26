#pragma once

#include <iostream>
#include "Matrix4x4.h"
#include "Ptr.h"

struct Point3D;
struct Vector3D;
struct NormalVector;
class Ray;
struct BBox;

class TONATIUH_LIBRARIES Transform
{
public:
    Transform();
    Transform(double mat[4][4]);
    Transform(const Ptr<Matrix4x4>& mdir);
    Transform(const Ptr<Matrix4x4>& mdir, const Ptr<Matrix4x4>& minv);
    Transform(double t00, double t01, double t02, double t03,
              double t10, double t11, double t12, double t13,
              double t20, double t21, double t22, double t23,
              double t30, double t31, double t32, double t33);

    Point3D operator()(const Point3D& point) const;
    void operator()(const Point3D& point, Point3D& transformedPoint) const;
    Vector3D operator()(const Vector3D& vector) const;
    void operator()(const Vector3D& vector, Vector3D& transformedVector) const;
    NormalVector operator()(const NormalVector& normal) const;
    void operator()(const NormalVector& normal, NormalVector& transformedNormal) const;
    Ray operator()(const Ray& ray) const;
    void operator()(const Ray& ray, Ray& transformedRay) const;
    BBox operator()(const BBox& bbox) const;
    void operator()(const BBox& bbox, BBox& transformedBbox) const;
    Transform operator*(const Transform& rhs) const;

    bool operator==(const Transform& mat) const;

    Ptr<Matrix4x4> GetMatrix() const {
        return m_mdir;
    }
    Transform Transpose() const;
    Transform GetInverse() const;
    bool SwapsHandedness() const;
    Vector3D multVecMatrix(const Vector3D& src) const;
    Vector3D multDirMatrix(const Vector3D& src) const;

private:
    Ptr<Matrix4x4> m_mdir;
    Ptr<Matrix4x4> m_minv;
};

TONATIUH_LIBRARIES Transform Translate(const Vector3D& delta);
TONATIUH_LIBRARIES Transform Translate(double x, double y, double z);
TONATIUH_LIBRARIES Transform Scale(double x, double y, double z);
TONATIUH_LIBRARIES Transform RotateX(double angle);
TONATIUH_LIBRARIES Transform RotateY(double angle);
TONATIUH_LIBRARIES Transform RotateZ(double angle);
TONATIUH_LIBRARIES Transform Rotate(double angle, const Vector3D& axis);
TONATIUH_LIBRARIES Transform LookAt(const Point3D& pos, const Point3D& look, const Vector3D& up);

TONATIUH_LIBRARIES std::ostream& operator<<(std::ostream& os, const Transform& tran);


