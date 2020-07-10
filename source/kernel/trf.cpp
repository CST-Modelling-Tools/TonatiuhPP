#include <cmath>
#include <vector>

#include <QFile>
#include <QTextStream>

#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>

#include "run/InstanceNode.h"
#include "libraries/math/3D/Matrix4x4.h"
#include "photons/Photon.h"
#include "photons/PhotonsBuffer.h"
#include "random/Random.h"
#include "libraries/math/3D/Ray.h"
#include "TonatiuhFunctions.h"
#include "sun/SunKit.h"
#include "trf.h"
#include "scene/TShapeKit.h"


void trf::DrawRays(SoSeparator* parent, const PhotonsBuffer& map, long raysLimit)
{
    parent->removeAllChildren();

    SoCoordinate3* points = new SoCoordinate3;
    QVector<int> rayLengths;
    uint n = 0;
    int s = 0;
    for (const Photon& photon : map.getPhotons())
    {
        if (photon.id == 0 && s > 0) {
            rayLengths << s;
            s = 0;
            if (rayLengths.size() >= raysLimit) break;
        }
        const vec3d& pos = photon.pos;
        points->point.set1Value(n++, pos.x, pos.y, pos.z);
        s++;
    }
    if (s > 0) rayLengths << s;
    parent->addChild(points);

    SoDrawStyle* style = new SoDrawStyle;
    style->lineWidth = 1.;
    style->pointSize = 3.;
    parent->addChild(style);

    // rays
    SoSwitch* sRays = new SoSwitch;
    sRays->setName("rays");
    parent->addChild(sRays);

    SoMaterial* materialRays = new SoMaterial;
    materialRays->diffuseColor.setValue(1.0f, 1.0f, 0.8f);
    sRays->addChild(materialRays);

    SoLineSet* lineSet = new SoLineSet;
    lineSet->numVertices.setValues(0, rayLengths.size(), rayLengths.data());
    sRays->addChild(lineSet);

    // points
    SoSwitch* sPhotons = new SoSwitch;
    sPhotons->setName("photons");
    parent->addChild(sPhotons);

    SoMaterial* materialPhotons = new SoMaterial;
    materialPhotons->diffuseColor.setValue(1.f, 1.f, 0.f);
    sPhotons->addChild(materialPhotons);

    SoPointSet* pointSet = new SoPointSet;
    sPhotons->addChild(pointSet);
}

//void trf::CreatePhotonMap(Photons*& photonMap, QPair<Photons*, std::vector<Photon> > photonsList)
//{
//    if (!photonMap) photonMap = photonsList.first;
//    photonMap->addPhotons(photonsList.second);
//}

Transform trf::GetObjectToWorld(SoPath* nodePath)
{
    SoGetMatrixAction* getmatrixAction = new SoGetMatrixAction(SbViewportRegion () );
    getmatrixAction->apply(nodePath);

    Transform transform = tgf::makeTransform(getmatrixAction->getMatrix() );
    delete getmatrixAction;

    return transform;
}
