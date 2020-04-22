#include "TMaterial.h"

SO_NODE_ABSTRACT_SOURCE(TMaterial);

void TMaterial::initClass()
{
    SO_NODE_INIT_ABSTRACT_CLASS(TMaterial, SoMaterial, "Material");

}

TMaterial::TMaterial()
{
    //SO_NODE_CONSTRUCTOR( TMaterial );

}

TMaterial::~TMaterial()
{
}
