#pragma once

#include "kernel/profiles/ProfileRT.h"


class TONATIUH_KERNEL ProfileRing: public ProfileRT
{
    SO_NODE_HEADER(ProfileRing);

public:
    static void initClass();
    ProfileRing();

    SoSFDouble rMin;
    SoSFDouble rMax;
    SoSFDouble phiMin;
    SoSFDouble phiMax;

    BoundingBox getBox() const;
    bool isInside(double u, double v) const;
    QVector<Vector2D> makeMesh(const QSize& dims) const;

    NAME_ICON_FUNCTIONS("Ring", ":/images/ProfileRing.png")

protected:
    ~ProfileRing() {}
};
