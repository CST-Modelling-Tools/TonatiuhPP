#pragma once

#include <QMap>
#include <QString>

struct PhotonMapExportSettings
{

    void AddParameter( QString parameterName, QString parameterValue );

    QString modeTypeName;
    bool exportCoordinates;
    bool exportIntersectionSurfaceSide;
    bool exportInGlobalCoordinates;
    bool exportPreviousNextPhotonID;;
    bool exportAbsorption;
    bool exportSurfaceID;
    QStringList exportSurfaceNodeList;
    QMap< QString, QString > modeTypeParameters;

};

inline void PhotonMapExportSettings::AddParameter( QString parameterName, QString parameterValue )
{
    modeTypeParameters.insert( parameterName, parameterValue );
}
