#pragma once

#include "SunPathLib/calculators/SunCalculator.h"

namespace sp {

class SUNPATHLIB SunCalculatorNREL: public SunCalculator
{
public:
    SunCalculatorNREL();
    SunCalculatorNREL* copy() const;

    Horizontal findHorizontalV(const QDateTime& t) const;

    QString info() const {return "NREL (2003)";}
};

} // namespace sp
