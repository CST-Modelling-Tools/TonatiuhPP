#pragma once

#include "kernel/node/TNode.h"

struct DifferentialGeometry;
class Random;
class Ray;


class TONATIUH_KERNEL MaterialRT: public TNode
{
    SO_NODE_ABSTRACT_HEADER(MaterialRT);

public:
    static void initClass();

    //Ray* OutputRay(const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand) const;
    virtual bool OutputRay(const Ray& rayIn, const DifferentialGeometry& dg, Random& rand, Ray& rayOut) const = 0;

    NAME_ICON_FUNCTIONS("X", ":/MaterialX.png")
};



#include "kernel/node/TFactory.h"

class MaterialFactory: public TFactory
{
public:
    virtual MaterialRT* create() const = 0;
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
