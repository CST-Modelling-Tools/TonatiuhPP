#pragma once

#include "Point3D.h"

class Ray;

struct BBox
{
    BBox();
    explicit BBox(const Point3D& point);
    BBox(const Point3D& point1, const Point3D& point2);
    bool Overlaps(const BBox& bbox) const;
    bool Inside(const Point3D& point) const;
    void Expand(double delta);
    double Volume() const;
    int MaximumExtent() const;
    void BoundingSphere(Point3D& center, double& radius) const;
    bool IntersectP(const Ray& ray, double* hitt0 = NULL, double* hitt1 = NULL) const;

	Point3D pMin;
	Point3D pMax;
};

BBox Union(const BBox& bbox, const Point3D& point);
BBox Union(const BBox& bbox1, const BBox& bbox2);
std::ostream& operator<<(std::ostream& os, const BBox& bbox);
