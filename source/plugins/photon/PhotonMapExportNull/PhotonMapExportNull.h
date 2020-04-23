#pragma once

#include <QMap>
#include <QString>

#include "kernel/gui/PhotonMapExport.h"

class Photon;

class PhotonMapExportNull: public PhotonMapExport
{

public:
    PhotonMapExportNull() {}

    static QStringList GetParameterNames() {return QStringList();}
    void SetSaveParameterValue(QString parameterName, QString parameterValue) {}
    bool StartExport() {return true;}

    static const char* getClassName() {return "No export";}
};



#include "kernel/gui/PhotonMapExportFactory.h"

class PhotonMapExportNullFactory:
    public QObject, public PhotonExportFactory<PhotonMapExportNull>
{
    Q_OBJECT
    Q_INTERFACES(PhotonMapExportFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.PhotonMapExportFactory")
};
