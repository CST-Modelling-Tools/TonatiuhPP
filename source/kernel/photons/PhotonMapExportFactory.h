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

    virtual QString name() const = 0;
    virtual QIcon icon() const {return QIcon();}
    virtual PhotonMapExport* GetExportPhotonMapMode() const = 0;
    virtual PhotonMapExportParametersWidget* GetExportPhotonMapModeWidget() const {return 0;}

    static const char* getClassName() {return "Export file";}
    static const char* getClassIcon() {return ":/Photons.png";}
    const char* getIcon() const {return getClassIcon();}
};

Q_DECLARE_INTERFACE(PhotonMapExportFactory, "tonatiuh.PhotonMapExportFactory")


//#include "PhotonMapExportParametersWidget.h"

template<class T, class W>
class PhotonExportFactory: public PhotonMapExportFactory
{
public:

    QString name() const
    {
        return T::getClassName();
    }

    QIcon icon() const
    {
        return QIcon(T::getClassIcon());
    }

    T* GetExportPhotonMapMode() const
    {
        return new T;
    }

    W* GetExportPhotonMapModeWidget() const
    {
        return new W;
    }
};
