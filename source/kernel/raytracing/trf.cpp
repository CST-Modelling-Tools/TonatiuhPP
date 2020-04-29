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

#include "gui/InstanceNode.h"
#include "libraries/geometry/Matrix4x4.h"
#include "photons/Photon.h"
#include "photons/PhotonMap.h"
#include "random/RandomAbstract.h"
#include "libraries/geometry/Ray.h"
#include "tgf.h"
#include "sun/TLightKit.h"
#include "trf.h"
#include "TShapeKit.h"


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
