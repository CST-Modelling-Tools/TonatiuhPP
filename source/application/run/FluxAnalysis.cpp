#include "FluxAnalysis.h"

#include <cmath>

#include <QFileDialog>
#include <QFutureWatcher>
#include <QMutex>
#include <QPair>
#include <QProgressDialog>
#include <QtConcurrentMap>

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoGroup.h>

#include "tree/SceneModel.h"
#include "kernel/air/Air.h"
#include "kernel/run/InstanceNode.h"
#include "kernel/photons/Photons.h"
#include "kernel/random//Random.h"
#include "kernel/run/RayTracer.h"
#include "kernel/scene/TSceneKit.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/trf.h"
#include "kernel/shape/ShapeRT.h"
#include "kernel/sun/SunKit.h"
#include "kernel/sun/SunAperture.h"
#include "libraries/geometry/Transform.h"
#include "libraries/geometry/gcf.h"
#include "kernel/air/AirVacuum.h"
#include "kernel/profiles/ProfileRT.h"

/******************************************
 * FluxAnalysis
 *****************************************/

/*!
 * Create FluxAnalysis object
 */
FluxAnalysis::FluxAnalysis(
    TSceneKit* currentScene,
    SceneModel& currentSceneModel,
    InstanceNode* rootSeparatorInstance,
    int sunWidthDivisions,
    int sunHeightDivisions,
    Random* randomDeviate
):
    m_sceneKit(currentScene),
    m_sceneModel(&currentSceneModel),
    m_instanceRoot(rootSeparatorInstance),
    m_sunWidthDivisions(sunWidthDivisions),
    m_sunHeightDivisions(sunHeightDivisions),
    m_rand(randomDeviate),
    m_photons(0),
    m_surfaceURL(""),
    m_tracedRays(0),
    m_photonPower(0),
    m_photonCounts(0),
    m_heightDivisions(0),
    m_widthDivisions(0),
    m_uMin(0),
    m_uMax(0),
    m_vMin(0),
    m_vMax(0),
    m_maximumPhotons(0),
    m_maximumPhotonsXCoord(0),
    m_maximumPhotonsYCoord(0),
    m_maximumPhotonsError(0),
    m_totalPower(0)
{

}

/*!
 * Destroy FluxAnalysis object
 */
FluxAnalysis::~FluxAnalysis()
{
    clearPhotonMap();

    if (m_photonCounts) {
        for (int h = 0; h < m_heightDivisions; h++)
            delete[] m_photonCounts[h];

        delete[] m_photonCounts;
    }
}

/*
 * Returns the type of the surface
 */
QString FluxAnalysis::GetSurfaceType(QString nodeURL)
{
    QModelIndex nodeIndex = m_sceneModel->IndexFromUrl(nodeURL);
    if (!nodeIndex.isValid()) return "";

    InstanceNode* instanceNode = m_sceneModel->getInstance(nodeIndex);
    if (!instanceNode) return "";

    TShapeKit* shapeKit = static_cast<TShapeKit*> (instanceNode->getNode() );
    if (!shapeKit) return "";

    ShapeRT* shape = (ShapeRT*) shapeKit->shapeRT.getValue();
    if (!shape) return "";

    return shape->getTypeName();
}

/*
 * Check if it the selected surface is suitable (cylinder, flat disk or flat rectangle) for the analysis
 */
bool FluxAnalysis::CheckSurface()
{
    QString surfaceType = GetSurfaceType(m_surfaceURL);
    if (surfaceType == "Planar") return true;
    if (surfaceType == "ShapeFlatDisk") return true;
    if (surfaceType == "Cylinder") return true;
    return true;
}

/*
 * Check the surface side.
 */
bool FluxAnalysis::CheckSurfaceSide()
{
    QString surfaceType = GetSurfaceType(m_surfaceURL);

    if (surfaceType == "Planar")
    {
        if (m_surfaceSide != "front" && m_surfaceSide != "back")
            return false;
    }
    else if (surfaceType == "ShapeFlatDisk")
    {
        if (m_surfaceSide != "front" && m_surfaceSide != "back")
            return false;
    }
    else if (surfaceType == "Cylinder")
    {
        if (m_surfaceSide != "front" && m_surfaceSide != "back")
            return false;
    }

    return true;
}

