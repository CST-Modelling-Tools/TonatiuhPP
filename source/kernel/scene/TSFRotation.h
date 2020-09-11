#pragma once

#include "kernel/TonatiuhKernel.h"

#include <Inventor/fields/SoSField.h>
#include <Inventor/fields/SoSubField.h>
#include <Inventor/SbRotation.h>


class TONATIUH_KERNEL TSFRotation: public SoSField
{
    typedef SoSField inherited;

    SO_SFIELD_HEADER(TSFRotation, SbRotation, const SbRotation&)

public:
    static void initClass(void);

    void getValue(SbVec3f & axis, float & angle) const;
    void setValue(const float q0, const float q1, const float q2, const float q3);
    void setValue(const float q[4]);
    void setValue(const SbVec3f & axis, const float angle);
};
