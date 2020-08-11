#include "FluxAnalysis.h"

#include <cmath>
#include <vector>

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
#include "kernel/air/AirTransmission.h"
#include "kernel/run/InstanceNode.h"
#include "kernel/photons/PhotonsBuffer.h"
#include "kernel/random//Random.h"
#include "kernel/run/RayTracer.h"
#include "kernel/scene/TSceneKit.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/trf.h"
#include "kernel/shape/ShapeRT.h"
#include "kernel/sun/SunKit.h"
#include "kernel/sun/SunKitW.h"
#include "kernel/sun/SunPosition.h"
#include "kernel/sun/SunAperture.h"
#include "libraries/math/3D/Transform.h"
#include "libraries/math/gcf.h"
#include "kernel/profiles/ProfileRT.h"
#include "libraries/math/2D/Matrix2D.h"
#include "kernel/photons/Photon.h"


FluxAnalysis::FluxAnalysis(TSceneKit* sceneKit,
    SceneModel* sceneModel,
    int sunWidthDivisions,
    int sunHeightDivisions,
    Random* randomDeviate
):
    m_sceneKit(sceneKit),
    m_sceneModel(sceneModel),
    m_sunWidthDivisions(sunWidthDivisions),
    m_sunHeightDivisions(sunHeightDivisions),
    m_rand(randomDeviate),
    m_photons(0),
    m_surfaceURL(""),
    m_tracedRays(0),
    m_powerTotal(0.),
    m_powerPhoton(0.),
    m_uMin(0.),
    m_uMax(0.),
    m_vMin(0.),
    m_vMax(0.),
    m_photonsMax(0),
    m_photonsMaxRow(0),
    m_photonsMaxCol(0),
    m_photonsError(0)
{
    m_instanceLayout = m_sceneModel->getInstance(QModelIndex());
    m_instanceLayout = m_instanceLayout->children[1];
}

FluxAnalysis::~FluxAnalysis()
{
    clear();
}

/*
 * Returns the type of the surface
 */
QString FluxAnalysis::getShapeType(QString nodeURL)
{
    QModelIndex index = m_sceneModel->indexFromUrl(nodeURL);
    if (!index.isValid()) return "";

    InstanceNode* instance = m_sceneModel->getInstance(index);
    if (!instance) return "";

    TShapeKit* shapeKit = static_cast<TShapeKit*>(instance->getNode());
    if (!shapeKit) return "";

    ShapeRT* shape = (ShapeRT*) shapeKit->shapeRT.getValue();
    if (!shape) return "";

    return shape->getTypeName();
}

/*
 * Fun flux analysis
 */
