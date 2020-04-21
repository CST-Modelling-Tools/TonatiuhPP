#pragma once

#include "kernel/raytracing/TMaterialFactory.h"
#include "MaterialStandardSpecular.h"


class MaterialStandardSpecularFactory: public QObject, public TMaterialFactory
{
    Q_OBJECT
    Q_INTERFACES(TMaterialFactory)

    Q_PLUGIN_METADATA(IID "tonatiuh.TMaterialFactory")

public:
    QString TMaterialName() const;
    QIcon TMaterialIcon() const;
    MaterialStandardSpecular* CreateTMaterial() const;
};
