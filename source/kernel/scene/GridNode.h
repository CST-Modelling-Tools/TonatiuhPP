#pragma once
#include "kernel/TonatiuhKernel.h"

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFVec2d.h>
#include <Inventor/fields/SoSFVec2d.h>

class SoSensor;
class SoNodeSensor;

class TONATIUH_KERNEL GridNode: public SoNode
{
    SO_NODE_HEADER(LocationNode);

public:
    static void initClass();
    GridNode();
    ~GridNode();

    SoSFBool show;
    SoSFDouble step;
    SoSFInt32 divisions;
    SoSFVec2d xRange;
    SoSFVec2d yRange;

    SoGroup* getRoot() const {return m_root;}
    void create();

protected:
    SoSwitch* m_root;
    SoSeparator* makeAxes(double xMin, double xMax, double yMin, double yMax);

    SoNodeSensor* m_sensor;
    static void update(void* data, SoSensor*);
};

