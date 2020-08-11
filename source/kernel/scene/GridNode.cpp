#include "GridNode.h"

SO_NODE_SOURCE(GridNode)


void GridNode::initClass()
{
    SO_NODE_INIT_CLASS(GridNode, SoNode, "Node");
}

GridNode::GridNode()
{
    SO_NODE_CONSTRUCTOR(GridNode);

    SO_NODE_ADD_FIELD(show, (TRUE) );
    SO_NODE_ADD_FIELD(step, (1.) );
    SO_NODE_ADD_FIELD(divisions, (5) );
    SO_NODE_ADD_FIELD(xRange, (SbVec2d(-10., 10.)) );
    SO_NODE_ADD_FIELD(yRange, (SbVec2d(-10., 10.)) );

    setName("Grid");
}
