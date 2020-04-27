#pragma once

#include "kernel/raytracing/TFactory.h"

class TMaterial;



class TMaterialFactory: public TFactory
{
public: 
    virtual TMaterial* create() const = 0;
};

Q_DECLARE_INTERFACE(TMaterialFactory, "tonatiuh.TMaterialFactory")



template<class T>
class MaterialFactory: public TMaterialFactory
{
public:
    QString name() const {return T::getClassName();}
    QIcon icon() const {return QIcon(T::getClassIcon());}
    void init() const {T::initClass();}
    T* create() const {return new T;}
};
