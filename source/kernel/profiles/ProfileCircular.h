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

    Box3D getBox() const;
    bool isInside(double u, double v) const;
    QVector<vec2d> makeMesh(QSize& dims) const;

    NAME_ICON_FUNCTIONS("Circular", ":/images/ProfileCircular.png")

protected:
    ~ProfileCircular() {}
};
