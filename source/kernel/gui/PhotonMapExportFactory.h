#pragma once

#include <QtPlugin>

class PhotonMapExport;
class PhotonMapExportParametersWidget;
class QString;
class QIcon;

//!  PhotonMapExportFactory is the interface for export photon map plugins.
/*!
  A export photonmap plugin must implement the following interface to load as a valid plugin for Tonatiuh.
*/

class PhotonMapExportFactory
{
public:
    virtual ~PhotonMapExportFactory() {}

    virtual QString GetName() const  = 0;
    virtual QIcon GetIcon() const = 0;
    virtual PhotonMapExport* GetExportPhotonMapMode() const = 0;
       virtual PhotonMapExportParametersWidget* GetExportPhotonMapModeWidget() const = 0;
};

Q_DECLARE_INTERFACE( PhotonMapExportFactory, "tonatiuh.PhotonMapExportFactory")
