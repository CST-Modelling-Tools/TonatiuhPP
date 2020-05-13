#include "Transform.h"

#include "gc.h"
#include "Ray.h"
#include "BoundingBox.h"
#include "NormalVector.h"

Transform::Transform():
    m_mdir(0), m_minv(0)
{

}

Transform::Transform(double m[4][4])
{
    m_mdir = new Matrix4x4(
        m[0][0], m[0][1], m[0][2], m[0][3],
        m[1][0], m[1][1], m[1][2], m[1][3],
        m[2][0], m[2][1], m[2][2], m[2][3],
        m[3][0], m[3][1], m[3][2], m[3][3]
    );
    m_minv = m_mdir->Inverse();
}

Transform::Transform(
    double t00, double t01, double t02, double t03,
    double t10, double t11, double t12, double t13,
    double t20, double t21, double t22, double t23,
    double t30, double t31, double t32, double t33
)
{
    m_mdir = new Matrix4x4(
        t00, t01, t02, t03,
        t10, t11, t12, t13,
        t20, t21, t22, t23,
        t30, t31, t32, t33
    );
    m_minv = m_mdir->Inverse();
}

Transform::Transform(const Ptr<Matrix4x4>& mdir):
    m_mdir(mdir),
    m_minv(mdir->Inverse())
{

}

Transform::Transform(const Ptr<Matrix4x4>& mdir, const Ptr<Matrix4x4>& minv):
    m_mdir(mdir),
    m_minv(minv)
{

}

Vector3D Transform::transformPoint(const Vector3D& v) const
{
    return Vector3D(
        m_mdir->m[0][0]*v.x + m_mdir->m[0][1]*v.y + m_mdir->m[0][2]*v.z + m_mdir->m[0][3],
        m_mdir->m[1][0]*v.x + m_mdir->m[1][1]*v.y + m_mdir->m[1][2]*v.z + m_mdir->m[1][3],
        m_mdir->m[2][0]*v.x + m_mdir->m[2][1]*v.y + m_mdir->m[2][2]*v.z + m_mdir->m[2][3]
    );
}

Vector3D Transform::transformVector(const Vector3D& v) const
{
    return Vector3D(
        m_mdir->m[0][0]*v.x + m_mdir->m[0][1]*v.y + m_mdir->m[0][2]*v.z,
        m_mdir->m[1][0]*v.x + m_mdir->m[1][1]*v.y + m_mdir->m[1][2]*v.z,
        m_mdir->m[2][0]*v.x + m_mdir->m[2][1]*v.y + m_mdir->m[2][2]*v.z
    );
}

Vector3D Transform::transformNormal(const Vector3D& v) const
{
    return Vector3D(
        m_minv->m[0][0]*v.x + m_minv->m[1][0]*v.y + m_minv->m[2][0]*v.z,
        m_minv->m[0][1]*v.x + m_minv->m[1][1]*v.y + m_minv->m[2][1]*v.z,
        m_minv->m[0][2]*v.x + m_minv->m[1][2]*v.y + m_minv->m[2][2]*v.z
    );
}

Point3D Transform::operator()(const Point3D& p) const
{
    Point3D ans(
        m_mdir->m[0][0]*p.x + m_mdir->m[0][1]*p.y + m_mdir->m[0][2]*p.z + m_mdir->m[0][3],
        m_mdir->m[1][0]*p.x + m_mdir->m[1][1]*p.y + m_mdir->m[1][2]*p.z + m_mdir->m[1][3],
        m_mdir->m[2][0]*p.x + m_mdir->m[2][1]*p.y + m_mdir->m[2][2]*p.z + m_mdir->m[2][3]
    );

    double w = m_mdir->m[3][0]*p.x + m_mdir->m[3][1]*p.y + m_mdir->m[3][2]*p.z + m_mdir->m[3][3];
    if (w != 1.) return ans /= w;

    return ans;
}

