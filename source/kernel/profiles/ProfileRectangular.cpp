#include "ProfileRectangular.h"


SO_NODE_SOURCE(ProfileRectangular)

void ProfileRectangular::initClass()
{
     SO_NODE_INIT_CLASS(ProfileRectangular, ProfileRT, "Profile");
}

ProfileRectangular::ProfileRectangular()
{
    SO_NODE_CONSTRUCTOR(ProfileRectangular);
    SO_NODE_ADD_FIELD( uMin, (-0.5) );
    SO_NODE_ADD_FIELD( uMax, (0.5) );
    SO_NODE_ADD_FIELD( vMin, (-0.5) );
    SO_NODE_ADD_FIELD( vMax, (0.5) );
}

Box3D ProfileRectangular::getBox() const
{
    return Box3D(
        Vector3D(uMin.getValue(), vMin.getValue(), 0.),
        Vector3D(uMax.getValue(), vMax.getValue(), 0.)
    );
}

bool ProfileRectangular::isInside(double u, double v) const
{
    return uMin.getValue() <= u && u <= uMax.getValue() &&
           vMin.getValue() <= v && v <= vMax.getValue();
}

QVector<Vector2D> ProfileRectangular::makeMesh(const QSize& dims) const
{
    const int iMax = dims.width();
    const int jMax = dims.height();
    QVector<Vector2D> ans;

    for (int i = 0; i < iMax; ++i) {
        double un = i/double(iMax - 1);
        double u = (1. - un)*uMin.getValue() + un*uMax.getValue();
        for (int j = 0; j < jMax; ++j) {
            double vn = j/double(jMax - 1);
            double v = (1. - vn)*vMin.getValue() + vn*vMax.getValue();
            ans << Vector2D(u, v);
        }
    }
    return ans;
}
