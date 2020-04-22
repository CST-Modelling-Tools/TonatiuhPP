#pragma once

#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFFloat.h>

#include "kernel/raytracing/TSunShape.h"
#include "kernel/raytracing/trt.h"


class SunshapePillbox : public TSunShape
{
	SO_NODE_HEADER(SunshapePillbox);

public:
    SunshapePillbox( );
    static void initClass();
	SoNode* copy( SbBool copyConnections ) const;

    //Sunshape Interface
    void GenerateRayDirection( Vector3D& direction, RandomDeviate& rand) const;
	double GetIrradiance() const;
    double GetThetaMax() const;

	trt::TONATIUH_REAL irradiance;
	trt::TONATIUH_REAL thetaMax;

protected:
	 ~SunshapePillbox();
};
