#include "Grid.h"
#include "math/gcf.h"


Grid::Grid(const Interval& interval, int divisions):
    Interval(interval)
{
    Q_ASSERT(divisions > 0);
    m_divisions = divisions;
    m_step = (m_b - m_a)/m_divisions;
}

Interval Grid::interval(int n) const
{
    double x = m_a + n*m_step;
    return Interval(x, x + m_step);
}

int Grid::index(double x) const
{
    int n = std::floor((x - m_a)/m_step);
    return gcf::clamp(n, 0, m_divisions - 1);
}