/*
 * Fun flux analysis
 */
void FluxAnalysis::RunFluxAnalysis(QString nodeURL, QString surfaceSide, ulong nOfRays, bool increasePhotonMap, int heightDivisions, int widthDivisions)
{
    m_surfaceURL = nodeURL;
    m_surfaceSide = surfaceSide;

    //Delete a photonCounts
    if (m_photonCounts && m_photonCounts != 0)
    {
        for (int h = 0; h < m_heightDivisions; h++)
        {
            delete[] m_photonCounts[h];
        }

        delete[] m_photonCounts;
    }
    m_photonCounts = 0;
    m_heightDivisions = heightDivisions;
    m_widthDivisions = widthDivisions;

    //Check if there is a scene
    if (!m_sceneKit) return;

    //Check if there is a transmissivity defined
    Air* air = 0;
    if (!m_sceneKit->getPart("air", false) )
        air = 0;
    else
        air = static_cast<Air*> (m_sceneKit->getPart("air", false) );

    //Check if there is a rootSeparator InstanceNode
    if (!m_instanceRoot) return;

    InstanceNode* sceneInstance = m_instanceRoot->getParent();
    if (!sceneInstance) return;

    //Check if there is a light and is properly configured
    if (!m_sceneKit->getPart("lightList[0]", false) ) return;
    SunKit* sunKit = static_cast<SunKit*>(m_sceneKit->getPart("lightList[0]", false) );

    InstanceNode* instanceSun = sceneInstance->children[0];
    if (!instanceSun) return;

    if (!sunKit->getPart("tsunshape", false) ) return;
    SunShape* sunShape = static_cast< SunShape* >(sunKit->getPart("tsunshape", false) );

    if (!sunKit->getPart("icon", false) ) return;
    SunAperture* sunAperture = static_cast< SunAperture* >(sunKit->getPart("icon", false) );

    if (!sunKit->getPart("transform", false) ) return;
    SoTransform* lightTransform = static_cast< SoTransform* >(sunKit->getPart("transform",false) );

    //Check if there is a random generator is defined.
    if (!m_rand || m_rand== 0) return;

    //Check if the surface and the surface side defined is suitable
    if (CheckSurface() == false || CheckSurfaceSide() == false) return;

    //Create the photon map where photons are going to be stored
    if (!m_photons  || !increasePhotonMap)
    {
        if (m_photons) m_photons->endExport(-1);
        delete m_photons;
        m_photons = new Photons();
        m_photons->setBufferSize(HUGE_VAL);
        m_tracedRays = 0;
        m_photonPower = 0;
        m_totalPower = 0;
    }

    QVector<InstanceNode*> exportSuraceList;
    QModelIndex nodeIndex = m_sceneModel->IndexFromUrl(m_surfaceURL);
    if (!nodeIndex.isValid()) return;

    InstanceNode* surfaceNode = m_sceneModel->getInstance(nodeIndex);
    if (!surfaceNode || surfaceNode == 0) return;
    exportSuraceList << surfaceNode;

    //UpdateLightSize(); from MainWindow
    SoGroup* separatorKit = static_cast<SoGroup*>(m_sceneKit->getPart("group", false) );
    if (!separatorKit) return;

    SoGetBoundingBoxAction* action = new SoGetBoundingBoxAction(SbViewportRegion() );
    separatorKit->getBoundingBox(action);
    SbBox3f box = action->getBoundingBox();
    delete action;

    if (!box.isEmpty() )
    {
        BoundingBox sceneBox(
            Vector3D(box.getMin()[0], box.getMin()[1], box.getMin()[2]),
            Vector3D(box.getMax()[0], box.getMax()[1], box.getMax()[2])
        );
        if (sunKit) sunKit->setBox(sceneBox);
    }

    m_sceneModel->UpdateSceneModel();

    //Compute bounding boxes and world to object transforms
    m_instanceRoot->updateTree(Transform::Identity);

    m_photons->setTransform(m_instanceRoot->getTransform().inversed() );


    if (!sunKit->findTexture(m_sunWidthDivisions, m_sunHeightDivisions, m_instanceRoot)) return;

    QVector<long> raysPerThread;
    int maximumValueProgressScale = 100;

    ulong t1 = nOfRays / maximumValueProgressScale;
    for (int progressCount = 0; progressCount < maximumValueProgressScale; ++progressCount)
        raysPerThread << t1;

    if (t1*maximumValueProgressScale < nOfRays)
        raysPerThread << nOfRays - t1*maximumValueProgressScale;

    Transform lightToWorld = tgf::makeTransform(lightTransform);
    instanceSun->setTransform(lightToWorld);

    // Create a progress dialog.
    QProgressDialog dialog;
    dialog.setLabelText(QString("Progressing using %1 thread(s)...").arg(QThread::idealThreadCount() ) );

    // Create a QFutureWatcher and conncect signals and slots.
    QFutureWatcher< void > futureWatcher;
    QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog, SLOT(reset()));
    QObject::connect(&dialog, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));
    QObject::connect(&futureWatcher, SIGNAL(progressRangeChanged(int,int)), &dialog, SLOT(setRange(int,int)));
    QObject::connect(&futureWatcher, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));

    QMutex mutex;
    QMutex mutexPhotonMap;
    QFuture<void> photonMap;
    Air* airTemp = 0;
    if (!dynamic_cast<AirVacuum*>(air))
        airTemp = air;

    photonMap = QtConcurrent::map(raysPerThread, RayTracer(
        m_instanceRoot,
        instanceSun, sunAperture, sunShape, airTemp,
        *m_rand, &mutex, m_photons, &mutexPhotonMap, exportSuraceList
    ));

    futureWatcher.setFuture(photonMap);

    // Display the dialog and start the event loop.
    dialog.exec();
    futureWatcher.waitForFinished();

    m_tracedRays += nOfRays;

    double irradiance = sunKit->irradiance.getValue();
    double area = sunAperture->getArea();
    m_photonPower = area*irradiance/m_tracedRays;

    UpdatePhotonCounts();
}

