#pragma once

#include <QMap>
#include <QString>

#include "kernel/photons/PhotonsAbstract.h"

class Photon;

class TONATIUH_KERNEL PhotonsDefault: public PhotonsAbstract
{

public:
    PhotonsDefault() {}

    static QStringList GetParameterNames() {return QStringList();}
    void SetSaveParameterValue(QString /*parameterName*/, QString /*parameterValue*/) {}
    bool StartExport() {return true;}

    static const char* getClassName() {return "No export";}
    static const char* getClassIcon() {return ":/images/PhotonsDefault.png";}
    const char* getIcon() const {return getClassIcon();}
};
