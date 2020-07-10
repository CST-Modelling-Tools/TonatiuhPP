#pragma once

#include "kernel/TonatiuhKernel.h"
#include <QMap>
#include <QString>


struct TONATIUH_KERNEL PhotonsSettings
{
    QString name; // output

    bool saveCoordinates;
    bool saveCoordinatesGlobal;
    bool saveSurfaceID;
    bool saveSurfaceSide;
    bool savePhotonsID;

    QStringList surfaces;
    QMap<QString, QString> parameters;
};

