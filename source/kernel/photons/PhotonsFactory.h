#pragma once

#include "kernel/scene/TFactory.h"

class PhotonsAbstract;
class PhotonsWidget;



class TONATIUH_KERNEL PhotonsFactory: public TFactory
{
public:
    virtual PhotonsAbstract* create() const = 0;
    virtual PhotonsWidget* createWidget() const {return 0;}
};

Q_DECLARE_INTERFACE(PhotonsFactory, "tonatiuh.PhotonsFactory")



template<class T, class W>
class PhotonsFactoryT: public PhotonsFactory
{
public:
    QString name() const {return T::getClassName();}
    QIcon icon() const {return QIcon(T::getClassIcon());}
    T* create() const {return new T;}
    W* createWidget() const {return new W;}
};
