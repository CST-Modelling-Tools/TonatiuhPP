#pragma once

#include "kernel/TonatiuhKernel.h"
#include <QMap>
#include <QString>


struct TONATIUH_KERNEL PhotonMapExportSettings
{
    void AddParameter( QString parameterName, QString parameterValue )
    {
        modeTypeParameters.insert( parameterName, parameterValue );
    }

    QString modeTypeName;
    bool exportCoordinates;
    bool exportIntersectionSurfaceSide;
    bool exportInGlobalCoordinates;
    bool exportPreviousNextPhotonID;
    bool exportAbsorption;
    bool exportSurfaceID;
    QStringList exportSurfaceNodeList;
    QMap< QString, QString > modeTypeParameters;
};

