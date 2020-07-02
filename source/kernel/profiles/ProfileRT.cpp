#include "ProfileRT.h"

SO_NODE_ABSTRACT_SOURCE(ProfileRT)


void ProfileRT::initClass()
{
    SO_NODE_INIT_ABSTRACT_CLASS(ProfileRT, SoNode, "Node");
}

vec2d ProfileRT::getAbsMin(const Box2D& b)
{
    const vec2d& vA = b.min();
    const vec2d& vB = b.max();
    vec2d ans = vec2d::min(vA.abs(), vB.abs());
    if (vA.x <= 0. && 0. <= vB.x) ans.x = 0.;
    if (vA.y <= 0. && 0. <= vB.y) ans.y = 0.;
    return ans;
}

vec2d ProfileRT::getAbsMax(const Box2D& b)
{
    return vec2d::max(b.min().abs(), b.max().abs());
}
