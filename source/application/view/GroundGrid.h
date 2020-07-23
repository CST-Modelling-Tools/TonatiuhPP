#pragma once

#include <Inventor/nodes/SoSubNode.h>

class SoGroup;


class GroundGrid
{
public:
    GroundGrid();

    SoGroup* makeGrid(
        double step = 1., int divs = 5,
        double xMin = -10., double xMax = 10.,
        double yMin = -10., double yMax = 10.
    );

protected:
    SoGroup* makeAxes(double xMin, double xMax, double yMin, double yMax);
};

