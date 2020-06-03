#include "ProfileRing.h"

#include "libraries/geometry/gcf.h"


SO_NODE_SOURCE(ProfileRing)

void ProfileRing::initClass()
{
     SO_NODE_INIT_CLASS(ProfileRing, ProfileRT, "Profile");
}

ProfileRing::ProfileRing()
{
    SO_NODE_CONSTRUCTOR(ProfileRing);
    SO_NODE_ADD_FIELD( rMin, (0.) );
    SO_NODE_ADD_FIELD( rMax, (1.) );
    SO_NODE_ADD_FIELD( phiMin, (-gcf::pi) );
    SO_NODE_ADD_FIELD( phiMax, (gcf::pi) );
}

BoundingBox ProfileRing::getBox() const
{
    return BoundingBox::UnitCube;
}

bool ProfileRing::isInside(double x, double y) const
{
    return 2.*std::abs(x) <= rMin.getValue() &&
           2.*std::abs(y) <= rMax.getValue();
}

QVector<Vector2D> ProfileRing::makeMesh(const QSize& dims) const
{
    QVector<Vector2D> ans;
    return ans;
}