/*
 * Update photon counts for a specific grid divisions
 */
void FluxAnalysis::UpdatePhotonCounts(int heightDivisions, int widthDivisions)
{
    //Delete a photonCounts
    if (m_photonCounts && (m_photonCounts != 0) )
    {
        for (int h = 0; h < m_heightDivisions; h++)
        {
            delete[] m_photonCounts[h];
        }

        delete[] m_photonCounts;
    }

    m_heightDivisions = heightDivisions;
    m_widthDivisions = widthDivisions;

    UpdatePhotonCounts();
}

/*
 * Update photon counts
 */
void FluxAnalysis::UpdatePhotonCounts()
{
    if (!m_photons) return;

    //Create a new photonCounts
    m_photonCounts = new int*[m_heightDivisions];
    for (int h = 0; h < m_heightDivisions; h++)
    {
        m_photonCounts[h] = new int[m_widthDivisions];
        for (int w = 0; w < m_widthDivisions; w++)
            m_photonCounts[h][w] = 0;
    }

    m_maximumPhotons = 0;
    m_maximumPhotonsXCoord = 0;
    m_maximumPhotonsYCoord = 0;
    m_maximumPhotonsError = 0;

    QString surfaceType = GetSurfaceType(m_surfaceURL);
    QModelIndex nodeIndex = m_sceneModel->IndexFromUrl(m_surfaceURL);
    InstanceNode* instanceNode = m_sceneModel->getInstance(nodeIndex);

    if (surfaceType == "Planar")
    {
        FluxAnalysisPlanar(instanceNode);
    }
    else if (surfaceType == "ShapeFlatDisk")
    {
        FluxAnalysisFlatDisk(instanceNode);
    }
    else if (surfaceType == "Cylinder")
    {
        FluxAnalysisCylinder(instanceNode);
    }
}

/*
 * Flux Analysis for cylinder surfaces.
 */
