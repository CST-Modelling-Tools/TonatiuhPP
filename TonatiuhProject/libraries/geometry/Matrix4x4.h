#pragma once

#include <iostream>
#include "RefCount.h"
#include "Ptr.h"

class Matrix4x4: public RefCount
{
public:
    Matrix4x4();
    Matrix4x4(double array[4][4]);
    Matrix4x4(double t00, double t01, double t02, double t03,
              double t10, double t11, double t12, double t13,
              double t20, double t21, double t22, double t23,
              double t30, double t31, double t32, double t33);
    Matrix4x4(const Matrix4x4& rhs);
    ~Matrix4x4();
    Ptr<Matrix4x4> Transpose() const;
    Ptr<Matrix4x4> Inverse() const;

    bool operator==(const Matrix4x4& matrix) const;

    double m[4][4];

};

Ptr<Matrix4x4> Mul(const Ptr<Matrix4x4>& m1, const Ptr<Matrix4x4>& m2);
std::ostream& operator<<(std::ostream& os, const Matrix4x4& matrix);

