#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/nodekits/SoAppearanceKit.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransform.h>

#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/geometry/Ray.h"
#include "kernel/shape/ShapeCube.h"
#include "kernel/material/MaterialAbstract.h"
#include "kernel/shape/ShapeAbstract.h"
#include "TShapeKit.h"


SO_KIT_SOURCE(TShapeKit)

/**
 * Sets up initialization for data common to all instances of this class, like submitting necessary information to the Coin type system.
 */
void TShapeKit::initClass()
{
    SO_KIT_INIT_CLASS(TShapeKit, SoShapeKit, "ShapeKit");
}

/**
 * Constructor.
 *
 */
TShapeKit::TShapeKit()
{
    SO_KIT_CONSTRUCTOR(TShapeKit);

    SO_KIT_CHANGE_ENTRY_TYPE(shape, ShapeAbstract, ShapeCube);
    SO_KIT_CHANGE_NULL_BY_DEFAULT(shape, TRUE);
    SO_KIT_INIT_INSTANCE();

    setPart("shape", NULL);

    //SoTransform* transform = new SoTransform;
    //setPart("transform",  NULL);
}
/*!
 * Destroys the TShapeKit object.
 */
TShapeKit::~TShapeKit()
{

}

/**
 * Check if ray intersects with the node.
 *
 */
bool TShapeKit::IntersectP(const Ray&) const
{
    return false;
}

/**
 * Interect \a object ray with the shape and computed reflected ray. The \a object ray
 * is on shape local coordinates.
 *
 *Return the reflected ray. If the returned value is null, there is not reflected ray.
 */
Ray* TShapeKit::Intersect(const Ray& /* objectRay */, bool* /* isShapeFront */, RandomDeviate& /* rand */ ) const
{
    return 0;
}
