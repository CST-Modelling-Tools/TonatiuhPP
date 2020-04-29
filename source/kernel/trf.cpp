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


SoSeparator* trf::DrawPhotonMapPoints(const PhotonMap& map)
{
    SoSeparator* drawpoints = new SoSeparator;
    SoCoordinate3* points = new SoCoordinate3;
    std::vector< Photon* > photonsList = map.GetAllPhotons();
    uint numRays = 0;

    for (uint i = 0; i < photonsList.size(); i++)
    {
        Point3D photon = photonsList[i]->pos;
        points->point.set1Value(numRays, photon.x, photon.y, photon.z);
        numRays++;
    }

    SoMaterial* myMaterial = new SoMaterial;
    myMaterial->diffuseColor.setValue(1.0, 1.0, 0.0);
    drawpoints->addChild(myMaterial);
    drawpoints->addChild(points);

    SoDrawStyle* drawstyle = new SoDrawStyle;
    drawstyle->pointSize = 3;
    drawpoints->addChild(drawstyle);

    SoPointSet* pointset = new SoPointSet;
    drawpoints->addChild(pointset);

    return drawpoints;

}

SoSeparator* trf::DrawPhotonMapRays(const PhotonMap& map, unsigned long /*numberOfRays*/)
{

    SoSeparator* drawrays = new SoSeparator;
    SoCoordinate3* points = new SoCoordinate3;

    QVector< int >  rayLengths;
    std::vector< Photon* > allRaysLists = map.GetAllPhotons();


    int nRay = 0;
    unsigned int photonIndex = 0;
    while (photonIndex < allRaysLists.size() )
    {
        unsigned long rayLength = 0;
        do
        {
            Photon* photon = allRaysLists[photonIndex];
            Point3D photonPosistion = photon->pos;
            points->point.set1Value(photonIndex, photonPosistion.x, photonPosistion.y, photonPosistion.z);
            photonIndex++;
            rayLength++;
        }while(photonIndex < allRaysLists.size() && allRaysLists[photonIndex]->id > 0);


        rayLengths.push_back(rayLength);
        nRay++;

    }

    SoMaterial* myMaterial = new SoMaterial;
    myMaterial->diffuseColor.setValue(1.0f, 1.0f, 0.8f);
    drawrays->addChild(myMaterial);
    drawrays->addChild(points);

    int* lines = new int[rayLengths.size()];
    for (int l = 0; l < rayLengths.size(); l++)
        lines[l] =  rayLengths[l];

    SoLineSet* lineset = new SoLineSet;
    lineset->numVertices.setValues(0, rayLengths.size(), lines);
    drawrays->addChild(lineset);


    delete[] lines;
    return drawrays;
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
                shapeBB = shapeToWorld(shapeNode->GetBBox() );

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
