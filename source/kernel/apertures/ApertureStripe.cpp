#include "ApertureStripe.h"


SO_NODE_SOURCE(ApertureStripe)

void ApertureStripe::initClass()
{
     SO_NODE_INIT_CLASS(ApertureStripe, Aperture, "Aperture");
}

ApertureStripe::ApertureStripe()
{
    SO_NODE_CONSTRUCTOR(ApertureStripe);
    SO_NODE_ADD_FIELD( uMin, (-0.5) );
    SO_NODE_ADD_FIELD( uMax, (0.5) );
    SO_NODE_ADD_FIELD( vMin, (-0.5) );
    SO_NODE_ADD_FIELD( vMax, (0.5) );
}

BoundingBox ApertureStripe::getBox() const
{
    return BoundingBox(
        Vector3D(uMin.getValue(), vMin.getValue(), 0.),
        Vector3D(uMax.getValue(), vMax.getValue(), 0.)
    );
}

bool ApertureStripe::isInside(double u, double v) const
{
    return uMin.getValue() <= u && u <= uMax.getValue() &&
           vMin.getValue() <= v && v <= vMax.getValue();
}

QVector<Vector2D> ApertureStripe::makeMesh(const QSize& dims) const
{
    const int iMax = dims.width();
    const int jMax = dims.height();
    QVector<Vector2D> ans;

    for (int i = 0; i < iMax; ++i) {
        double un = i/double(iMax - 1);
        double u = (1. - un)*uMin.getValue() + un*uMax.getValue();
        for (int j = 0; j < jMax; ++j) {
            double vn = j/double(jMax - 1);
            double v = (1. - vn)*uMin.getValue() + vn*vMax.getValue();
            ans << Vector2D(u, v);
        }
    }
    return ans;
}
