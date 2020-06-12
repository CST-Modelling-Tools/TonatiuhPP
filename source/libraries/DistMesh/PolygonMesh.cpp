#include "PolygonMesh.h"

#include "libraries/DistMesh/distmesh.h"


PolygonMesh::PolygonMesh(const QPolygonF& polygon)
{
    m_polygon = polygon;
}

bool PolygonMesh::makeMesh(double step)
{
    Eigen::ArrayXXd polygon(m_polygon.size(), 2);
    for (int n = 0; n < m_polygon.size(); ++n) {
        polygon(n, 0) = m_polygon[n].x();
        polygon(n, 1) = m_polygon[n].y();
    }

    Eigen::ArrayXXd points;
    Eigen::ArrayXXi elements;
    std::tie(points, elements) = distmesh::distmesh(
        distmesh::distanceFunction::polygon(polygon),
        step,
        1.0,
        distmesh::utils::boundingBox(2),
        polygon);

    m_points.clear();
    for (int n = 0; n < points.rows(); ++n)
        m_points << Vector2D(points(n, 0), points(n, 1));

    m_triangles.clear();
    for (int n = 0; n < elements.rows(); ++n)
        m_triangles << Triangle(elements(n, 0), elements(n, 1), elements(n, 2));
}
