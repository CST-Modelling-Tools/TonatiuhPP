#pragma once

#include "kernel/TonatiuhKernel.h"
#include <vector>
#include <QPair>

#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFString.h>

#include "kernel/shape/ShapeRT.h"
#include "kernel/TonatiuhTypes.h"
#include "libraries/geometry/Vector3D.h"

class Transform;
class TShapeKit;
class SunKit;

class TONATIUH_KERNEL SunAperture: public SoShape
{
    SO_NODE_HEADER(SunAperture);

public:
    static void initClass();
    SunAperture();

    double getArea() const;
    const std::vector< QPair<int, int> >& getCells() const {return m_cells;}

    Vector3D Sample(double u, double v, int w, int h) const;

    void setSize(double xMin, double xMax, double yMin, double yMax, double delta);
    void findTexture(int widthDivisions, int heightDivisions, QVector< QPair<TShapeKit*, Transform> > surfaces, SunKit* sunKit);

    SoSFString disabledNodes;

protected:
    ~SunAperture();
    void SetLightSourceArea(int w, int h, QVector<uchar> bmp);
    void computeBBox(SoAction* action, SbBox3f& box, SbVec3f& center);
    void generatePrimitives(SoAction* action);

private:
    double m_xMin;
    double m_xMax;
    double m_yMin;
    double m_yMax;
    double m_delta;

    int m_xCells;
    int m_yCells;
    std::vector< QPair<int, int> > m_cells;
};
