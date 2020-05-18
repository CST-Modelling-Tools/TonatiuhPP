#pragma once

#include "libraries/TonatiuhLibraries.h"
#include <memory>
#include <iostream>


class TONATIUH_LIBRARIES Matrix4x4
{
public:
    Matrix4x4();
    Matrix4x4(double t00, double t01, double t02, double t03,
              double t10, double t11, double t12, double t13,
              double t20, double t21, double t22, double t23,
              double t30, double t31, double t32, double t33);
    Matrix4x4(double array[4][4]);
    Matrix4x4(const Matrix4x4& rhs);

    std::shared_ptr<Matrix4x4> Transpose() const;
    std::shared_ptr<Matrix4x4> Inverse() const;

    bool operator==(const Matrix4x4& matrix) const;

    double m[4][4];
};

TONATIUH_LIBRARIES std::shared_ptr<Matrix4x4> multiply(const Matrix4x4& m1, const Matrix4x4& m2);
TONATIUH_LIBRARIES std::ostream& operator<<(std::ostream& os, const Matrix4x4& matrix);
