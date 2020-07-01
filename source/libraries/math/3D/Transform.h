#pragma once

#include "libraries/math/3D/vec3d.h"
#include "libraries/math/3D/Matrix4x4.h"

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

    vec3d transformPoint(const vec3d& p) const;
    vec3d transformVector(const vec3d& v) const;
    vec3d transformNormal(const vec3d& n) const;
    vec3d transformInverseNormal(const vec3d& n) const;
    Ray transformDirect(const Ray& r) const;
    Ray transformInverse(const Ray& r) const;

    vec3d operator()(const vec3d& v) const;
    void operator()(const vec3d& v, vec3d& ans) const;

    Ray operator()(const Ray& r) const;
    void operator()(const Ray& r, Ray& ans) const;

    Box3D operator()(const Box3D& b) const;
    void operator()(const Box3D& b, Box3D& ans) const;

    vec3d multVecMatrix(const vec3d& v) const;
    vec3d multDirMatrix(const vec3d& src) const;

    bool operator==(const Transform& mat) const;

    static const Transform Identity;
    static Transform translate(double x, double y, double z);
    static Transform translate(const vec3d& v) {return translate(v.x, v.y, v.z);}
    static Transform scale(double x, double y, double z);
    static Transform rotateX(double angle);
    static Transform rotateY(double angle);
    static Transform rotateZ(double angle);
    static Transform rotate(double angle, const vec3d& axis);
    static Transform LookAt(const vec3d& pos, const vec3d& look, const vec3d& up);

private:
    std::shared_ptr<Matrix4x4> m_mdir;
    std::shared_ptr<Matrix4x4> m_minv;
};


TONATIUH_LIBRARIES std::ostream& operator<<(std::ostream& os, const Transform& tran);
