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
#include "photons/Photons.h"
#include "random/RandomAbstract.h"
#include "libraries/geometry/Ray.h"
#include "TonatiuhFunctions.h"
#include "sun/TLightKit.h"
#include "trf.h"
#include "scene/TShapeKit.h"


SoSeparator* trf::DrawPhotons(const Photons& map)
{
    SoSeparator* ans = new SoSeparator;

    SoCoordinate3* coordinates = new SoCoordinate3;
    uint n = 0;
    for (const Photon& photon : map.getPhotons())
    {
        const Point3D& pos = photon.pos;
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

SoSeparator* trf::DrawRays(const Photons& map, unsigned long /*numberOfRays*/)
{
    SoSeparator* ans = new SoSeparator;

    SoCoordinate3* points = new SoCoordinate3;
    QVector<int> rayLengths;
    uint n = 0;
    int s = 0;
    for (const Photon& photon : map.getPhotons())
    {
        if (photon.id == 0 && s > 0) {
            rayLengths << s;
            s = 0;
        }
        const Point3D& pos = photon.pos;
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
void trf::ComputeSceneTreeMap(InstanceNode* instanceRoot, Transform parentWTO, bool insertInSurfaceList)
{
    if (!instanceRoot) return;
    SoBaseKit* nodeRoot = (SoBaseKit*) instanceRoot->getNode();
    if (!nodeRoot) return;

    if (nodeRoot->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId() ) )
    {
        SoTransform* nodeTransform = (SoTransform*) nodeRoot->getPart("transform", true);
        Transform objectToWorld = tgf::TransformFromSoTransform(nodeTransform);
        Transform worldToObject = objectToWorld.GetInverse();

        BoundingBox nodeBox;
        Transform nodeWTO(worldToObject*parentWTO);
        instanceRoot->setTransform(nodeWTO);

        bool insertChildInSurfaceList = insertInSurfaceList;
        for (int index = 0; index < instanceRoot->children.count(); ++index)
        {
            InstanceNode* childInstance = instanceRoot->children[index];
            ComputeSceneTreeMap(childInstance, nodeWTO, insertChildInSurfaceList);

            nodeBox = Union(nodeBox, childInstance->getBox() );
        }
        instanceRoot->setBox(nodeBox);
    }
    else if (nodeRoot->getTypeId().isDerivedFrom(TShapeKit::getClassTypeId()))
    {
        Transform shapeTransform;
        Transform shapeToWorld;

        SoTransform* nodeTransform = static_cast< SoTransform* >(((SoBaseKit*)nodeRoot)->getPart("transform", false) );
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

        BoundingBox shapeBB;

        if (instanceRoot->children.count() > 0)
        {
            InstanceNode* shapeInstance = 0;
            if (instanceRoot->children[0]->getNode()->getTypeId().isDerivedFrom(ShapeAbstract::getClassTypeId() ) )
                shapeInstance =  instanceRoot->children[0];
            else if (instanceRoot->children.count() > 1) shapeInstance =  instanceRoot->children[1];

            if (shapeInstance)
            {
                ShapeAbstract* shapeNode = static_cast< ShapeAbstract* > (shapeInstance->getNode() );
                shapeBB = shapeToWorld(shapeNode->getBox() );

                instanceRoot->setTransform(shapeTransform);
                instanceRoot->setBox(shapeBB);
            }
        }
    }
}

void trf::ComputeFistStageSurfaceList(InstanceNode* instanceNode, QStringList disabledNodesURL, QVector<QPair<TShapeKit*, Transform> >* surfacesList)
{
    if (!instanceNode) return;
    if (disabledNodesURL.contains(instanceNode->GetNodeURL() ) ) return;

    SoBaseKit* coinNode = static_cast< SoBaseKit* > (instanceNode->getNode() );
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
        Transform shapeTransform = instanceNode->getTransform();

        surfacesList->push_back(QPair< TShapeKit*, Transform >(surface, shapeTransform) );
    }
}

void trf::CreatePhotonMap(Photons*& photonMap, QPair<Photons*, std::vector<Photon> > photonsList)
{
    if (!photonMap) photonMap = photonsList.first;
    photonMap->addPhotons(photonsList.second);
}

Transform trf::GetObjectToWorld(SoPath* nodePath)
{
    SoGetMatrixAction* getmatrixAction = new SoGetMatrixAction(SbViewportRegion () );
    getmatrixAction->apply(nodePath);

    Transform transform = tgf::TransformFromMatrix(getmatrixAction->getMatrix() );
    delete getmatrixAction;

    return transform;
}
