#pragma once

#include <QMap>
#include <QString>

#include "kernel/photons/PhotonMapExport.h"

class Photon;

class PhotonMapExportNull: public PhotonMapExport
{

public:
    PhotonMapExportNull() {}

    static QStringList GetParameterNames() {return QStringList();}
    void SetSaveParameterValue(QString /*parameterName*/, QString /*parameterValue*/) {}
    bool StartExport() {return true;}

    static const char* getClassName() {return "No export";}
    static const char* getClassIcon() {return ":/PhotonMapExportNull.png";}
    const char* getIcon() const {return getClassIcon();}
};



#include "kernel/photons/PhotonMapExportFactory.h"
#include "kernel/photons/PhotonMapExportParametersWidget.h"

class PhotonMapExportNullFactory:
    public QObject,
    public PhotonExportFactory<PhotonMapExportNull, PhotonMapExportParametersWidget>
{
    Q_OBJECT
    Q_INTERFACES(PhotonMapExportFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.PhotonMapExportFactory")
};
