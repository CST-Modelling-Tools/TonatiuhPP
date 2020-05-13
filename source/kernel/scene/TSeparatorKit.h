#pragma once

#include "kernel/TonatiuhKernel.h"
#include <Inventor/nodekits/SoSeparatorKit.h>

/*!
  \class TSeparatorKit TSeparatorKit.h TSeparatorKit.h
  \brief The TSeparatorKit class provides a set of others TSeparatorKit and TShapeKit nodes  with a transform and a tracker.


  The TSeparatorKit structure:
  \verbatim
  CLASS TSeparatorKitt
  -->"this"
        "callbackList"
  -->   "topSeparator"
  -->      "pickStyle"
  -->      "appearance"
  -->      "units"
  -->      "transform"
  -->      "texture2Transform"
  -->      "childList"
  -->"tracker"
  \endverbatim

  Detailed information on catalog parts:
  \verbatim
  CLASS SoSeparatorKit
  PVT   "this",  SoSeparatorKit  ---
        "callbackList",  SoNodeKitListPart [ SoCallback, SoEventCallback ]
  PVT   "topSeparator",  SoSeparator  ---
        "pickStyle",  SoPickStyle  ---
        "appearance",  SoAppearanceKit  ---
        "units",  SoUnits  ---
        "transform",  SoTransform  ---
        "texture2Transform",  SoTexture2Transform  ---
        "childList",  SoNodeKitListPart [ SoShapeKit, SoSeparatorKit ]
        "tracker", TTacker  --- , (default type = TDefaultTracker)
  \endverbatim

*/

class QString;

class TONATIUH_KERNEL TSeparatorKit: public SoSeparatorKit
{
    typedef SoSeparatorKit inherited;

    SO_KIT_HEADER(TSeparatorKit);

    SO_KIT_CATALOG_ENTRY_HEADER(tracker);

public:
    TSeparatorKit();
    static void initClass();

    virtual SbBool setPart(const SbName& partname, SoNode* from);
    virtual SoNode* getPart(const SbName &partname, SbBool makeifneeded);

    void getBoundingBox(SoGetBoundingBoxAction* action);

protected:
    ~TSeparatorKit() {}
};
