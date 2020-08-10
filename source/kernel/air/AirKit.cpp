#include "AirKit.h"

#include "kernel/air/AirTransmission.h"

SO_KIT_SOURCE(AirKit)


void AirKit::initClass()
{
    SO_KIT_INIT_CLASS(AirKit, SoBaseKit, "BaseKit");
}

AirKit::AirKit()
{
    SO_KIT_CONSTRUCTOR(AirKit);

    SO_KIT_ADD_CATALOG_ENTRY(transmission, AirTransmission, FALSE, this, "", TRUE);

    SO_KIT_INIT_INSTANCE();

    setName("Air");
}
