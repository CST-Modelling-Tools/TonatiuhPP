#pragma once

#include "libraries/TonatiuhLibraries.h"
#include <iostream>

struct Vector3D;

struct TONATIUH_LIBRARIES NormalVector
{
    NormalVector(double dx = 0.0, double dy = 0.0, double dz = 0.0);
    explicit NormalVector(const Vector3D& vector);
    ~NormalVector();

    NormalVector& operator+=(const NormalVector& nV);
    NormalVector& operator-=(const NormalVector& nV);

    NormalVector& operator*=(double scalar);
    NormalVector operator*(double scalar) const;
    NormalVector& operator/=(double scalar);
    NormalVector operator/(double scalar) const;

    NormalVector operator-() const;

    bool operator==(const NormalVector& nV) const;
    bool operator!=(const NormalVector& nV) const;
    double operator[](int i) const;
    double& operator[](int i);

    double lengthSquared() const;
    double length() const;

    double x;
    double y;
    double z;
};

TONATIUH_LIBRARIES NormalVector operator+(NormalVector lhs, const NormalVector& rhs);
TONATIUH_LIBRARIES NormalVector operator-(NormalVector lhs, const NormalVector& rhs);
TONATIUH_LIBRARIES NormalVector operator*(double scalar, const NormalVector& nV);
TONATIUH_LIBRARIES std::ostream& operator<<(std::ostream& os, const NormalVector& nV);
TONATIUH_LIBRARIES double DotProduct(const NormalVector& nA, const NormalVector& nB);
TONATIUH_LIBRARIES double AbsDotProduct(const NormalVector& nA, const NormalVector& nB);
TONATIUH_LIBRARIES NormalVector Normalize(const NormalVector& nV);


