#pragma once

#include <QMap>
#include <QPair>
#include <QStringList>

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoNode.h>

#include "kernel/photons/Photon.h"
#include "kernel/photons/Photons.h"
#include "kernel/scene/TSeparatorKit.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/ShapeAbstract.h"
#include "kernel/sun/SunAbstract.h"
#include "kernel/TonatiuhFunctions.h"
#include "libraries/geometry/Ray.h"
#include "libraries/geometry/Transform.h"

class InstanceNode;
class RandomAbstract;
class Photons;

namespace trf
{
    TONATIUH_KERNEL void ComputeSceneTreeMap(InstanceNode* instanceNode, Transform parentWTO, bool insertInSurfaceList);
    TONATIUH_KERNEL void ComputeFistStageSurfaceList(InstanceNode* instanceNode, QStringList disabledNodesURL, QVector< QPair< TShapeKit*, Transform > >* surfacesList);
    TONATIUH_KERNEL void CreatePhotonMap(Photons*& photonMap, QPair<Photons*, std::vector<Photon> > photonsList);

    TONATIUH_KERNEL SoSeparator* DrawPhotons(const Photons& map);
    TONATIUH_KERNEL SoSeparator* DrawRays(const Photons& map, ulong numberOfRays);
    TONATIUH_KERNEL Transform GetObjectToWorld(SoPath* nodePath);
}
