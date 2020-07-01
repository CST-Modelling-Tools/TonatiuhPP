#pragma once

#include "libraries/math/Vector3D.h"
#include "libraries/math/Matrix4x4.h"

class Ray;
struct Box3D;


class TONATIUH_LIBRARIES Transform
{
public:
    Transform();
    Transform(
        double t00, double t01, double t02, double t03,
        double t10, double t11, double t12, double t13,
        double t20, double t21, double t22, double t23,
        double t30, double t31, double t32, double t33
    );
    Transform(double m[4][4]);
    Transform(Matrix4x4* m);
    Transform(const std::shared_ptr<Matrix4x4>& mdir);
    Transform(const std::shared_ptr<Matrix4x4>& mdir, const std::shared_ptr<Matrix4x4>& minv);

    std::shared_ptr<Matrix4x4> getMatrix() const {return m_mdir;}
    Transform transposed() const {return Transform(m_mdir->transposed(), m_minv->transposed());}
    Transform inversed() const {return Transform(m_minv, m_mdir);}
    bool SwapsHandedness() const;

    Transform operator*(const Transform& rhs) const;

    Vector3D transformPoint(const Vector3D& p) const;
    Vector3D transformVector(const Vector3D& v) const;
    Vector3D transformNormal(const Vector3D& n) const;
    Vector3D transformInverseNormal(const Vector3D& n) const;
    Ray transformDirect(const Ray& r) const;
    Ray transformInverse(const Ray& r) const;

    Vector3D operator()(const Vector3D& v) const;
    void operator()(const Vector3D& v, Vector3D& ans) const;

    Ray operator()(const Ray& r) const;
    void operator()(const Ray& r, Ray& ans) const;

    Box3D operator()(const Box3D& b) const;
    void operator()(const Box3D& b, Box3D& ans) const;

    Vector3D multVecMatrix(const Vector3D& v) const;
    Vector3D multDirMatrix(const Vector3D& src) const;

    bool operator==(const Transform& mat) const;

    static const Transform Identity;
    static Transform translate(double x, double y, double z);
    static Transform translate(const Vector3D& v) {return translate(v.x, v.y, v.z);}
    static Transform scale(double x, double y, double z);
    static Transform rotateX(double angle);
    static Transform rotateY(double angle);
    static Transform rotateZ(double angle);
    static Transform rotate(double angle, const Vector3D& axis);
    static Transform LookAt(const Vector3D& pos, const Vector3D& look, const Vector3D& up);

private:
    std::shared_ptr<Matrix4x4> m_mdir;
    std::shared_ptr<Matrix4x4> m_minv;
};


TONATIUH_LIBRARIES std::ostream& operator<<(std::ostream& os, const Transform& tran);
