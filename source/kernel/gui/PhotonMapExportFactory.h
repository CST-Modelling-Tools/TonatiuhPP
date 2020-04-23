#pragma once

#include <QtPlugin>
#include <QIcon>

class PhotonMapExport;
class PhotonMapExportParametersWidget;
class QString;


//!  PhotonMapExportFactory is the interface for export photon map plugins.
/*!
   A export photonmap plugin must implement the following interface to load as a valid plugin for Tonatiuh.
 */

class PhotonMapExportFactory
{
public:
    virtual ~PhotonMapExportFactory() {}

    virtual QString GetName() const = 0;
    virtual QIcon GetIcon() const {return QIcon();}
    virtual PhotonMapExport* GetExportPhotonMapMode() const = 0;
    virtual PhotonMapExportParametersWidget* GetExportPhotonMapModeWidget() const {return 0;}
};

Q_DECLARE_INTERFACE(PhotonMapExportFactory, "tonatiuh.PhotonMapExportFactory")



template<class T>
class PhotonExportFactory: public PhotonMapExportFactory
{
public:

    QString GetName() const
    {
        return T::getClassName();
    }

    T* GetExportPhotonMapMode() const
    {
        return new T;
    }
};
