#pragma once

#include "libraries/TonatiuhLibraries.h"
#include "libraries/math/2D/vec2d.h"

#include <QPolygonF>


class TONATIUH_LIBRARIES PolygonMesh
{
public:
    struct Triangle {
        Triangle(int a, int b, int c): a(a), b(b), c(c) {}
        int a, b, c;
    };

    PolygonMesh(const QPolygonF& polygon);

    bool makeMesh(double step);

    const QVector<vec2d>& getPoints() const {return m_points;}
    const QVector<Triangle>& getTriangles() const {return m_triangles;}

protected:
    QPolygonF m_polygon;
    QVector<vec2d> m_points;
    QVector<Triangle> m_triangles;
};
