#pragma once

#include "PhotonMapExportNull.h"
#include "kernel/gui/PhotonMapExportFactory.h"
#include "kernel/gui/PhotonMapExportParametersWidget.h"

class PhotonMapExportNullFactory: public QObject, public PhotonMapExportFactory
{
    Q_OBJECT
    Q_INTERFACES(PhotonMapExportFactory)

    Q_PLUGIN_METADATA(IID "tonatiuh.PhotonMapExportFactory")

public:
   	QString GetName() const;
   	QIcon GetIcon() const;
   	PhotonMapExportNull* GetExportPhotonMapMode() const;
   	PhotonMapExportParametersWidget* GetExportPhotonMapModeWidget() const;
};

