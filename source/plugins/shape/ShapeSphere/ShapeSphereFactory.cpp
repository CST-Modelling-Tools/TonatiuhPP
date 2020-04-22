#include <QIcon>
#include "ShapeSphereFactory.h"


QString ShapeSphereFactory::TShapeName() const
{
	return QString("Sphere");
}

QIcon ShapeSphereFactory::TShapeIcon() const
{
    return QIcon(":/ShapeSphere.png");
}

ShapeSphere* ShapeSphereFactory::CreateTShape() const
{
	static bool firstTime = true;
    if (firstTime)
	{
        ShapeSphere::initClass();
        firstTime = false;
	}

	return new ShapeSphere;
}
