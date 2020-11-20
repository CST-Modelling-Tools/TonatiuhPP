#pragma once

#include "kernel/profiles/ProfileRT.h"


class TONATIUH_KERNEL ProfileCircular: public ProfileRT
{
    SO_NODE_HEADER(ProfileCircular);

public:
    static void initClass();
    ProfileCircular();

    SoSFDouble rMin;
    SoSFDouble rMax;
    SoSFDouble phiMin;
    SoSFDouble phiMax;

    Box2D getBox() const;
    void setBox(const Box2D& box);

    bool isInside(double u, double v) const;
    QVector<vec2d> makeMesh(QSize& dims) const;

    NAME_ICON_FUNCTIONS("Circular", ":/profiles/ProfileCircular.png")

protected:
    ~ProfileCircular() {}
};
