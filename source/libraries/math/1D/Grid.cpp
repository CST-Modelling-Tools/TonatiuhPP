#include "Grid.h"

Grid::Grid(const Interval& interval, int divisions):
    m_interval(interval)
{
    setDivisions(divisions);
}

void Grid::setDivisions(int divisions)
{
    Q_ASSERT(divisions > 0);
    m_divisions = divisions;
    m_step = m_interval.size()/m_divisions;
}
