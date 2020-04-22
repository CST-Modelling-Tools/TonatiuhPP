#include <QIcon>
#include "SunshapePillboxFactory.h"


QString SunshapePillboxFactory::TSunShapeName() const
{
    return QString("Pillbox_Sunshape");
}

QIcon SunshapePillboxFactory::TSunShapeIcon() const
{
    return QIcon(":/SunshapePillbox.png");
}

SunshapePillbox* SunshapePillboxFactory::CreateTSunShape() const
{
	static bool firstTimeSunShape = true;
    if (firstTimeSunShape)
	{
        SunshapePillbox::initClass();
        firstTimeSunShape = false;
	}

	SunshapePillbox* pill = new SunshapePillbox;
	return pill;
}
