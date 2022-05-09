#pragma once

#include "SunPathLib/math/matrices/Matrix.h"

namespace sp {

class SUNPATHLIB MatrixNR: public Matrix<double>
{
public:
    MatrixNR(int rows = 0, int columns = 0);

    QVector<double> multiply(const QVector<double>& vector);
    QVector<double> solve(const QVector<double>& vector);

    void invert();
};

} // namespace sp
