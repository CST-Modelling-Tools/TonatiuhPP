#pragma once

#include <iostream>

struct Point3D;
struct NormalVector;
#include "NormalVector.h"
#include "Point3D.h"
#include <cmath>

struct Vector3D
{
    Vector3D( double dx = 0.0, double dy = 0.0, double dz = 0.0 )
        : x(dx), y(dy), z(dz)
    {
    }
    Vector3D( const NormalVector& norm )
        : x(norm.x), y(norm.y), z(norm.z)
    {
    }

    explicit Vector3D( const Point3D& point )
        : x(point.x), y(point.y), z(point.z)
    {
    }

    Vector3D& operator+=( const Vector3D& vector )
    {
        x += vector.x;
        y += vector.y;
        z += vector.z;
        return *this;
    }

    Vector3D& operator-=( const Vector3D& vector )
{
    x -= vector.x;
    y -= vector.y;
    z -= vector.z;
    return *this;
}
    Vector3D& operator*=( double scalar )
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vector3D operator*( double scalar ) const
    {
        return Vector3D( x * scalar, y * scalar, z * scalar );
    }

    Vector3D& operator/=( double scalar )
    {
        double inv = 1.0/scalar;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }

    Vector3D operator/( double scalar ) const
    {
        double inv = 1.0/scalar;
        return Vector3D( x * inv, y * inv, z * inv );
    }

    Vector3D operator-() const
    {
        return Vector3D( -x, -y, -z );
    }

    bool operator==( const Vector3D& vector ) const;
    bool operator!=( const Vector3D& vector ) const;
    double operator[]( int i ) const
    {
        if( i == 0 ) return x;
        if( i == 1 ) return y;
        return z;
    }

    double& operator[]( int i )
    {
        if( i == 0 ) return x;
        if( i == 1 ) return y;
        return z;
    }


    void zero();
    double lengthSquared() const
    {
        return x*x + y*y + z*z;
    }

    double length() const
    {
        return std::sqrt(x*x + y*y + z*z);
    }

    double x;
    double y;
    double z;
};

inline Vector3D operator+( Vector3D lhs, const Vector3D& rhs )
{
    //lhs take by value to let the compile to make the copy
    return lhs += rhs;
}
inline Vector3D operator-( Vector3D lhs, const Vector3D& rhs )
{
    //lhs take by value to let the compile to make the copy
    return lhs -= rhs;
}
inline Vector3D operator*( double scalar, const Vector3D& vector )
{
    return Vector3D( scalar * vector.x, scalar * vector.y, scalar * vector.z );
}
std::ostream& operator<<( std::ostream& os, const Vector3D& vector );

inline double DotProduct( const Vector3D& vA, const Vector3D& vB )
{
    return vA.x*vB.x + vA.y*vB.y + vA.z*vB.z;
}

inline double DotProduct( const Vector3D& vA, const NormalVector& nB )
{
    return vA.x*nB.x + vA.y*nB.y + vA.z*nB.z;
}

inline double DotProduct( const NormalVector& nA, const Vector3D& vB )
{
    return nA.x*vB.x + nA.y*vB.y + nA.z*vB.z;
}

double AbsDotProduct( const Vector3D& vA, const Vector3D& vB );
double AbsDotProduct( const Vector3D& vA, const NormalVector& nB );
double AbsDotProduct( const NormalVector& nA, const Vector3D& vB );

inline Vector3D CrossProduct( const Vector3D& vA, const Vector3D& vB )
{
    return Vector3D( ( vA.y*vB.z ) - ( vA.z*vB.y ),
                     ( vA.z*vB.x ) - ( vA.x*vB.z ),
                     ( vA.x*vB.y ) - ( vA.y*vB.x ) );
}

inline Vector3D CrossProduct( const Vector3D& vA, const NormalVector& nB )
{
    return Vector3D( ( vA.y*nB.z ) - ( vA.z*nB.y ),
                     ( vA.z*nB.x ) - ( vA.x*nB.z ),
                     ( vA.x*nB.y ) - ( vA.y*nB.x ) );
}

inline Vector3D CrossProduct( const NormalVector& nA, const Vector3D& vB )
{
    return Vector3D( ( nA.y*vB.z ) - ( nA.z*vB.y ),
                     ( nA.z*vB.x ) - ( nA.x*vB.z ),
                     ( nA.x*vB.y ) - ( nA.y*vB.x ) );
}

inline Vector3D Normalize( const Vector3D& vA )
{
    if (vA.length() > 0.0f) return vA / vA.length();
    return vA;
}

bool SameHemisphere( const Vector3D& vA, const Vector3D& vB );
