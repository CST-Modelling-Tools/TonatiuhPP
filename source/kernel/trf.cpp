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
        const Vector3D& pos = photon.pos;
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
        const Vector3D& pos = photon.pos;
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