void Transform::operator()(const Point3D& p, Point3D& ans) const
{
    ans.x = m_mdir->m[0][0]*p.x + m_mdir->m[0][1]*p.y + m_mdir->m[0][2]*p.z + m_mdir->m[0][3];
    ans.y = m_mdir->m[1][0]*p.x + m_mdir->m[1][1]*p.y + m_mdir->m[1][2]*p.z + m_mdir->m[1][3];
    ans.z = m_mdir->m[2][0]*p.x + m_mdir->m[2][1]*p.y + m_mdir->m[2][2]*p.z + m_mdir->m[2][3];

    double w = m_mdir->m[3][0]*p.x + m_mdir->m[3][1]*p.y + m_mdir->m[3][2]*p.z + m_mdir->m[3][3];
    if (w != 1.) ans /= w;
}

Vector3D Transform::operator()(const Vector3D& v) const
{
    return Vector3D(
        m_mdir->m[0][0]*v.x + m_mdir->m[0][1]*v.y + m_mdir->m[0][2]*v.z,
        m_mdir->m[1][0]*v.x + m_mdir->m[1][1]*v.y + m_mdir->m[1][2]*v.z,
        m_mdir->m[2][0]*v.x + m_mdir->m[2][1]*v.y + m_mdir->m[2][2]*v.z
    );
}

void Transform::operator()(const Vector3D& v, Vector3D& ans) const
{
    ans.x = m_mdir->m[0][0]*v.x + m_mdir->m[0][1]*v.y + m_mdir->m[0][2]*v.z;
    ans.y = m_mdir->m[1][0]*v.x + m_mdir->m[1][1]*v.y + m_mdir->m[1][2]*v.z;
    ans.z = m_mdir->m[2][0]*v.x + m_mdir->m[2][1]*v.y + m_mdir->m[2][2]*v.z;
}

NormalVector Transform::operator()(const NormalVector& n) const
{
    // https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/geometry/transforming-normals
    return NormalVector(
        m_minv->m[0][0]*n.x + m_minv->m[1][0]*n.y + m_minv->m[2][0]*n.z,
        m_minv->m[0][1]*n.x + m_minv->m[1][1]*n.y + m_minv->m[2][1]*n.z,
        m_minv->m[0][2]*n.x + m_minv->m[1][2]*n.y + m_minv->m[2][2]*n.z
    );
}

void Transform::operator()(const NormalVector& n, NormalVector& ans) const
{
    ans.x = m_minv->m[0][0]*n.x + m_minv->m[1][0]*n.y + m_minv->m[2][0]*n.z;
    ans.y = m_minv->m[0][1]*n.x + m_minv->m[1][1]*n.y + m_minv->m[2][1]*n.z;
    ans.z = m_minv->m[0][2]*n.x + m_minv->m[1][2]*n.y + m_minv->m[2][2]*n.z;
}

Ray Transform::operator()(const Ray& r) const
{
    Ray ans;
    (*this)(r.origin, ans.origin);
    Vector3D d = (*this)(r.direction());
    ans.setDirection(d);
    ans.tMin = r.tMin;
    ans.tMax = r.tMax;
    return ans;
}

void Transform::operator()(const Ray& r, Ray& ans) const
{
    (*this)(r.origin, ans.origin);
    Vector3D d = (*this)(r.direction());
    ans.setDirection(d);
    ans.tMin = r.tMin;
    ans.tMax = r.tMax;
}

BoundingBox Transform::operator()(const BoundingBox& b) const
{
    BoundingBox ans;
    const Transform& t = *this;
    ans << t(Point3D(b.pMin.x, b.pMin.y, b.pMin.z));
    ans << t(Point3D(b.pMin.x, b.pMin.y, b.pMax.z));
    ans << t(Point3D(b.pMin.x, b.pMax.y, b.pMin.z));
    ans << t(Point3D(b.pMin.x, b.pMax.y, b.pMax.z));
    ans << t(Point3D(b.pMax.x, b.pMin.y, b.pMin.z));
    ans << t(Point3D(b.pMax.x, b.pMin.y, b.pMax.z));
    ans << t(Point3D(b.pMax.x, b.pMax.y, b.pMin.z));
    ans << t(Point3D(b.pMax.x, b.pMax.y, b.pMax.z));
    return ans;
}

