#include <cmath>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/misc/SoState.h>
#include "TDefaultSunShape.h"

#include <cmath>
#include "geometry/tgc.h"
#include "geometry/tgf.h"
#include "Vector3D.h"

SO_NODE_SOURCE(TDefaultSunShape);

void TDefaultSunShape::initClass()
{
    SO_NODE_INIT_CLASS(TDefaultSunShape, TSunShape, "TSunShape");
}

TDefaultSunShape::TDefaultSunShape( )
{
    SO_NODE_CONSTRUCTOR(TDefaultSunShape);
}


//Light Interface
void TDefaultSunShape::GenerateRayDirection( Vector3D& /* direction */, RandomDeviate& /* rand */ ) const
{

}

double TDefaultSunShape::GetIrradiance() const
{
    return -1;
}

double TDefaultSunShape::GetThetaMax() const
{
    return 0;
}

SoNode* TDefaultSunShape::copy( SbBool copyConnections ) const
{
    // Use the standard version of the copy method to create
    // a copy of this instance, including its field data
    TDefaultSunShape* newSunShape = static_cast< TDefaultSunShape* >( SoNode::copy( copyConnections ) );

    return newSunShape;
}
