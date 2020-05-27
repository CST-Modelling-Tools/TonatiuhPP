#include "Aperture.h"

SO_NODE_ABSTRACT_SOURCE(Aperture)


void Aperture::initClass()
{
    SO_NODE_INIT_ABSTRACT_CLASS(Aperture, SoNode, "Node");
}
