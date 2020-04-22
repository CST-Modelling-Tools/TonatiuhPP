#include <QIcon>
#include "ShapeCylinder.h"
#include "ShapeCylinderFactory.h"


QString ShapeCylinderFactory::TShapeName() const
{
	return QString("Cylinder");
}

QIcon ShapeCylinderFactory::TShapeIcon() const
{
    return QIcon(":/ShapeCylinder.png");
}

ShapeCylinder* ShapeCylinderFactory::CreateTShape( ) const
{
	static bool firstTime = true;
	if ( firstTime )
	{
	    ShapeCylinder::initClass();
	    firstTime = false;
	}
	return new ShapeCylinder;
}
