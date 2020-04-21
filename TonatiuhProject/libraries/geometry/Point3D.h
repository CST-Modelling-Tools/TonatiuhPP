#pragma once


#include <iostream>
struct Vector3D;

struct Point3D
{
    Point3D(double dx = 0.0, double dy = 0.0, double dz = 0.0);
    explicit Point3D (const Vector3D& vector);
    ~Point3D();

    Point3D& operator+=(const Vector3D& vector);
    Point3D operator+(const Vector3D& vector) const;
    Point3D& operator-=(const Vector3D& vector);
    Point3D operator-(const Vector3D& vector) const;
    Vector3D operator-(const Point3D& point) const;

    Point3D& operator*=(double scalar);
    Point3D operator*(double scalar) const;
    Point3D& operator/=(double scalar);
    Point3D operator/(double scalar) const;

    bool operator==(const Point3D& point) const;
    bool operator!=(const Point3D& point) const;
    double operator[](int i) const;
    double& operator[](int i);

    double x;
    double y;
    double z;
};

Point3D operator*(double scalar, const Point3D& point);
std::ostream& operator<<(std::ostream& os, const Point3D& point);
double Distance(const Point3D& pointA, const Point3D& pointB);
double DistanceSquared(const Point3D& pointA, const Point3D& pointB);

