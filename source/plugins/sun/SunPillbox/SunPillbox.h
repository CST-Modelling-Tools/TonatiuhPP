#pragma once

#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFFloat.h>

#include "kernel/raytracing/TSunShape.h"

class SoSensor;
class SoFieldSensor;

class SunPillbox: public TSunShape
{
    SO_NODE_HEADER(SunPillbox);

public:
    SunPillbox();
    static void initClass();
    SoNode* copy(SbBool copyConnections) const;

    void GenerateRayDirection(Vector3D& direction, RandomDeviate& rand) const;
	double GetIrradiance() const;
    double GetThetaMax() const;

    SoSFDouble irradiance;
    SoSFDouble thetaMax;

    static const char* getClassName() {return "Pillbox";}

protected:
    ~SunPillbox();
    static void updateTheta(void* data, SoSensor*);

private:
    SoFieldSensor* m_sensorTheta;
    double m_sinTheta;
};



#include "kernel/raytracing/TSunShapeFactory.h"

class SunPillboxFactory:
    public QObject, public SunFactory<SunPillbox>
{
    Q_OBJECT
    Q_INTERFACES(TSunShapeFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.TSunShapeFactory")
};
