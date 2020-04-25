#pragma once

#include <QMap>
#include <QPair>
#include <QStringList>

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoNode.h>

#include "kernel/photons/Photon.h"
#include "kernel/photons/TPhotonMap.h"
#include "libraries/geometry/Ray.h"
#include "kernel/geometry/tgf.h"
#include "TShape.h"
#include "TSunShape.h"
#include "libraries/geometry/Transform.h"
#include "TSeparatorKit.h"
#include "TShapeKit.h"


class InstanceNode;
class RandomDeviate;
class TPhotonMap;

namespace trf
{
void ComputeSceneTreeMap(InstanceNode* instanceNode, Transform parentWTO, bool insertInSurfaceList);
void ComputeFistStageSurfaceList(InstanceNode* instanceNode, QStringList disabledNodesURL, QVector< QPair< TShapeKit*, Transform > >* surfacesList);
void CreatePhotonMap(TPhotonMap*& photonMap, QPair< TPhotonMap*,  std::vector < Photon  > > photonsList);

SoSeparator* DrawPhotonMapPoints(const TPhotonMap& map);
SoSeparator* DrawPhotonMapRays(const TPhotonMap& map, unsigned long numberOfRays);
Transform GetObjectToWorld(SoPath* nodePath);
}

/**
 * Compute a map with the InstanceNodes of sub-tree with top node \a instanceNode.
 *
 * The map stores for each InstanceNode its BBox and its transform in global coordinates.
 **/
inline void trf::ComputeSceneTreeMap(InstanceNode* instanceNode, Transform parentWTO, bool insertInSurfaceList)
{

    if (!instanceNode) return;
    SoBaseKit* coinNode = static_cast< SoBaseKit* > (instanceNode->GetNode() );
    if (!coinNode) return;

    if (coinNode->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId() ) )
    {
        SoTransform* nodeTransform = static_cast< SoTransform* >(coinNode->getPart("transform", true) );
        Transform objectToWorld = tgf::TransformFromSoTransform(nodeTransform);
        Transform worldToObject = objectToWorld.GetInverse();

        BBox nodeBB;
        Transform nodeWTO(worldToObject * parentWTO);
        instanceNode->SetIntersectionTransform(nodeWTO);

        bool insertChildInSurfaceList = insertInSurfaceList;
        for (int index = 0; index < instanceNode->children.count(); ++index)
        {
            InstanceNode* childInstance = instanceNode->children[index];
            ComputeSceneTreeMap(childInstance, nodeWTO, insertChildInSurfaceList);

            nodeBB = Union(nodeBB, childInstance->GetIntersectionBBox() );
        }

        instanceNode->SetIntersectionBBox(nodeBB);

    }
    else if (coinNode->getTypeId().isDerivedFrom(TShapeKit::getClassTypeId()))
    {
        Transform shapeTransform;
        Transform shapeToWorld;

        SoTransform* nodeTransform = static_cast< SoTransform* >(((SoBaseKit*)coinNode)->getPart("transform", false) );
        if (nodeTransform)
        {
            shapeToWorld = tgf::TransformFromSoTransform(nodeTransform);
            Transform worldToObject = shapeToWorld.GetInverse();
            shapeTransform = worldToObject * parentWTO;
        }
        else
        {
            shapeTransform = parentWTO;
            shapeToWorld = shapeTransform.GetInverse();
        }


        BBox shapeBB;

        if (instanceNode->children.count() > 0)
        {
            InstanceNode* shapeInstance = 0;
            if (instanceNode->children[0]->GetNode()->getTypeId().isDerivedFrom(TShape::getClassTypeId() ) )
                shapeInstance =  instanceNode->children[0];
            else if (instanceNode->children.count() > 1) shapeInstance =  instanceNode->children[1];

            if (shapeInstance)
            {
                TShape* shapeNode = static_cast< TShape* > (shapeInstance->GetNode() );
                shapeBB = shapeToWorld(shapeNode->GetBBox() );

                instanceNode->SetIntersectionTransform(shapeTransform);
                instanceNode->SetIntersectionBBox(shapeBB);

            }
        }

    }
}

inline void trf::ComputeFistStageSurfaceList(InstanceNode* instanceNode, QStringList disabledNodesURL, QVector< QPair< TShapeKit*, Transform > >* surfacesList)
{
    if (!instanceNode) return;
    if (disabledNodesURL.contains(instanceNode->GetNodeURL() ) ) return;

    SoBaseKit* coinNode = static_cast< SoBaseKit* > (instanceNode->GetNode() );
    if (!coinNode) return;

    if (coinNode->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId() ) )
    {
        for (int index = 0; index < instanceNode->children.count(); ++index)
        {
            InstanceNode* childInstance = instanceNode->children[index];
            ComputeFistStageSurfaceList(childInstance, disabledNodesURL, surfacesList);
        }

    }
    else if (coinNode->getTypeId().isDerivedFrom(TShapeKit::getClassTypeId() ) )
    {

        TShapeKit* surface = static_cast< TShapeKit* > (coinNode);
        Transform shapeTransform = instanceNode->GetIntersectionTransform();

        surfacesList->push_back(QPair< TShapeKit*, Transform >(surface, shapeTransform) );

    }

}

inline void trf::CreatePhotonMap(TPhotonMap*& photonMap, QPair< TPhotonMap*, std::vector< Photon >  > photonsList)
{
    if (!photonMap) photonMap = photonsList.first;
    photonMap->StoreRays(photonsList.second);

}

inline Transform trf::GetObjectToWorld(SoPath* nodePath)
{

    SoGetMatrixAction* getmatrixAction = new SoGetMatrixAction(SbViewportRegion () );
    getmatrixAction->apply(nodePath);

    Transform transform =  tgf::TransformFromMatrix(getmatrixAction->getMatrix() );
    delete getmatrixAction;

    return transform;
}
