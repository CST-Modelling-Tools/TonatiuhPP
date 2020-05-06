#include <cmath>
#include <vector>

#include <QFile>
#include <QTextStream>

#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>

#include "run/InstanceNode.h"
#include "libraries/geometry/Matrix4x4.h"
#include "photons/Photon.h"
#include "photons/PhotonMap.h"
#include "random/RandomAbstract.h"
#include "libraries/geometry/Ray.h"
#include "TonatiuhFunctions.h"
#include "sun/TLightKit.h"
#include "trf.h"
#include "scene/TShapeKit.h"


SoSeparator* trf::DrawPhotons(const PhotonMap& map)
{
    SoSeparator* ans = new SoSeparator;

    SoCoordinate3* coordinates = new SoCoordinate3;
    uint n = 0;
    for (Photon* photon : map.GetAllPhotons())
    {
        Point3D& pos = photon->pos;
        coordinates->point.set1Value(n++, pos.x, pos.y, pos.z);
    }
    ans->addChild(coordinates);

    SoMaterial* material = new SoMaterial;
    material->diffuseColor.setValue(1.f, 1.f, 0.f);
    ans->addChild(material);

    SoDrawStyle* style = new SoDrawStyle;
    style->pointSize = 3;
    ans->addChild(style);

    SoPointSet* points = new SoPointSet;
    ans->addChild(points);

    return ans;
}

SoSeparator* trf::DrawRays(const PhotonMap& map, unsigned long /*numberOfRays*/)
{
    SoSeparator* ans = new SoSeparator;

    SoCoordinate3* points = new SoCoordinate3;
    QVector<int> rayLengths;
    uint n = 0;
    int s = 0;
    for (Photon* photon : map.GetAllPhotons())
    {
        if (photon->id == 0 && s > 0) {
            rayLengths << s;
            s = 0;
        }
        Point3D& pos = photon->pos;
        points->point.set1Value(n++, pos.x, pos.y, pos.z);
        s++;
    }
    rayLengths << s;
    ans->addChild(points);

    SoMaterial* material = new SoMaterial;
    material->diffuseColor.setValue(1.0f, 1.0f, 0.8f);
    ans->addChild(material);

    SoLineSet* lineset = new SoLineSet;
    lineset->numVertices.setValues(0, rayLengths.size(), rayLengths.data());
    ans->addChild(lineset);

    return ans;
}

/**
 * Compute a map with the InstanceNodes of sub-tree with top node \a instanceNode.
 *
 * The map stores for each InstanceNode its BBox and its transform in global coordinates.
 **/
void trf::ComputeSceneTreeMap(InstanceNode* instanceNode, Transform parentWTO, bool insertInSurfaceList)
{
    if (!instanceNode) return;
    SoBaseKit* coinNode = static_cast<SoBaseKit*> (instanceNode->GetNode() );
    if (!coinNode) return;

    if (coinNode->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId() ) )
    {
        SoTransform* nodeTransform = static_cast< SoTransform* >(coinNode->getPart("transform", true) );
        Transform objectToWorld = tgf::TransformFromSoTransform(nodeTransform);
        Transform worldToObject = objectToWorld.GetInverse();

        BBox nodeBB;
        Transform nodeWTO(worldToObject*parentWTO);
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
            if (instanceNode->children[0]->GetNode()->getTypeId().isDerivedFrom(ShapeAbstract::getClassTypeId() ) )
                shapeInstance =  instanceNode->children[0];
            else if (instanceNode->children.count() > 1) shapeInstance =  instanceNode->children[1];

            if (shapeInstance)
            {
                ShapeAbstract* shapeNode = static_cast< ShapeAbstract* > (shapeInstance->GetNode() );
                shapeBB = shapeToWorld(shapeNode->getBox() );

                instanceNode->SetIntersectionTransform(shapeTransform);
                instanceNode->SetIntersectionBBox(shapeBB);

            }
        }

    }
}

void trf::ComputeFistStageSurfaceList(InstanceNode* instanceNode, QStringList disabledNodesURL, QVector<QPair<TShapeKit*, Transform> >* surfacesList)
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

void trf::CreatePhotonMap(PhotonMap*& photonMap, QPair<PhotonMap*, std::vector<Photon> > photonsList)
{
    if (!photonMap) photonMap = photonsList.first;
    photonMap->StoreRays(photonsList.second);
}

Transform trf::GetObjectToWorld(SoPath* nodePath)
{
    SoGetMatrixAction* getmatrixAction = new SoGetMatrixAction(SbViewportRegion () );
    getmatrixAction->apply(nodePath);

    Transform transform = tgf::TransformFromMatrix(getmatrixAction->getMatrix() );
    delete getmatrixAction;

    return transform;
}
