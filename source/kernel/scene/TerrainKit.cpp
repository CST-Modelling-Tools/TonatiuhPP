#include "TerrainKit.h"

#include "GridNode.h"

SO_KIT_SOURCE(TerrainKit)


void TerrainKit::initClass()
{
    SO_KIT_INIT_CLASS(TerrainKit, SoBaseKit, "BaseKit");

    GridNode::initClass();
}

TerrainKit::TerrainKit()
{
    SO_KIT_CONSTRUCTOR(TerrainKit);

    SO_KIT_ADD_CATALOG_ENTRY(grid, GridNode, FALSE, this, "", TRUE);

    SO_KIT_INIT_INSTANCE();

    setName("Terrain");
}
