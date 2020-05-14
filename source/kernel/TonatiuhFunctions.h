#pragma once

#include "kernel/TonatiuhKernel.h"

class RandomAbstract;

class SbMatrix;
class SoTransform;
class Transform;
struct Point3D;
struct Vector3D;
class SbVec3f;

namespace tgf
{
    TONATIUH_KERNEL double AlternateBoxMuller(RandomAbstract& rand);
    TONATIUH_KERNEL SbMatrix MatrixFromTransform(const Transform& transform);
    TONATIUH_KERNEL Transform TransformFromMatrix(SbMatrix const& matrix);
    TONATIUH_KERNEL Transform TransformFromSoTransform(SoTransform* const& soTransform);
    TONATIUH_KERNEL SbMatrix MatrixFromSoTransform(SoTransform* const& soTransform);
    TONATIUH_KERNEL Vector3D makeVector3D(const SbVec3f& v);

    template<class T>
    T clamp(T x, T a, T b)
    {
        if (x <= a) return a;
        if (x >= b) return b;
        return x;
    }
}
