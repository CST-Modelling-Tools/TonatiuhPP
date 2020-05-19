#include <cmath>

#include <Inventor/SbVec2f.h>
#include <Inventor/nodes/SoTransform.h>

#include "kernel/random/RandomAbstract.h"
#include "TonatiuhFunctions.h"
#include "libraries/geometry/Transform.h"
#include "libraries/geometry/Vector3D.h"
#include "libraries/geometry/Vector2D.h"


double tgf::AlternateBoxMuller(RandomAbstract& rand)
{
    static bool firsttime = true;
    static double x1;
    static double x2;

    if (firsttime) {
        double s = 2;
        double u1;
        double u2;
        while (s > 1) {
            u1 = 2*rand.RandomDouble() - 1;
            u2 = 2*rand.RandomDouble() - 1;
            s = u1*u1 + u2*u2;
        }

        double z = sqrt( -2 * log(s) / s );
        x1 = z*u1;
        x2 = z*u2;

        firsttime = false;
        return x1;
    } else {
        firsttime = true;
        return x2;
    }
}

SbMatrix tgf::makeSbMatrix(const Transform& transform)
{
    std::shared_ptr<Matrix4x4> transformMatrix = transform.getMatrix()->transposed();
    float m00 = float(transformMatrix->m[0][0]);
    float m01 = float(transformMatrix->m[0][1]);
    float m02 = float(transformMatrix->m[0][2]);
    float m03 = float(transformMatrix->m[0][3]);
    float m10 = float(transformMatrix->m[1][0]);
    float m11 = float(transformMatrix->m[1][1]);
    float m12 = float(transformMatrix->m[1][2]);
    float m13 = float(transformMatrix->m[1][3]);
    float m20 = float(transformMatrix->m[2][0]);
    float m21 = float(transformMatrix->m[2][1]);
    float m22 = float(transformMatrix->m[2][2]);
    float m23 = float(transformMatrix->m[2][3]);
    float m30 = float(transformMatrix->m[3][0]);
    float m31 = float(transformMatrix->m[3][1]);
    float m32 = float(transformMatrix->m[3][2]);
    float m33 = float(transformMatrix->m[3][3]);

    SbVec3f axis1(m00, m10, m20);
    SbVec3f axis2(m01, m11, m21);
    SbVec3f axis3(m02, m12, m22);
    //axis2.normalize();
    //axis3.normalize();

    return SbMatrix(
        axis1[0], axis2[0], axis3[0], m03,
        axis1[1], axis2[1], axis3[1], m13,
        axis1[2], axis2[2], axis3[2], m23,
        m30, m31, m32, m33
    );
}

SbMatrix tgf::makeSbMatrix(SoTransform* t)
{
    SbMatrix ans;
    ans.setTransform(
        t->translation.getValue(),
        t->rotation.getValue(),
        t->scaleFactor.getValue(),
        t->scaleOrientation.getValue(),
        t->center.getValue()
    );
    return ans;
}

Vector2D tgf::makeVector2D(const SbVec2f& v)
{
    return Vector2D(v[0], v[1]);
}

Vector3D tgf::makeVector3D(const SbVec3f& v)
{
    return Vector3D(v[0], v[1], v[2]);
}

Transform tgf::makeTransform(const SbMatrix& m)
{
    return Transform(
        m[0][0], m[1][0], m[2][0], m[3][0],
        m[0][1], m[1][1], m[2][1], m[3][1],
        m[0][2], m[1][2], m[2][2], m[3][2],
        m[0][3], m[1][3], m[2][3], m[3][3]
    );
}

Transform tgf::makeTransform(SoTransform* soTransform)
{
    return makeTransform(makeSbMatrix(soTransform));
}
