#include "AirKit.h"

#include "kernel/air/AirVacuum.h"

SO_KIT_SOURCE(AirKit)


void AirKit::initClass()
{
    SO_KIT_INIT_CLASS(AirKit, SoBaseKit, "BaseKit");

    AirTransmission::initClass();
}

AirKit::AirKit()
{
    SO_KIT_CONSTRUCTOR(AirKit);

    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(transmission, AirTransmission, AirVacuum, FALSE, this, "", TRUE);

    SO_KIT_INIT_INSTANCE();

    setName("Air");
}
