#pragma once

#include "kernel/scene/GridNode.h"
#include <Inventor/nodes/SoSeparator.h>

class SoSensor;
class SoNodeSensor;
class SunPosition;
class SoTransform;


class SunNode3D: public SoSeparator
{
public:
    SunNode3D();
    ~SunNode3D();

    void attach(SunPosition* sp);
    void create();


    SoTransform* getTransform() {return m_transform;}

protected:
    SoTransform* m_transform;

    SoNodeSensor* m_sensor;
    static void update(void* data, SoSensor*);
};
