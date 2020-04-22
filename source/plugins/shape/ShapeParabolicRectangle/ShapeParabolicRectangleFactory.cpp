#include <QIcon>
#include "ShapeParabolicRectangleFactory.h"


QString ShapeParabolicRectangleFactory::TShapeName() const
{
	return QString( "Parabolic_rectangle" );
}

QIcon ShapeParabolicRectangleFactory::TShapeIcon() const
{
    return QIcon(":/ShapeParabolicRectangle.png");
}

ShapeParabolicRectangle* ShapeParabolicRectangleFactory::CreateTShape() const
{
	static bool firstTime = true;
	if ( firstTime )
	{
	    ShapeParabolicRectangle::initClass();
	    firstTime = false;
	}

	return new ShapeParabolicRectangle;
}


