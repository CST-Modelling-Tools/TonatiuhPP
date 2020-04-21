#include "TShape.h"

SO_NODE_ABSTRACT_SOURCE(TShape);

void TShape::initClass()
{
    SO_NODE_INIT_ABSTRACT_CLASS(TShape, SoShape, "Shape");
}

TShape::TShape()
{

}

TShape::~TShape()
{

}
