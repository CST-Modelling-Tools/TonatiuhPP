#pragma once

#include "libraries/math/1D/Interval.h"
#include <QVector>


class TONATIUH_LIBRARIES Grid: public Interval
{
public:
    Grid(const Interval& interval = Interval(), int divisions = 1);

    // components
    int divisions() const {return m_divisions;}
    double step() const {return m_step;}

    // functions
    double point(int n) const {return m_a + n*m_step;}
    Interval interval(int n) const;
    int index(double x) const;

protected:
    int m_divisions;
    double m_step;
};
