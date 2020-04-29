#pragma once

#include "kernel/scene/TFactory.h"

class MaterialAbstract;



class MaterialFactory: public TFactory
{
public: 
    virtual MaterialAbstract* create() const = 0;
};

Q_DECLARE_INTERFACE(MaterialFactory, "tonatiuh.MaterialFactory")



template<class T>
class MaterialFactoryT: public MaterialFactory
{
public:
    QString name() const {return T::getClassName();}
    QIcon icon() const {return QIcon(T::getClassIcon());}
    void init() const {T::initClass();}
    T* create() const {return new T;}
};
