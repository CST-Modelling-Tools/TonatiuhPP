#pragma once

#include "MaterialAbstract.h"


class TONATIUH_KERNEL TDefaultMaterial: public MaterialAbstract
{
    SO_NODE_HEADER(TDefaultMaterial);
    
public:
    TDefaultMaterial();
    static void initClass();
    
    QString getIcon();
    //Ray* OutputRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand  ) const;
    bool OutputRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand, Ray* outputRay  ) const;
     
protected:
    virtual ~TDefaultMaterial();
};
