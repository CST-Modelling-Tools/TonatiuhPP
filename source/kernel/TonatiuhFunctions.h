#pragma once

#include "kernel/TonatiuhKernel.h"

class SbMatrix;
class RandomAbstract;
class SoTransform;
class Transform;

namespace tgf
{
    TONATIUH_KERNEL double AlternateBoxMuller(RandomAbstract& rand);
    TONATIUH_KERNEL SbMatrix MatrixFromTransform(const Transform& transform);
    TONATIUH_KERNEL Transform TransformFromMatrix(SbMatrix const& matrix);
    TONATIUH_KERNEL Transform TransformFromSoTransform(SoTransform* const& soTransform);
    TONATIUH_KERNEL SbMatrix MatrixFromSoTransform(SoTransform* const& soTransform);
}