void FluxAnalysis::FluxAnalysisCylinder(InstanceNode* node)
{
    if (!node) return;
    TShapeKit* shapeKit = static_cast< TShapeKit* > (node->getNode() );
    if (!shapeKit) return;

    ShapeRT* shape = (ShapeRT*) shapeKit->shapeRT.getValue();
    if (!shape || shape == 0) return;

    SoSFDouble* radiusField = static_cast< SoSFDouble* > (shape->getField("radius") );
    double radius = radiusField->getValue();

    SoSFDouble* lengthField = static_cast< SoSFDouble* > (shape->getField("sizeZ") );
    double length = lengthField->getValue();

    SoSFDouble* phiMaxField = static_cast< SoSFDouble* > (shape->getField("phiMax") );
    double phiMax = phiMaxField->getValue();

    int widthDivisionsError = m_widthDivisions - 1;
    int heightDivisionsError = m_heightDivisions - 1;

    int** photonCountsError = new int*[heightDivisionsError];
    for (int i = 0; i < heightDivisionsError; ++i)
    {
        photonCountsError[i] = new int[widthDivisionsError];
        for (int w = 0; w < widthDivisionsError; w++)
            photonCountsError[i][w] = 0;
    }

    int activeSideID = 1;
    if (m_surfaceSide == "INSIDE")
        activeSideID = 0;

    Transform worldToObject = node->getTransform().inversed();

    m_uMin = 0.;
    m_uMax = phiMax*radius;
    m_vMin = -length/2;
    m_vMax = length/2;

    const std::vector<Photon>& photonList = m_photons->getPhotons();
    int totalPhotons = 0;
    for (uint p = 0; p < photonList.size(); p++)
    {
        const Photon& photon = photonList[p];
        if (photon.side == activeSideID)
        {
            totalPhotons++;
            Vector3D photonLocalCoord = worldToObject.transformPoint(photon.pos);
            double phi = atan2(photonLocalCoord.y, photonLocalCoord.x);
            if (phi < 0.) phi += 2 * gcf::pi;
            double arcLength = phi * radius;

            int xbin = floor( (arcLength - m_uMin) / (m_uMax - m_uMin) * m_widthDivisions);
            int ybin = floor( (photonLocalCoord.z - m_vMin) / (m_vMax - m_vMin) * m_heightDivisions);
            m_photonCounts[ybin][xbin] += 1;
            if (m_maximumPhotons < m_photonCounts[ybin][xbin])
            {
                m_maximumPhotons = m_photonCounts[ybin][xbin];
                m_maximumPhotonsXCoord = xbin;
                m_maximumPhotonsYCoord = ybin;
            }

            int xbinE = floor( (arcLength - m_uMin) / (m_uMax - m_uMin) * widthDivisionsError);
            int ybinE = floor( (photonLocalCoord.z - m_vMin) / (m_vMax - m_vMin) * heightDivisionsError);
            photonCountsError[ybinE][xbinE] += 1;
            if (m_maximumPhotonsError < photonCountsError[ybinE][xbinE])
            {
                m_maximumPhotonsError = photonCountsError[ybinE][xbinE];
            }
        }
    }

    m_totalPower = totalPhotons * m_photonPower;

    if (photonCountsError)
    {
        for (int h = 0; h < heightDivisionsError; h++)
        {
            delete[] photonCountsError[h];
        }

        delete[] photonCountsError;
    }
}

/*
 * Flux Analysis for flat disk surfaces.
 */
