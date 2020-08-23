#include "TNode.h"

SO_NODE_ABSTRACT_SOURCE(TNode)


void TNode::initClass()
{
    SO_NODE_INIT_ABSTRACT_CLASS(TNode, SoNode, "Node");
}
