#pragma once

#include <iostream>
#include "Matrix4x4.h"
#include "Ptr.h"
#include "Vector3D.h"

struct Point3D;
struct NormalVector;
class Ray;
struct BoundingBox;


class TONATIUH_LIBRARIES Transform
{
public:
    Transform();
    Transform(double m[4][4]);
    Transform(
        double t00, double t01, double t02, double t03,
        double t10, double t11, double t12, double t13,
        double t20, double t21, double t22, double t23,
        double t30, double t31, double t32, double t33
    );
    Transform(const Ptr<Matrix4x4>& mdir);
    Transform(const Ptr<Matrix4x4>& mdir, const Ptr<Matrix4x4>& minv);

    Vector3D transformPoint(const Vector3D& v) const;
    Vector3D transformVector(const Vector3D& v) const;
    Vector3D transformNormal(const Vector3D& v) const;

    Point3D operator()(const Point3D& p) const;
    void operator()(const Point3D& p, Point3D& ans) const;

    Vector3D operator()(const Vector3D& v) const;
    void operator()(const Vector3D& v, Vector3D& ans) const;

    NormalVector operator()(const NormalVector& n) const;
    void operator()(const NormalVector& n, NormalVector& ans) const;

    Ray operator()(const Ray& r) const;
    void operator()(const Ray& r, Ray& ans) const;

    BoundingBox operator()(const BoundingBox& b) const;
    void operator()(const BoundingBox& b, BoundingBox& ans) const;


    Transform operator*(const Transform& rhs) const;

    bool operator==(const Transform& mat) const;

    Ptr<Matrix4x4> GetMatrix() const {return m_mdir;}

    Transform transposed() const;
    Transform inversed() const {return Transform(m_minv, m_mdir);}
    bool SwapsHandedness() const;

    Vector3D multVecMatrix(const Vector3D& v) const;
    Vector3D multDirMatrix(const Vector3D& src) const;

    static Transform translate(double x, double y, double z);
    static Transform translate(const Vector3D& v) {return translate(v.x, v.y, v.z);}
    static Transform scale(double x, double y, double z);
    static Transform rotateX(double angle);
    static Transform rotateY(double angle);
    static Transform rotateZ(double angle);
    static Transform rotate(double angle, const Vector3D& axis);
    static Transform LookAt(const Point3D& pos, const Point3D& look, const Vector3D& up);

private:
    Ptr<Matrix4x4> m_mdir;
    Ptr<Matrix4x4> m_minv;
};


TONATIUH_LIBRARIES std::ostream& operator<<(std::ostream& os, const Transform& tran);


