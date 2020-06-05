#include "ProfileBox.h"


SO_NODE_SOURCE(ProfileBox)

void ProfileBox::initClass()
{
     SO_NODE_INIT_CLASS(ProfileBox, ProfileRT, "Profile");
}

ProfileBox::ProfileBox()
{
    SO_NODE_CONSTRUCTOR(ProfileBox);
    SO_NODE_ADD_FIELD( uSize, (1.) );
    SO_NODE_ADD_FIELD( vSize, (1.) );
}

Box3D ProfileBox::getBox() const
{
    Vector3D v(uSize.getValue()/2., vSize.getValue()/2., 0.);
    return Box3D(-v, v);
}

bool ProfileBox::isInside(double u, double v) const
{
    return 2.*std::abs(u) <= uSize.getValue() &&
           2.*std::abs(v) <= vSize.getValue();
}

QVector<Vector2D> ProfileBox::makeMesh(const QSize& dims) const
{
    const int iMax = dims.width();
    const int jMax = dims.height();
    QVector<Vector2D> ans;

    for (int i = 0; i < iMax; ++i) {
        double un = i/double(iMax - 1);
        double u = (un - 0.5)*uSize.getValue();
        for (int j = 0; j < jMax; ++j) {
            double vn = j/double(jMax - 1);
            double v = (vn - 0.5)*vSize.getValue();
            ans << Vector2D(u, v);
        }
    }
    return ans;
}
