#pragma once

#include "kernel/scene/GridNode.h"
#include <Inventor/nodes/SoSwitch.h>


class GridNode3D
{
public:
    GridNode3D();
    ~GridNode3D();

    void attach(GridNode* grid);
    SoGroup* getRoot() const {return m_root;}
    void create();

protected:
    SoSwitch* m_root;
    SoSeparator* makeAxes(double xMin, double xMax, double yMin, double yMax);

    SoNodeSensor* m_sensor;
    static void update(void* data, SoSensor*);
};

