#include "SunAbstract.h"

SO_NODE_ABSTRACT_SOURCE(SunAbstract)


void SunAbstract::initClass()
{
    SO_NODE_INIT_ABSTRACT_CLASS(SunAbstract, SoNode, "Node");
}
