#pragma once

#include "libraries/TonatiuhLibraries.h"
#include "libraries/math/2D/vec2d.h"

#include <QPolygonF>

struct TONATIUH_LIBRARIES MeshDensity
{
    virtual double operator()(double u, double v) {
        Q_UNUSED(u)
        Q_UNUSED(v)
        return 1.;
    }
};

class TONATIUH_LIBRARIES PolygonMesh
{
public:
    struct Triangle {
        Triangle(int a, int b, int c): a(a), b(b), c(c) {}
        int a, b, c;
    };

    PolygonMesh(const QPolygonF& polygon);

    bool makeMesh(double step);
    bool makeMesh(double step, MeshDensity& mds);

    const QVector<vec2d>& getPoints() const {return m_points;}
    const QVector<Triangle>& getTriangles() const {return m_triangles;}

protected:
    QPolygonF m_polygon;
    QVector<vec2d> m_points;
    QVector<Triangle> m_triangles;
};
