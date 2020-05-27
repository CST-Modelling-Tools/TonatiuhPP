#include "TShapeKit.h"

#include <Inventor/nodekits/SoAppearanceKit.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransform.h>

#include "kernel/apertures/ApertureRectangle.h"
#include "kernel/material/MaterialAbsorber.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "kernel/shape/ShapePlane.h"
#include "libraries/geometry/Ray.h"


SO_KIT_SOURCE(TShapeKit)

/**
 * Sets up initialization for data common to all instances of this class, like submitting necessary information to the Coin type system.
 */
void TShapeKit::initClass()
{
    SO_KIT_INIT_CLASS(TShapeKit, SoShapeKit, "ShapeKit");
}

TShapeKit::TShapeKit()
{
    SO_KIT_CONSTRUCTOR(TShapeKit);
    isBuiltIn = TRUE;

    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(materialRT, MaterialRT, MaterialAbsorber, TRUE, topSeparator, "", TRUE);
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(aperture, Aperture, ApertureRectangle, TRUE, topSeparator, "", TRUE);
    SO_KIT_INIT_INSTANCE();

    ShapeRT* shape = new ShapePlane;
    shape->setName(shape->getTypeName());
    setPart("shape", shape);

    Aperture* aperture = new ApertureRectangle;
    aperture->setName(aperture->getTypeName());
    setPart("aperture", aperture);

    MaterialRT* materialRT = new MaterialAbsorber;
    materialRT->setName(materialRT->getTypeName());
    setPart("materialRT", materialRT);

    SoMaterial* materialGL = new SoMaterial;
    materialGL->setName("MaterialGL");
    setPart("material", materialGL);

    //SoTransform* transform = new SoTransform;
    //setPart("transform",  NULL);
}

TShapeKit::~TShapeKit()
{

}
