#pragma once

#include <QMap>
#include <QString>

#include "kernel/photons/PhotonExport.h"

class Photon;

class TONATIUH_KERNEL PhotonExportNull: public PhotonExport
{

public:
    PhotonExportNull() {}

    static QStringList GetParameterNames() {return QStringList();}
    void SetSaveParameterValue(QString /*parameterName*/, QString /*parameterValue*/) {}
    bool StartExport() {return true;}

    static const char* getClassName() {return "No export";}
    static const char* getClassIcon() {return ":/images/PhotonExportNull.png";}
    const char* getIcon() const {return getClassIcon();}
};
