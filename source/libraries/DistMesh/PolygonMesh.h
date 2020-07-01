#pragma once

#include "libraries/TonatiuhLibraries.h"
#include "libraries/math/Vector2D.h"

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

    const QVector<Vector2D>& getPoints() const {return m_points;}
    const QVector<Triangle>& getTriangles() const {return m_triangles;}

protected:
    QPolygonF m_polygon;
    QVector<Vector2D> m_points;
    QVector<Triangle> m_triangles;
};
