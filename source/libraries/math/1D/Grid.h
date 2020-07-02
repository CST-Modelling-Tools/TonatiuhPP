#pragma once

#include "libraries/math/1D/Interval.h"
#include <QVector>


class TONATIUH_LIBRARIES Grid
{
public:
    Grid(const Interval& interval = Interval(), int divisions = 1);
    void setDivisions(int divisions);

    // components
    const Interval& interval() const {return m_interval;}
    int divisions() const {return m_divisions;}
    double step() const {return m_step;}

    // functions  
    double toNormalized(double x) const {return (x - m_interval.min())/m_step;}
    double fromNormalized(double u) const {return m_interval.min() + u*m_step;}

protected:
    Interval m_interval;
    int m_divisions;
    double m_step;
};
