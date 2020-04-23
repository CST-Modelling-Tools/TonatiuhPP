#pragma once

#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFFloat.h>

#include "kernel/raytracing/TSunShape.h"
#include "kernel/raytracing/trt.h"


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

	trt::TONATIUH_REAL irradiance;
	trt::TONATIUH_REAL thetaMax;

    static const char* getClassName() {return "Pillbox";}

protected:
    ~SunPillbox() {}
};



#include "kernel/raytracing/TSunShapeFactory.h"

class SunPillboxFactory: public QObject, public SunFactory<SunPillbox>
{
    Q_OBJECT
    Q_INTERFACES(TSunShapeFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.TSunShapeFactory")
};
