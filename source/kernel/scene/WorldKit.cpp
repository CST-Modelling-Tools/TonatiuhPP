#include "WorldKit.h"

#include "LocationNode.h"
#include "kernel/sun/SunKit.h"
#include "kernel/air/AirKit.h"
#include "TerrainKit.h"

SO_KIT_SOURCE(WorldKit)


void WorldKit::initClass()
{
    SO_KIT_INIT_CLASS(WorldKit, SoBaseKit, "BaseKit");

    LocationNode::initClass();
    SunKit::initClass();
    AirKit::initClass();
    TerrainKit::initClass();
}

WorldKit::WorldKit()
{
    SO_KIT_CONSTRUCTOR(WorldKit);

    SO_KIT_ADD_CATALOG_ENTRY(location, LocationNode, FALSE, this, "", TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(sun, SunKit, FALSE, this, "", TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(air, AirKit, FALSE, this, "", TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(terrain, TerrainKit, FALSE, this, "", TRUE);

    SO_KIT_INIT_INSTANCE();

    setName("World");
}
