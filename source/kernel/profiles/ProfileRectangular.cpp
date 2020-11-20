#include "ProfileRectangular.h"

#include "libraries/math/gcf.h"


SO_NODE_SOURCE(ProfileRectangular)

void ProfileRectangular::initClass()
{
     SO_NODE_INIT_CLASS(ProfileRectangular, ProfileRT, "Profile");
}

ProfileRectangular::ProfileRectangular()
{
    SO_NODE_CONSTRUCTOR(ProfileRectangular);
    isBuiltIn = TRUE;
    SO_NODE_ADD_FIELD( uMin, (-0.5) );
    SO_NODE_ADD_FIELD( uMax, (0.5) );
    SO_NODE_ADD_FIELD( vMin, (-0.5) );
    SO_NODE_ADD_FIELD( vMax, (0.5) );
}

Box2D ProfileRectangular::getBox() const
{
    return Box2D(
        vec2d(uMin.getValue(), vMin.getValue()),
        vec2d(uMax.getValue(), vMax.getValue())
    );
}

bool ProfileRectangular::isInside(double u, double v) const
{
    return uMin.getValue() <= u && u <= uMax.getValue() &&
           vMin.getValue() <= v && v <= vMax.getValue();
}

QVector<vec2d> ProfileRectangular::makeMesh(QSize& dims) const
{
    const int iMax = dims.width();
    const int jMax = dims.height();

    QVector<vec2d> ans;
    for (int i = 0; i < iMax; ++i) {
        double u = gcf::lerp(uMin.getValue(), uMax.getValue(), i/double(iMax - 1));
        for (int j = 0; j < jMax; ++j) {
            double v = gcf::lerp(vMin.getValue(), vMax.getValue(), j/double(jMax - 1));
            ans << vec2d(u, v);
        }
    }
    return ans;
}
