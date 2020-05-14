#pragma once

#include "kernel/TonatiuhKernel.h"

class RandomAbstract;

class SbMatrix;
class SoTransform;
class Transform;

class SbVec2f;
class Vector2D;

class SbVec3f;
struct Vector3D;

namespace tgf
{
    TONATIUH_KERNEL double AlternateBoxMuller(RandomAbstract& rand);
    TONATIUH_KERNEL SbMatrix MatrixFromTransform(const Transform& transform);
    TONATIUH_KERNEL Transform TransformFromMatrix(SbMatrix const& matrix);
    TONATIUH_KERNEL Transform TransformFromSoTransform(SoTransform* const& soTransform);
    TONATIUH_KERNEL SbMatrix MatrixFromSoTransform(SoTransform* const& soTransform);

    TONATIUH_KERNEL Vector2D makeVector2D(const SbVec2f& v);
    TONATIUH_KERNEL Vector3D makeVector3D(const SbVec3f& v);
}
