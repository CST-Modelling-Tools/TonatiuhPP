#pragma once

#include "kernel/TonatiuhKernel.h"
#include "TMaterial.h"


class TONATIUH_KERNEL TDefaultMaterial : public TMaterial
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
