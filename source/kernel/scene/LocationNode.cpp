#include "LocationNode.h"


SO_NODE_SOURCE(LocationNode)


void LocationNode::initClass()
{
    SO_NODE_INIT_CLASS(LocationNode, SoNode, "Node");
}

LocationNode::LocationNode()
{
    SO_NODE_CONSTRUCTOR(LocationNode);
    SO_NODE_ADD_FIELD(name, ("") );
    SO_NODE_ADD_FIELD(longitude, (0.) );
    SO_NODE_ADD_FIELD(latitude, (0.) );

    setName("Location");
}
