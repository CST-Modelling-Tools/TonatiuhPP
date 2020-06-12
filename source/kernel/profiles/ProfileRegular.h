#pragma once

#include "kernel/profiles/ProfileRT.h"


class TONATIUH_KERNEL ProfileRegular: public ProfileRT
{
    SO_NODE_HEADER(ProfileRegular);

public:
    static void initClass();
    ProfileRegular();

    SoSFInt32 n;
    SoSFDouble r;

    Box3D getBox() const;
    bool isInside(double u, double v) const;
    QVector<Vector2D> makeMesh(QSize& dims) const;

    NAME_ICON_FUNCTIONS("Regular", ":/images/ProfileRegular.png")

protected:
    ~ProfileRegular() {}
};
