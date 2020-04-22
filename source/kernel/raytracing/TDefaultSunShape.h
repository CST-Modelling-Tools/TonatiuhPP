#pragma once

#include "TSunShape.h"

class TDefaultSunShape: public TSunShape
{
    SO_NODE_HEADER(TDefaultSunShape);

public:
    TDefaultSunShape();
    static void initClass();
    SoNode* copy(SbBool copyConnections) const;

    //Sunshape Interface
    void GenerateRayDirection(Vector3D& direction, RandomDeviate& rand) const;
    double GetIrradiance() const;
    double GetThetaMax() const;

protected:
    ~TDefaultSunShape() {}
};