void FluxAnalysis::run(QString nodeURL, QString surfaceSide, ulong nRays, bool photonBufferAppend, int uDivs, int vDivs)
{
    m_surfaceURL = nodeURL;
    m_surfaceSide = surfaceSide;

    m_bins.resize(uDivs, vDivs);

    if (!m_sceneKit) return;

    AirTransmission* air = dynamic_cast<AirTransmission*>(m_sceneKit->getPart("world.air.transmission", false));

    if (!m_instanceLayout) return;
    InstanceNode* instanceScene = m_instanceLayout->getParent();
    if (!instanceScene) return;

    SunKitW* sunKit = static_cast<SunKitW*>(m_sceneKit->getPart("world.sun", false));

//    InstanceNode* instanceSun = instanceScene->children[0]->children[0];
    InstanceNode* instanceSun = instanceScene->children[0]->children[2];
    if (!instanceSun) return;

    SunPosition* sunPosition = (SunPosition*) sunKit->getPart("position", false);
    SunShape* sunShape = (SunShape*) sunKit->getPart("shape", false);
    SunAperture* sunAperture = (SunAperture*) sunKit->getPart("aperture", false);
    SoTransform* sunTransform = (SoTransform*) sunKit->getPart("transform", false);

    if (!m_rand) return;

    //Check if the surface and the surface side defined is suitable
//    QString shapeType = getShapeType(m_surfaceURL);
//    QStringList list = {"Planar", "Cylinder"};
//    if (!list.contains(shapeType)) return;

//    if (m_surfaceSide != "front" && m_surfaceSide != "back")
//        return;

    //Create the photon map where photons are going to be stored
    if (!m_photons || !photonBufferAppend)
    {
        if (m_photons) m_photons->endExport(-1);
        delete m_photons;
//        long q = std::vector<Photon>::max_size();
        m_photons = new PhotonsBuffer(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()/64);
        m_tracedRays = 0;
        m_powerPhoton = 0;
        m_powerTotal = 0;
    }

    QVector<InstanceNode*> exportSuraceList;
    QModelIndex nodeIndex = m_sceneModel->indexFromUrl(m_surfaceURL);
    if (!nodeIndex.isValid()) return;
    InstanceNode* instanceNode = m_sceneModel->getInstance(nodeIndex);
    if (!instanceNode) return;
    exportSuraceList << instanceNode;

    //UpdateLightSize(); from MainWindow
    sunKit->setBox(m_sceneKit);
    m_sceneModel->UpdateSceneModel();

    //Compute bounding boxes and world to object transforms
    m_instanceLayout->updateTree(Transform::Identity);

    if (!sunKit->findTexture(m_sunWidthDivisions, m_sunHeightDivisions, m_instanceLayout)) return;

    QVector<long> raysPerThread;
    int maximumValueProgressScale = 100;

    ulong t1 = nRays / maximumValueProgressScale;
    for (int progressCount = 0; progressCount < maximumValueProgressScale; ++progressCount)
        raysPerThread << t1;

    if (t1*maximumValueProgressScale < nRays)
        raysPerThread << nRays - t1*maximumValueProgressScale;

    Transform lightToWorld = tgf::makeTransform(sunTransform);
    instanceSun->setTransform(lightToWorld);

    // Create a progress dialog.
    QProgressDialog dialog;
    dialog.setLabelText(QString("Progressing using %1 thread(s)...").arg(QThread::idealThreadCount() ) );

    // Create a QFutureWatcher and conncect signals and slots.
    QFutureWatcher<void> watcher;
    QObject::connect(&watcher, SIGNAL(finished()), &dialog, SLOT(reset()));
    QObject::connect(&dialog, SIGNAL(canceled()), &watcher, SLOT(cancel()));
    QObject::connect(&watcher, SIGNAL(progressRangeChanged(int,int)), &dialog, SLOT(setRange(int,int)));
    QObject::connect(&watcher, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));

    QMutex mutex;
    QMutex mutexPhotonMap;
    QFuture<void> photonMap;
    AirTransmission* airTemp = 0;
    if (air->getTypeId() != AirTransmission::getClassTypeId())
        airTemp = air;

    photonMap = QtConcurrent::map(raysPerThread, RayTracer(
        m_instanceLayout,
        instanceSun, sunAperture, sunShape, airTemp,
        *m_rand, &mutex, m_photons, &mutexPhotonMap, exportSuraceList
    ));

    watcher.setFuture(photonMap);

    // Display the dialog and start the event loop.
    dialog.exec();
    watcher.waitForFinished();

    m_tracedRays += nRays;

    double irradiance = sunPosition->irradiance.getValue();
    double area = sunAperture->getArea();
    m_powerPhoton = area*irradiance/m_tracedRays;

    fillBins();
}

/*
 * Update photon counts for a specific grid divisions
 */
void FluxAnalysis::setBins(int rows, int cols)
{
    m_bins.resize(rows, cols);
    fillBins();
}

/*
 * Export the flux distribution
 */
