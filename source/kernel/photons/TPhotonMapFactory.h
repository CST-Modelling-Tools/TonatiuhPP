#pragma once

#include <QtPlugin>

class QString;
class QIcon;
class TPhotonMap;

class TPhotonMapFactory
{
public:
    virtual ~TPhotonMapFactory() {}
    virtual QString TPhotonMapName() const  = 0;
    virtual QIcon TPhotonMapIcon() const = 0;
    virtual TPhotonMap* CreateTPhotonMap( ) const = 0;
};

Q_DECLARE_INTERFACE( TPhotonMapFactory, "tonatiuh.TPhotonMapFactory")