void FluxAnalysis::FluxAnalysisFlatDisk(InstanceNode* node)
{
    if (!node) return;
    TShapeKit* surfaceNode = static_cast< TShapeKit* > (node->getNode() );
    if (!surfaceNode) return;

    ShapeRT* shape = (ShapeRT*)surfaceNode->shapeRT.getValue();
    if (!shape || shape == 0) return;

    trt::TONATIUH_REAL* radiusField = static_cast< trt::TONATIUH_REAL* > (shape->getField("radius") );
    double radius = radiusField->getValue();

    int widthDivisionsError = m_widthDivisions - 1;
    int heightDivisionsError = m_heightDivisions - 1;

    int** photonCountsError = new int*[heightDivisionsError];
    for (int i = 0; i < heightDivisionsError; ++i)
    {
        photonCountsError[i] = new int[widthDivisionsError];
        for (int w = 0; w < widthDivisionsError; w++)
            photonCountsError[i][w] = 0;
    }

    int activeSideID = 1;
    if (m_surfaceSide == "BACK")
        activeSideID = 0;

    Transform worldToObject = node->getTransform().inversed();

    m_uMin = -radius;
    m_vMin = -radius;
    m_uMax = radius;
    m_vMax = radius;

    const std::vector<Photon>& photonList = m_photons->getPhotons();
    int totalPhotons = 0;
    for (uint p = 0; p < photonList.size(); p++)
    {
        const Photon& photon = photonList[p];
        if (photon.side == activeSideID)
        {
            totalPhotons++;
            Vector3D photonLocalCoord = worldToObject.transformPoint(photon.pos);
            int xbin = floor( (photonLocalCoord.x - m_uMin) / (m_uMax - m_uMin) * m_widthDivisions);
            int ybin = floor( (photonLocalCoord.z - m_vMin) / (m_vMax - m_vMin) * m_heightDivisions);
            m_photonCounts[ybin][xbin] += 1;
            if (m_maximumPhotons < m_photonCounts[ybin][xbin])
            {
                m_maximumPhotons = m_photonCounts[ybin][xbin];
                m_maximumPhotonsXCoord = xbin;
                m_maximumPhotonsYCoord = ybin;
            }

            int xbinE = floor( (photonLocalCoord.x - m_uMin) / (m_uMax - m_uMin) * widthDivisionsError);
            int ybinE = floor( (photonLocalCoord.z - m_vMin) / (m_vMax - m_vMin) * heightDivisionsError);
            photonCountsError[ybinE][xbinE] += 1;
            if (m_maximumPhotonsError < photonCountsError[ybinE][xbinE])
            {
                m_maximumPhotonsError = photonCountsError[ybinE][xbinE];
            }
        }
    }

    m_totalPower = totalPhotons * m_photonPower;

    if (photonCountsError)
    {
        for (int h = 0; h < heightDivisionsError; h++)
            delete[] photonCountsError[h];

        delete[] photonCountsError;
    }
}

/*
 * Flux Analysis for flat rectangle surfaces.
 */
void FluxAnalysis::FluxAnalysisPlanar(InstanceNode* node)
{
    if (!node) return;

    TShapeKit* shapeKit = static_cast<TShapeKit*> (node->getNode());
    if (!shapeKit) return;

    ShapeRT* shape = (ShapeRT*) shapeKit->shapeRT.getValue();
    if (!shape || shape == 0) return;

    ProfileRT* aperture = (ProfileRT*) shapeKit->profileRT.getValue();
    BoundingBox box = aperture->getBox();

    int widthDivisionsError = m_widthDivisions - 1;
    int heightDivisionsError = m_heightDivisions - 1;

    int** photonCountsError = new int*[heightDivisionsError];
    for (int i = 0; i < heightDivisionsError; ++i)
    {
        photonCountsError[i] = new int[widthDivisionsError];
        for (int w = 0; w < widthDivisionsError; w++)
            photonCountsError[i][w] = 0;
    }

    int activeSideID = 1;
    if (m_surfaceSide == "back")
        activeSideID = 0;

    Transform worldToObject = node->getTransform().inversed();

    m_uMin = box.pMin.x;
    m_uMax = box.pMax.x;
    m_vMin = box.pMin.y;
    m_vMax = box.pMax.y;

    int totalPhotons = 0;
    for (const Photon& photon : m_photons->getPhotons())
    {
        if (photon.side == activeSideID)
        {
            totalPhotons++;
            Vector3D pos = worldToObject.transformPoint(photon.pos);
            int xbin = floor( (pos.x - m_uMin) / (m_uMax - m_uMin) * m_widthDivisions);
            int ybin = floor( (pos.y - m_vMin) / (m_vMax - m_vMin) * m_heightDivisions);

            m_photonCounts[ybin][xbin] += 1;
            if (m_maximumPhotons < m_photonCounts[ybin][xbin])
            {
                m_maximumPhotons = m_photonCounts[ybin][xbin];
                m_maximumPhotonsXCoord = xbin;
                m_maximumPhotonsYCoord = ybin;
            }

            int xbinE = floor( (pos.x - m_uMin) / (m_uMax - m_uMin) * widthDivisionsError);
            int ybinE = floor( (pos.y - m_vMin) / (m_vMax - m_vMin) * heightDivisionsError);
            photonCountsError[ybinE][xbinE] += 1;
            if (m_maximumPhotonsError < photonCountsError[ybinE][xbinE])
            {
                m_maximumPhotonsError = photonCountsError[ybinE][xbinE];
            }
        }
    }

    m_totalPower = totalPhotons*m_photonPower;

    if (photonCountsError)
    {
        for (int h = 0; h < heightDivisionsError; h++)
            delete[] photonCountsError[h];

        delete[] photonCountsError;
    }
}

