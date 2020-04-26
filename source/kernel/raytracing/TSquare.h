#pragma once

#include "kernel/TonatiuhKernel.h"
#include <Inventor/fields/SoSFDouble.h>

#include "TShape.h"

class Transform;

class TONATIUH_KERNEL TSquare : public TShape
{
    SO_NODE_HEADER(TSquare);
public:
    TSquare( );
    static void initClass();
    double GetArea() const;
    double GetVolume() const {return 0.0;}
    BBox GetBBox() const;
    QString GetIcon() const;

    bool Intersect(const Ray &ray, double *tHit, DifferentialGeometry *dg ) const;
    bool IntersectP( const Ray &ray ) const;
    Point3D Sample( double u, double v ) const;

    SoSFDouble m_sideLength;

protected:
    Point3D GetPoint3D (double u, double v) const;
    NormalVector GetNormal(double u, double v) const;

    void computeBBox(SoAction *action, SbBox3f &box, SbVec3f &center);
    void generatePrimitives( SoAction *action );
    ~TSquare();

};
