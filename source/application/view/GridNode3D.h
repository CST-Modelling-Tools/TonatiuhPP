#pragma once

#include "kernel/scene/GridNode.h"
#include <Inventor/nodes/SoSwitch.h>

class SoSensor;
class SoNodeSensor;


class GridNode3D: public SoSwitch
{
public:
    GridNode3D();
    ~GridNode3D();

    void attach(GridNode* grid);
    void create();

protected:
    SoSeparator* makeAxes(double xMin, double xMax, double yMin, double yMax);

    SoNodeSensor* m_sensor;
    static void update(void* data, SoSensor*);
};

