#include <cmath>

#include <Inventor/SbVec2f.h>
#include <Inventor/nodes/SoTransform.h>

#include "kernel/random/Random.h"
#include "TonatiuhFunctions.h"
#include "libraries/math/3D/Transform.h"
#include "libraries/math/3D/vec3d.h"
#include "libraries/math/2D/vec2d.h"

// https://www.openinventor.com/reference-manuals/NewRefMan9912/RefManJava/com/openinventor/inventor/SbMatrix.html


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

vec2d tgf::makeVector2D(const SbVec2f& v)
{
    return vec2d(v[0], v[1]);
}

vec3d tgf::makeVector3D(const SbVec3f& v)
{
    return vec3d(v[0], v[1], v[2]);
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
