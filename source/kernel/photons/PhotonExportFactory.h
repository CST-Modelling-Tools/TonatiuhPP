#pragma once

#include "kernel/scene/TFactory.h"

class PhotonExport;
class PhotonExportWidget;



class TONATIUH_KERNEL PhotonExportFactory: public TFactory
{
public:
    virtual PhotonExport* create() const = 0;
    virtual PhotonExportWidget* createWidget() const {return 0;}
};

Q_DECLARE_INTERFACE(PhotonExportFactory, "tonatiuh.PhotonExportFactory")



template<class T, class W>
class PhotonExportFactoryT: public PhotonExportFactory
{
public:
    QString name() const {return T::getClassName();}
    QIcon icon() const {return QIcon(T::getClassIcon());}
    T* create() const {return new T;}
    W* createWidget() const {return new W;}
};
