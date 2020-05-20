#pragma once

#include "kernel/TonatiuhKernel.h"
#include <vector>
#include <QPair>

#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>

#include "kernel/shape/ShapeAbstract.h"
#include "kernel/TonatiuhTypes.h"
#include "libraries/geometry/Vector3D.h"

class Transform;


class TONATIUH_KERNEL SunAperture: public SoShape
{
    SO_NODE_HEADER(SunAperture);

public:
    static void initClass();
    SunAperture();

    double getArea() const;
    const std::vector< QPair<int, int> >& getCells() const {return m_cells;}

    Vector3D Sample(double u, double v, int h, int w) const;

    void SetLightSourceArea(int h, int w, int** lightArea);

    trt::TONATIUH_REAL xMin;
    trt::TONATIUH_REAL xMax;
    trt::TONATIUH_REAL yMin;
    trt::TONATIUH_REAL yMax;
    trt::TONATIUH_REAL delta;

protected:
    bool OutOfRange(double u, double v) const
    {
        return u < 0. || u > 1. || v < 0. || v > 1.;
    }

    void generatePrimitives(SoAction* action);
    void computeBBox(SoAction* action, SbBox3f& box, SbVec3f& center);
    ~SunAperture();

private:
    int m_heightElements;
    int** m_lightAreaMatrix;
    int m_widthElements;
    std::vector< QPair<int, int> > m_cells;
};
