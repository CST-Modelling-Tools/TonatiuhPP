#pragma once

#include "kernel/raytracing/TFactory.h"

class PhotonMapExport;
class PhotonMapExportParametersWidget;



class TONATIUH_KERNEL PhotonMapExportFactory: public TFactory
{
public:
    virtual PhotonMapExport* create() const = 0;
    virtual PhotonMapExportParametersWidget* createWidget() const {return 0;}
};

Q_DECLARE_INTERFACE(PhotonMapExportFactory, "tonatiuh.PhotonMapExportFactory")



template<class T, class W>
class PhotonExportFactory: public PhotonMapExportFactory
{
public:
    QString name() const {return T::getClassName(); }
    QIcon icon() const {return QIcon(T::getClassIcon());}
    T* create() const {return new T;}
    W* createWidget() const {return new W;}
};