void Transform::operator()(const BoundingBox& b, BoundingBox& ans) const
{
    ans = BoundingBox();
    const Transform& t = *this;
    ans << t(Point3D(b.pMin.x, b.pMin.y, b.pMin.z));
    ans << t(Point3D(b.pMin.x, b.pMin.y, b.pMax.z));
    ans << t(Point3D(b.pMin.x, b.pMax.y, b.pMin.z));
    ans << t(Point3D(b.pMin.x, b.pMax.y, b.pMax.z));
    ans << t(Point3D(b.pMax.x, b.pMin.y, b.pMin.z));
    ans << t(Point3D(b.pMax.x, b.pMin.y, b.pMax.z));
    ans << t(Point3D(b.pMax.x, b.pMax.y, b.pMin.z));
    ans << t(Point3D(b.pMax.x, b.pMax.y, b.pMax.z));
}

Transform Transform::operator*(const Transform& rhs) const
{
    Ptr<Matrix4x4> mdir = Mul(m_mdir, rhs.m_mdir);
    Ptr<Matrix4x4> minv = Mul(rhs.m_minv, m_minv);
    return Transform(mdir, minv);
}

bool Transform::operator==(const Transform& t) const
{
    if (this == &t) return true;
    else return( (fabs(m_mdir->m[0][0] - t.m_mdir->m[0][0]) < gc::Epsilon) &&
             (fabs(m_mdir->m[0][1] - t.m_mdir->m[0][1]) < gc::Epsilon) &&
             (fabs(m_mdir->m[0][2] - t.m_mdir->m[0][2]) < gc::Epsilon) &&
             (fabs(m_mdir->m[0][3] - t.m_mdir->m[0][3]) < gc::Epsilon) &&
             (fabs(m_mdir->m[1][0] - t.m_mdir->m[1][0]) < gc::Epsilon) &&
             (fabs(m_mdir->m[1][1] - t.m_mdir->m[1][1]) < gc::Epsilon) &&
             (fabs(m_mdir->m[1][2] - t.m_mdir->m[1][2]) < gc::Epsilon) &&
             (fabs(m_mdir->m[1][3] - t.m_mdir->m[1][3]) < gc::Epsilon) &&
             (fabs(m_mdir->m[2][0] - t.m_mdir->m[2][0]) < gc::Epsilon) &&
             (fabs(m_mdir->m[2][1] - t.m_mdir->m[2][1]) < gc::Epsilon) &&
             (fabs(m_mdir->m[2][2] - t.m_mdir->m[2][2]) < gc::Epsilon) &&
             (fabs(m_mdir->m[2][3] - t.m_mdir->m[2][3]) < gc::Epsilon) &&
             (fabs(m_mdir->m[3][0] - t.m_mdir->m[3][0]) < gc::Epsilon) &&
             (fabs(m_mdir->m[3][1] - t.m_mdir->m[3][1]) < gc::Epsilon) &&
             (fabs(m_mdir->m[3][2] - t.m_mdir->m[3][2]) < gc::Epsilon) &&
             (fabs(m_mdir->m[3][3] - t.m_mdir->m[3][3]) < gc::Epsilon) );
}

Transform Transform::inversed() const
{
    return Transform(m_minv, m_mdir);
}

Transform Transform::transposed() const
{
    return Transform(m_mdir->Transpose(), m_mdir->Transpose()->Inverse() ); //?
}


Vector3D Transform::multVecMatrix(const Vector3D& v) const
{
    Vector3D ans;

    const double* t0 = m_mdir->m[0];
    const double* t1 = m_mdir->m[1];
    const double* t2 = m_mdir->m[2];
    const double* t3 = m_mdir->m[3];

    double w = t3[0]*v[0] + t3[1]*v[1] + t3[2]*v[2] + t3[3];

    if (w != 0.) {
        ans[0] = (t0[0]*v[0] + t0[1]*v[1] + t0[2]*v[2] + t0[3])/w;
        ans[1] = (t1[0]*v[0] + t1[1]*v[1] + t1[2]*v[2] + t1[3])/w;
        ans[2] = (t2[0]*v[0] + t2[1]*v[1] + t2[2]*v[2] + t2[3])/w;
    }

    return ans;
}

