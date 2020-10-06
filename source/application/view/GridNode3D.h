#pragma once

#include "kernel/scene/GridNode.h"
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoSeparator.h>

class SoSensor;
class SoNodeSensor;


class GridNode3D: public SoSeparator
{
public:
    GridNode3D();
    ~GridNode3D();

    void attach(GridNode* grid);
    void create();

protected:
    void makeAxes(double xMin, double xMax, double yMin, double yMax);
    SoSwitch* m_grid;

    SoNodeSensor* m_sensor;
    static void update(void* data, SoSensor*);

    // SoSeparator interface
//protected:
//    SbBool cullTest(SoState* state) {return 0;}
//    SbBool cullTest(SoGLRenderAction* action, int& cullresults) {return 0;}
};