void FluxAnalysis::write(QString directory, QString file, bool withCoords)
{
    if (!m_photons) return;
    if (directory.isEmpty()) return;
    if (file.isEmpty()) return;

    QFileInfo info(file);
    if (info.completeSuffix().compare("txt"))
        file.append(".txt");

    QFile fileOut(directory + "/" + file);
    fileOut.open(QIODevice::WriteOnly);
    QTextStream out(&fileOut);

    double uStep = (m_uMax - m_uMin)/m_bins.cols();
    double vStep = (m_vMax - m_vMin)/m_bins.rows();
    double coeff = m_powerPhoton/(uStep*vStep);

    if (withCoords)
    {
        out << "x(m)\ty(m)\tFlux(W/m2)\n";
        for (int r = 0; r < m_bins.rows(); ++r) {
            for (int c = 0; c < m_bins.cols(); ++c)
                out << m_uMin + uStep*(r + 0.5)  << "\t" << m_vMin + vStep*(c + 0.5) << "\t" << m_bins(r, c)*coeff << "\n";
        }
    }
    else
    {
        for (int r = 0; r < m_bins.rows(); ++r) {
            for (int c = 0; c < m_bins.cols(); c++)
                out << m_bins(r, c)*coeff << "\t";
            out << "\n";
        }
    }

    fileOut.close();
}

/*
 * Clear photon map
 */
void FluxAnalysis::clear()
{
    if (m_photons) m_photons->endExport(-1);
    delete m_photons;
    m_photons = 0;
    m_tracedRays = 0;
    m_powerPhoton = 0.;
    m_powerTotal = 0.;
}

/*
 * Update photon counts
 */
void FluxAnalysis::fillBins()
{
    if (!m_photons) return;

    m_bins.fill(0);
    m_photonsMax = 0;
    m_photonsMaxRow = 0;
    m_photonsMaxCol = 0;
    m_photonsError = 0;

    QString shapeType = getShapeType(m_surfaceURL);
    QModelIndex index = m_sceneModel->indexFromUrl(m_surfaceURL);
    InstanceNode* instance = m_sceneModel->getInstance(index);
    if (!instance) return;
    TShapeKit* shapeKit = static_cast<TShapeKit*>(instance->getNode());
    if (!shapeKit) return;
    ShapeRT* shape = (ShapeRT*) shapeKit->shapeRT.getValue();
    if (!shape) return;
    ProfileRT* profile = (ProfileRT*) shapeKit->profileRT.getValue();
    Box2D box = profile->getBox();
    m_uMin = box.min().x;
    m_vMin = box.min().y;
    m_uMax = box.max().x;
    m_vMax = box.max().y;

    int activeSideID = m_surfaceSide == "back" ? 0 : 1;
    Transform toObject = instance->getTransform().inversed();

    Matrix2D<int> binErrors(m_bins.rows() - 1, m_bins.cols() - 1);
    binErrors.fill(0);

    int photonsTotal = 0;
    for (const Photon& photon : m_photons->getPhotons())
    {
        if (photon.isFront != activeSideID) continue;
        photonsTotal++;
        vec3d p = toObject.transformPoint(photon.pos);
        vec2d uv = shape->getUV(p);
        int r = floor((uv.x - m_uMin)/(m_uMax - m_uMin)*m_bins.rows());
        int c = floor((uv.y - m_vMin)/(m_vMax - m_vMin)*m_bins.cols());

        int& bin = m_bins(r, c);
        bin++;
        if (m_photonsMax < bin)
        {
            m_photonsMax = bin;
            m_photonsMaxRow = r;
            m_photonsMaxCol = c;
        }

        int rE = floor((uv.x - m_uMin)/(m_uMax - m_uMin)*binErrors.rows());
        int cE = floor((uv.y - m_vMin)/(m_vMax - m_vMin)*binErrors.cols());
        int& binE = binErrors(rE, cE);
        binE++;
        if (m_photonsError < binE)
            m_photonsError = binE;
    }

    m_powerTotal = photonsTotal*m_powerPhoton;
}