Vector3D Transform::multDirMatrix(const Vector3D& src) const
{
    Vector3D dst;
    // Checks if the "this" matrix is equal to the identity matrix.  See
    // also code comments at the start of SbMatrix::multRight().
    //if (SbMatrixP::isIdentity(this->matrix)) { dst = src; return dst; }


    const double* t0 = (m_mdir->m)[0];
    const double* t1 = (m_mdir->m)[1];
    const double* t2 = (m_mdir->m)[2];
    // Copy the src vector, just in case src and dst is the same vector.
    dst[0] = src[0]*t0[0] + src[1]*t0[1] + src[2]*t0[2];
    dst[1] = src[0]*t1[0] + src[1]*t1[1] + src[2]*t1[2];
    dst[2] = src[0]*t2[0] + src[1]*t2[1] + src[2]*t2[2];

    return dst;
}
bool Transform::SwapsHandedness() const
{
    double det = ( (m_mdir->m[0][0] *
                    (m_mdir->m[1][1]*m_mdir->m[2][2] -
                     m_mdir->m[1][2]*m_mdir->m[2][1]) ) -
                   (m_mdir->m[0][1] *
                    (m_mdir->m[1][0]*m_mdir->m[2][2] -
                     m_mdir->m[1][2]*m_mdir->m[2][0]) ) +
                   (m_mdir->m[0][2] *
                    (m_mdir->m[1][0]*m_mdir->m[2][1] -
                     m_mdir->m[1][1]*m_mdir->m[2][0]) ) );
    return det < 0.0;
}


Transform Transform::Translate(const Vector3D& delta)
{
    Ptr<Matrix4x4> mdir = new Matrix4x4(1.0,   0.0,   0.0,  delta.x,
                                        0.0,   1.0,   0.0,  delta.y,
                                        0.0,   0.0,   1.0,  delta.z,
                                        0.0,   0.0,   0.0,      1.0);

    Ptr<Matrix4x4> minv = new Matrix4x4(1.0,   0.0,   0.0, -delta.x,
                                        0.0,   1.0,   0.0, -delta.y,
                                        0.0,   0.0,   1.0, -delta.z,
                                        0.0,   0.0,   0.0,      1.0);

    return Transform(mdir, minv);
}

Transform Transform::Translate(double x, double y, double z)
{
    Ptr<Matrix4x4> mdir = new Matrix4x4(1.0,   0.0,   0.0,   x,
                                        0.0,   1.0,   0.0,   y,
                                        0.0,   0.0,   1.0,   z,
                                        0.0,   0.0,   0.0, 1.0);

    Ptr<Matrix4x4> minv = new Matrix4x4(1.0,   0.0,   0.0,   -x,
                                        0.0,   1.0,   0.0,   -y,
                                        0.0,   0.0,   1.0,   -z,
                                        0.0,   0.0,   0.0,  1.0);

    return Transform(mdir, minv);
}

Transform Transform::Scale(double sx, double sy, double sz)
{
    Ptr<Matrix4x4> mdir = new Matrix4x4(sx,     0.0,    0.0,  0.0,
                                        0.0,      sy,    0.0,  0.0,
                                        0.0,     0.0,     sz,  0.0,
                                        0.0,     0.0,    0.0,  1.0);

    Ptr<Matrix4x4> minv = new Matrix4x4(1.0 / sx,    0.0,    0.0,  0.0,
                                        0.0, 1.0 / sy,    0.0,  0.0,
                                        0.0,    0.0, 1.0 / sz,  0.0,
                                        0.0,    0.0,    0.0,  1.0);

    return Transform(mdir, minv);
}

