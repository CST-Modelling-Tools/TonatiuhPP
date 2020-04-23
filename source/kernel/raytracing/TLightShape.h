#pragma once

#include <vector>

#include <QPair>

#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>

#include "TShape.h"
#include "trt.h"

class Transform;

class TLightShape : public SoShape
{
    SO_NODE_HEADER(TLightShape);

public:
    TLightShape( );
    static void initClass();

    double GetValidArea() const;
    std::vector< QPair< int, int > > GetValidAreasCoord() const;
    double GetVolume() const {return 0.;}

    Point3D Sample(double u, double v, int a, int b) const {
        return GetPoint3D(u, v, a, b);
    }

    void SetLightSourceArea(int h, int w, int** lightArea);

    trt::TONATIUH_REAL xMin;
    trt::TONATIUH_REAL xMax;
    trt::TONATIUH_REAL zMin;
    trt::TONATIUH_REAL zMax;
    trt::TONATIUH_REAL delta;

protected:
    Point3D GetPoint3D (double u, double v, int h, int w) const;
    bool OutOfRange(double u, double v) const
    {
        return u < 0. || u > 1. || v < 0. || v > 1.;
    }

    void generatePrimitives(SoAction *action);
    void computeBBox(SoAction *action, SbBox3f &box, SbVec3f &center);
    ~TLightShape();

private:
    int m_heightElements;
    int** m_lightAreaMatrix;
    int m_widthElements;
    std::vector< QPair< int, int > > m_validAreasVector;
};
