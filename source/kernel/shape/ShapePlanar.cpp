#include "ShapePlanar.h"

#include "kernel/profiles/ProfileRT.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/math/3D/Box3D.h"
#include "libraries/math/3D/Ray.h"

SO_NODE_SOURCE(ShapePlanar)


void ShapePlanar::initClass()
{
    SO_NODE_INIT_CLASS(ShapePlanar, ShapeRT, "ShapeRT");
}

ShapePlanar::ShapePlanar()
{
    SO_NODE_CONSTRUCTOR(ShapePlanar);
    isBuiltIn = TRUE;
}

void ShapePlanar::updateShapeGL(TShapeKit* parent)
{
    makeQuadMesh(parent, QSize(2, 2));
}
