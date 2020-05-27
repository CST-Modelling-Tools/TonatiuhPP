#include "TShapeKit.h"

#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/nodekits/SoAppearanceKit.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransform.h>

#include "kernel/material/MaterialRT.h"
#include "kernel/material/MaterialAbsorber.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "kernel/shape/ShapeCube.h"
#include "kernel/shape/ShapePlane.h"
#include "kernel/shape/ShapeRT.h"
#include "libraries/geometry/Ray.h"


SO_KIT_SOURCE(TShapeKit)

/**
 * Sets up initialization for data common to all instances of this class, like submitting necessary information to the Coin type system.
 */
void TShapeKit::initClass()
{
    SO_KIT_INIT_CLASS(TShapeKit, SoShapeKit, "ShapeKit");
}

// faster getters
// does not work for loading from file
// see SceneModel::generateInstanceTree
//SbBool TShapeKit::setPart(const SbName& part, SoNode* node)
//{
//    if (part == "shape")
//        m_shape = (ShapeRT*) node;
//    else if (part == "material")
//        m_material = (MaterialRT*) node;

//    return SoShapeKit::setPart(part, node);
//}

TShapeKit::TShapeKit()
{
    SO_KIT_CONSTRUCTOR(TShapeKit);
    isBuiltIn = TRUE;

//    SO_KIT_CHANGE_ENTRY_TYPE(shape, ShapeRT, ShapePlane);
//    SO_KIT_CHANGE_NULL_BY_DEFAULT(shape, TRUE);
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(materialRT, MaterialRT, MaterialAbsorber, TRUE, topSeparator, "", TRUE);
    SO_KIT_INIT_INSTANCE();

    ShapeRT* shape = new ShapePlane;
    shape->setName(shape->getTypeName());
    setPart("shape", shape);

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
