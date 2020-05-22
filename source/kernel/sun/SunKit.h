#pragma once

#include "kernel/TonatiuhKernel.h"
#include <QDateTime>
#include <QPair>
#include <QVector>
#include <Inventor/nodekits/SoLightKit.h>
#include "kernel/TonatiuhTypes.h"
#include "kernel/sun/SunShape.h"

struct BoundingBox;
class InstanceNode;

/*!
  \class SunKit SunKit.h SunKit.h
  \brief The SunKit class provides a kit with a transform, a light, a shape and a sunshape to represents the sun light.


  The SunKit structure:
  \verbatim
  CLASS SunKit
  -->"this"
        "callbackList"
  -->   "transformGroup"
  -->      "transform"
  -->      "light"
  -->      "iconSeparator"
  -->         "icon"
  -->      "tsunshape"
  \endverbatim

  Detailed information on catalog parts:
  \verbatim
  CLASS SunKit
  PVT   "this",  SunKit  ---
        "callbackList",  SoNodeKitListPart [ SoCallback, SoEventCallback ]
  PVT   "transformGroup",  SoTransformSeparator  ---
        "transform",  SoTransform  ---
        "light",  SoLight  --- , (default type = SoDirectionalLight)
  PVT   "iconSeparator",  SoSeparator  ---
        "icon",  SoNode  --- , (default type = SoCube)
        "tsunshape",  SunShape  --- , (default type = TDefaultSunShape)
  \endverbatim

*/

class TONATIUH_KERNEL SunKit: public SoLightKit
{
    SO_KIT_HEADER(SunKit);
    SO_KIT_CATALOG_ENTRY_HEADER(iconMaterial);
    SO_KIT_CATALOG_ENTRY_HEADER(iconTexture);
    SO_KIT_CATALOG_ENTRY_HEADER(tsunshape);

public:
    static void initClass();
    SunKit();

    void updatePosition();
    void setBox(BoundingBox box);
    bool findTexture(int widthDivisions, int heightDivisions, InstanceNode* instanceRoot);

    SoSFDouble azimuth;  // in radians
    SoSFDouble elevation;
    SoSFDouble irradiance;
    SoSFString disabledNodes;

private:
    ~SunKit();
};