/*
 * Export the flux distribution
 */
void FluxAnalysis::ExportAnalysis(QString directory, QString fileName, bool saveCoords)
{
    if (m_photons == 0 || !m_photons) return;

    if (directory.isEmpty() ) return;

    if (fileName.isEmpty() ) return;

    QFileInfo exportFileInfo(fileName);
    if (exportFileInfo.completeSuffix().compare("txt") ) fileName.append(".txt");

    QFile exportFile(directory + "/" + fileName);
    exportFile.open(QIODevice::WriteOnly);
    QTextStream out(&exportFile);

    double widthCell = (m_uMax - m_uMin) / m_widthDivisions;
    double heightCell = (m_vMax - m_vMin) / m_heightDivisions;
    double areaCell = widthCell * heightCell;

    if (saveCoords)
    {
        out << "x(m)\ty(m)\tFlux(W/m2)" << "\n";

        for (int i = 0; i < m_heightDivisions; i++)
        {
            for (int j = 0; j < m_widthDivisions; j++)
            {
                out << m_uMin + widthCell / 2 + j * widthCell  << "\t" << m_vMin + heightCell / 2 + i * heightCell <<  "\t" << m_photonCounts[i][j] * m_photonPower / areaCell << "\n";
            }
        }
    }
    else
    {
        for (int i = 0; i < m_heightDivisions; i++)
        {
            for (int j = 0; j < m_widthDivisions; j++)
            {
                out << m_photonCounts[m_heightDivisions - 1 - i][j] * m_photonPower / areaCell << "\t";
            }
            out << "\n";
        }
    }
    exportFile.close();
}

/*
 * Returns m_photoCounts.
 */
int** FluxAnalysis::photonCountsValue()
{
    return m_photonCounts;
}

/*
 * Returns m_xmin value.
 */
double FluxAnalysis::xminValue()
{
    return m_uMin;
}

/*
 * Returns m_xmax value.
 */
double FluxAnalysis::xmaxValue()
{
    return m_uMax;
}

/*
 * Returns m_ymin value.
 */
double FluxAnalysis::yminValue()
{
    return m_vMin;
}

/*
 * Returns m_ymax value.
 */
double FluxAnalysis::ymaxValue()
{
    return m_vMax;
}

/*
 * Returns m_maximumPhotons value.
 */
int FluxAnalysis::maximumPhotonsValue()
{
    return m_maximumPhotons;
}

/*
 * Returns m_maximumPhotonsXCoord value.
 */
int FluxAnalysis::maximumPhotonsXCoordValue()
{
    return m_maximumPhotonsXCoord;
}

/*
 * Returns m_maximumPhotonsYCoord value.
 */
int FluxAnalysis::maximumPhotonsYCoordValue()
{
    return m_maximumPhotonsYCoord;
}

/*
 * Returns m_maximumPhotonsError value.
 */
int FluxAnalysis::maximumPhotonsErrorValue()
{
    return m_maximumPhotonsError;
}

/*
 * Returns m_wPhoton value.
 */
double FluxAnalysis::wPhotonValue()
{
    return m_photonPower;
}

/*
 * Returns m_totalFlux value.
 */
double FluxAnalysis::totalPowerValue()
{
    return m_totalPower;
}

/*
 * Clear photon map
 */
void FluxAnalysis::clearPhotonMap()
{
    if (m_photons) m_photons->endExport(-1);
    delete m_photons;
    m_photons = 0;
    m_tracedRays = 0;
    m_photonPower = 0;
    m_totalPower = 0;
}
