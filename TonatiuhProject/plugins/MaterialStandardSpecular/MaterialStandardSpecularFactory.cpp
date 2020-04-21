#include <QIcon>

#include "MaterialStandardSpecularFactory.h"

QString MaterialStandardSpecularFactory::TMaterialName() const
{
	return QString("Specular_Standard_Material");
}

QIcon MaterialStandardSpecularFactory::TMaterialIcon() const
{
    return QIcon(":/MaterialStandardSpecular.png");
}

MaterialStandardSpecular* MaterialStandardSpecularFactory::CreateTMaterial( ) const
{
	static bool firstTime = true;
    if (firstTime)
	{
	    MaterialStandardSpecular::initClass();
	    firstTime = false;
	}

	return new MaterialStandardSpecular;
}
