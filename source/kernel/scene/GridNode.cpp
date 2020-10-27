#include "GridNode.h"

SO_NODE_SOURCE(GridNode)


void GridNode::initClass()
{
    SO_NODE_INIT_CLASS(GridNode, SoNode, "Node");
}

GridNode::GridNode()
{
    SO_NODE_CONSTRUCTOR(GridNode);
    isBuiltIn = TRUE;

    SO_NODE_ADD_FIELD(grid, (TRUE) );
    SO_NODE_ADD_FIELD(fill, (TRUE) );

    SO_NODE_ADD_FIELD(steps, ( SbVec3d(1., 1., 1.) ));
    SO_NODE_ADD_FIELD(divisions, ( SbVec3i32(5, 5, 5) ));
    SO_NODE_ADD_FIELD(min, ( SbVec3d(-10., -10., 0.) ));
    SO_NODE_ADD_FIELD(max, ( SbVec3d(10., 10., 0.) ));

    SO_NODE_ADD_FIELD(file, ("") );

    setName("Grid");
}
