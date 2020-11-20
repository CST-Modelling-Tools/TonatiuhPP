#pragma once

#include "kernel/TonatiuhKernel.h"

#include <QPointF>
class Random;

class SbMatrix;
class SoTransform;
class TTransform;
class Transform;

class SbVec2f;
class vec2d;

class SbVec3f;
class QPointF;
struct vec3d;

namespace tgf
{
    TONATIUH_KERNEL SbMatrix makeSbMatrix(const Transform& transform);
    TONATIUH_KERNEL SbMatrix makeSbMatrix(TTransform* soTransform);
    TONATIUH_KERNEL SbMatrix makeSbMatrix(SoTransform* soTransform);

    TONATIUH_KERNEL vec2d makeVector2D(const QPointF& p);
    TONATIUH_KERNEL vec2d makeVector2D(const SbVec2f& v);
    TONATIUH_KERNEL vec3d makeVector3D(const SbVec3f& v);
    TONATIUH_KERNEL Transform makeTransform(const SbMatrix& matrix);
    TONATIUH_KERNEL Transform makeTransform(TTransform* soTransform);
    TONATIUH_KERNEL Transform makeTransform(SoTransform* soTransform);
}