Transform Transform::RotateX(double angle)
{
    // angle is assumed to be in radians.
    double sinAngle = sin(angle);
    double cosAngle = cos(angle);

    Ptr<Matrix4x4> mdir = new Matrix4x4(1.0,      0.0,       0.0, 0.0,
                                        0.0, cosAngle, -sinAngle, 0.0,
                                        0.0, sinAngle,  cosAngle, 0.0,
                                        0.0,      0.0,       0.0, 1.0);

    return Transform(mdir, mdir->Transpose() );
}

Transform Transform::RotateY(double angle)
{
    // angle is assumed to be in radians.
    double sinAngle = sin(angle);
    double cosAngle = cos(angle);

    Ptr<Matrix4x4> mdir = new Matrix4x4(cosAngle, 0.0, sinAngle, 0.0,
                                        0.0, 1.0,      0.0, 0.0,
                                        -sinAngle, 0.0, cosAngle, 0.0,
                                        0.0, 0.0,      0.0, 1.0);

    return Transform(mdir, mdir->Transpose() );
}


Transform Transform::RotateZ(double angle)
{
    // angle is assumed to be in radians.
    double sinAngle = sin(angle);
    double cosAngle = cos(angle);

    Ptr<Matrix4x4> mdir = new Matrix4x4(cosAngle, -sinAngle, 0.0, 0.0,
                                        sinAngle,  cosAngle, 0.0, 0.0,
                                        0.0,       0.0, 1.0, 0.0,
                                        0.0,       0.0, 0.0, 1.0);

    return Transform(mdir, mdir->Transpose() );
}

Transform Transform::Rotate(double angle, const Vector3D& axis)
{
    // angle is assumed to be in radians.
    Vector3D a = axis.normalized();
    double s = sin(angle);
    double c = cos(angle);
    double m[4][4];

    m[0][0] = a.x*a.x + (1.0 - a.x*a.x)*c;
    m[0][1] = a.x*a.y*(1.0 - c) - a.z*s;
    m[0][2] = a.x*a.z*(1.0 - c) + a.y*s;
    m[0][3] = 0.0;

    m[1][0] = a.x*a.y*(1.0 - c) + a.z*s;
    m[1][1] = a.y*a.y + (1.0 - a.y*a.y)*c;
    m[1][2] = a.y*a.z*(1.0 - c) - a.x*s;
    m[1][3] = 0.0;

    m[2][0] = a.x*a.z*(1.0 - c) - a.y*s;
    m[2][1] = a.y*a.z*(1.0 - c) + a.x*s;
    m[2][2] = a.z*a.z + (1.0 - a.z*a.z)*c;
    m[2][3] = 0.0;

    m[3][0] = 0.0;
    m[3][1] = 0.0;
    m[3][2] = 0.0;
    m[3][3] = 1.0;

    Ptr<Matrix4x4> mdir = new Matrix4x4(m);
    return Transform(mdir, mdir->Transpose() );
}

Transform Transform::LookAt(const Point3D& pos, const Point3D& look, const Vector3D& up)
{
    double m[4][4];
    m[0][3] = pos.x;
    m[1][3] = pos.y;
    m[2][3] = pos.z;
    m[3][3] = 1.;

    Vector3D dir = Normalize(look - pos);
    Vector3D right = cross(dir, up).normalized();
    Vector3D newUp = cross(right, dir);

    m[0][0] = right.x;
    m[1][0] = right.y;
    m[2][0] = right.z;
    m[3][0] = 0.;

    m[0][1] = dir.x;
    m[1][1] = dir.y;
    m[2][1] = dir.z;
    m[3][1] = 0.;

    m[0][2] = newUp.x;
    m[1][2] = newUp.y;
    m[2][2] = newUp.z;
    m[3][2] = 0.;

    Ptr<Matrix4x4> camToWorld = new Matrix4x4(m);
    return Transform(camToWorld->Inverse(), camToWorld);
}

std::ostream& operator<<(std::ostream& os, const Transform& tran)
{
    os << (*tran.GetMatrix());
    return os;
}
