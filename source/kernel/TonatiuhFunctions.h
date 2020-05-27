#pragma once

#include "kernel/TonatiuhKernel.h"

class Random;

class SbMatrix;
class SoTransform;
class Transform;

class SbVec2f;
class Vector2D;

class SbVec3f;
struct Vector3D;

namespace tgf
{
    TONATIUH_KERNEL SbMatrix makeSbMatrix(const Transform& transform);
    TONATIUH_KERNEL SbMatrix makeSbMatrix(SoTransform* soTransform);

    TONATIUH_KERNEL Vector2D makeVector2D(const SbVec2f& v);
    TONATIUH_KERNEL Vector3D makeVector3D(const SbVec3f& v);
    TONATIUH_KERNEL Transform makeTransform(const SbMatrix& matrix);
    TONATIUH_KERNEL Transform makeTransform(SoTransform* soTransform);
}
