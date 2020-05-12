#pragma once

#include "kernel/TonatiuhKernel.h"
#include <QDateTime>
#include <QPair>
#include <QVector>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/nodekits/SoLightKit.h>
#include "kernel/TonatiuhTypes.h"
#include "kernel/sun/SunAbstract.h"

struct BoundingBox;
class Transform;
class TShapeKit;

/*!
  \class TLightKit TLightKit.h TLightKit.h
  \brief The TLightKit class provides a kit with a transform, a light, a shape and a sunshape to represents the sun light.


  The TLightKit structure:
  \verbatim
  CLASS TLightKit
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
  CLASS TLightKit
  PVT   "this",  TLightKit  ---
        "callbackList",  SoNodeKitListPart [ SoCallback, SoEventCallback ]
  PVT   "transformGroup",  SoTransformSeparator  ---
        "transform",  SoTransform  ---
        "light",  SoLight  --- , (default type = SoDirectionalLight)
  PVT   "iconSeparator",  SoSeparator  ---
        "icon",  SoNode  --- , (default type = SoCube)
        "tsunshape",  SunShape  --- , (default type = TDefaultSunShape)
  \endverbatim

*/

class TONATIUH_KERNEL TLightKit: public SoLightKit
{
    SO_KIT_HEADER(TLightKit);
    SO_KIT_CATALOG_ENTRY_HEADER(iconMaterial);
    SO_KIT_CATALOG_ENTRY_HEADER(iconTexture);
    SO_KIT_CATALOG_ENTRY_HEADER(tsunshape);

public:
    TLightKit();
    static void initClass();

    void setPosition(double azimuth, double elevation);
    void setBox(BoundingBox box);
    void findTexture(int widthDivisions, int heightDivisions, QVector< QPair<TShapeKit*, Transform> > surfacesList);

    SoSFString disabledNodes;

private:
    ~TLightKit();
//    void UpdateSunPosition();
};
