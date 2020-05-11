#pragma once

#include <Inventor/fields/SoSFBool.h>

#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFDouble.h>

#include <Inventor/fields/SoSFVec2f.h>
#include <Inventor/fields/SoSFVec2d.h>

#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFVec3d.h>

#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoMFVec3d.h>

#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSubField.h>


namespace trt
{

typedef SoSFBool TONATIUH_BOOL;

#if unix //( defined(Q_WS_X11) || defined(Q_WS_MAC) )
    typedef SoSFDouble TONATIUH_REAL;
    typedef SoMFVec3d TONATIUH_CONTAINERREALVECTOR3;
    typedef SoSFVec3d TONATIUH_REALVECTOR3;
    typedef SoSFVec2d TONATIUH_REALVECTOR2;
#else
    typedef SoSFFloat TONATIUH_REAL;
    typedef SoMFVec3f TONATIUH_CONTAINERREALVECTOR3;
    typedef SoSFVec3f TONATIUH_REALVECTOR3;
    typedef SoSFVec2f TONATIUH_REALVECTOR2;
#endif

}
