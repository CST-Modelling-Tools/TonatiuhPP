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

protected:
    SoSwitch* m_grid;
    void create();
    void makeAxes(double xMin, double xMax, double yMin, double yMax, double zMean);
    void makeGround(double xMin, double xMax, double yMin, double yMax, double zMean);
    void makeTerrain(QString fileName);

    SoNodeSensor* m_sensor;
    static void update(void* data, SoSensor*);
};

