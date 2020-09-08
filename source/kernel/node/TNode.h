#pragma once

#include "kernel/TonatiuhKernel.h"
#include "kernel/node/TonatiuhTypes.h"

class SoSensor;
class SoFieldSensor;
class SoNodeSensor;

#define NAME_ICON_FUNCTIONS(NAME, ICON) \
static const char* getClassName() {return NAME;} \
static const char* getClassIcon() {return ICON;} \
virtual const char* getTypeName() const {return getClassName();} \
virtual const char* getTypeIcon() const {return getClassIcon();}

#include <Inventor/nodes/SoSubNode.h>


class TONATIUH_KERNEL TNode: public SoNode
{
    SO_NODE_ABSTRACT_HEADER(TNode);

public:
    static void initClass();

    NAME_ICON_FUNCTIONS("X", ":/X.png")

protected:
    TNode() {}
    ~TNode() {}
};
