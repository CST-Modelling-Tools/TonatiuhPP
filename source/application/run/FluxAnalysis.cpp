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
#include "kernel/photons/PhotonsBuffer.h"
#include "kernel/random//Random.h"
#include "kernel/run/RayTracer.h"
#include "kernel/scene/TSceneKit.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/trf.h"
#include "kernel/shape/ShapeRT.h"
#include "kernel/sun/SunKit.h"
#include "kernel/sun/SunAperture.h"
#include "libraries/math/3D/Transform.h"
#include "libraries/math/gcf.h"
#include "kernel/air/AirVacuum.h"
#include "kernel/profiles/ProfileRT.h"
#include "libraries/math/2D/Matrix2D.h"



FluxAnalysis::FluxAnalysis(TSceneKit* sceneKit,
    SceneModel* sceneModel,
    InstanceNode* instanceRoot,
    int sunWidthDivisions,
    int sunHeightDivisions,
    Random* randomDeviate
):
    m_sceneKit(sceneKit),
    m_sceneModel(sceneModel),
    m_instanceRoot(instanceRoot),
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
    QModelIndex index = m_sceneModel->IndexFromUrl(nodeURL);
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
void FluxAnalysis::run(QString nodeURL, QString surfaceSide, ulong nOfRays, bool increasePhotonMap, int rows, int cols)
{
    m_surfaceURL = nodeURL;
    m_surfaceSide = surfaceSide;

    m_bins.resize(rows, cols);

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
    SunShape* sunShape = static_cast<SunShape*>(sunKit->getPart("tsunshape", false) );

    if (!sunKit->getPart("icon", false) ) return;
    SunAperture* sunAperture = static_cast<SunAperture*>(sunKit->getPart("icon", false) );

    if (!sunKit->getPart("transform", false) ) return;
    SoTransform* lightTransform = static_cast<SoTransform*>(sunKit->getPart("transform", false) );

    //Check if there is a random generator is defined.
    if (!m_rand) return;

    //Check if the surface and the surface side defined is suitable
//    QString shapeType = getShapeType(m_surfaceURL);
//    QStringList list = {"Planar", "Cylinder"};
//    if (!list.contains(shapeType)) return;

//    if (m_surfaceSide != "front" && m_surfaceSide != "back")
//        return;

    //Create the photon map where photons are going to be stored
    if (!m_photons || !increasePhotonMap)
    {
        if (m_photons) m_photons->endExport(-1);
        delete m_photons;
        m_photons = new PhotonsBuffer(std::numeric_limits<uint>::max());
        m_tracedRays = 0;
        m_powerPhoton = 0;
        m_powerTotal = 0;
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
        Box3D sceneBox(
            vec3d(box.getMin()[0], box.getMin()[1], box.getMin()[2]),
            vec3d(box.getMax()[0], box.getMax()[1], box.getMax()[2])
        );
        if (sunKit) sunKit->setBox(sceneBox);
    }

    m_sceneModel->UpdateSceneModel();

    //Compute bounding boxes and world to object transforms
    m_instanceRoot->updateTree(Transform::Identity);

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
    QFutureWatcher<void> watcher;
    QObject::connect(&watcher, SIGNAL(finished()), &dialog, SLOT(reset()));
    QObject::connect(&dialog, SIGNAL(canceled()), &watcher, SLOT(cancel()));
    QObject::connect(&watcher, SIGNAL(progressRangeChanged(int,int)), &dialog, SLOT(setRange(int,int)));
    QObject::connect(&watcher, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));

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

    watcher.setFuture(photonMap);

    // Display the dialog and start the event loop.
    dialog.exec();
    watcher.waitForFinished();

    m_tracedRays += nOfRays;

    double irradiance = sunKit->irradiance.getValue();
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
        for (int r = 0; r < m_bins.rows(); r++) {
            for (int c = 0; c < m_bins.cols(); c++)
                out << m_uMin + uStep*(c + 0.5)  << "\t" << m_vMin + vStep*(r + 0.5) << "\t" << m_bins(r, c)*coeff << "\n";
        }
    }
    else
    {
        for (int r = m_bins.rows() - 1; r >= 0; r--) {
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
    QModelIndex index = m_sceneModel->IndexFromUrl(m_surfaceURL);
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
        int row = floor((uv.y - m_vMin)/(m_vMax - m_vMin)*m_bins.rows());
        int col = floor((uv.x - m_uMin)/(m_uMax - m_uMin)*m_bins.cols());

        int& bin = m_bins(row, col);
        bin++;
        if (m_photonsMax < bin)
        {
            m_photonsMax = bin;
            m_photonsMaxRow = row;
            m_photonsMaxCol = col;
        }

        int rowE = floor((uv.y - m_vMin)/(m_vMax - m_vMin)*binErrors.rows());
        int colE = floor((uv.x - m_uMin)/(m_uMax - m_uMin)*binErrors.cols());
        int& binE = binErrors(rowE, colE);
        binE++;
        if (m_photonsError < binE)
            m_photonsError = binE;
    }

    m_powerTotal = photonsTotal*m_powerPhoton;
}
