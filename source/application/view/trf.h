#pragma once

#include <QMap>
#include <QPair>
#include <QStringList>

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoNode.h>

#include "kernel/photons/Photon.h"
#include "kernel/photons/PhotonsBuffer.h"
#include "kernel/scene/TSeparatorKit.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/ShapeRT.h"
#include "kernel/sun/SunShape.h"
#include "kernel/node/TonatiuhFunctions.h"
#include "libraries/math/3D/Ray.h"
#include "libraries/math/3D/Transform.h"

class InstanceNode;
class Random;
class PhotonsBuffer;

namespace trf
{
    void DrawRays(SoSeparator* group, const PhotonsBuffer& map, long raysLimit);

//    TONATIUH_KERNEL void CreatePhotonMap(Photons*& photonMap, QPair<Photons*, std::vector<Photon> > photonsList);
    Transform GetObjectToWorld(SoPath* nodePath);
}
