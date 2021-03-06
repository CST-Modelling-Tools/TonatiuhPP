#include "ScriptRayTracer.h"

#include <QFutureWatcher>
#include <QMutex>
#include <QPoint>
//#include <QScriptContext>
#include <QtConcurrentMap>

#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodekits/SoSceneKit.h>
#include <Inventor/nodes/SoSelection.h>

#include "kernel/air/AirTransmission.h"
#include "kernel/node/TonatiuhFunctions.h"
#include "kernel/photons/PhotonsBuffer.h"
#include "kernel/random/Random.h"
#include "kernel/run/RayTracer.h"
#include "kernel/scene/TSeparatorKit.h"
#include "kernel/shape/ShapeRT.h"
#include "kernel/sun/SunAperture.h"
#include "kernel/sun/SunKit.h"
#include "kernel/sun/SunShape.h"
#include "libraries/math/gcf.h"
#include "main/Document.h"
#include "tree/SceneTreeModel.h"
#include "view/GraphicRoot.h"


ScriptRayTracer::ScriptRayTracer(QVector<RandomFactory*> randomFactories):
    m_document(0),
    m_irradiance(-1),
    m_numberOfRays(0),
    m_photonMap(0),
    m_randomFactories(randomFactories),
    m_random(0),
    m_sceneModel(0),
    m_widthDivisions(200),
    m_heightDivisions(200),
    m_sunPosistionChanged(false),
    m_sunAzimuth(0),
    m_sunElevation(0),
    m_wPhoton(0),
    m_dirName("")
{

}

ScriptRayTracer::~ScriptRayTracer()
{
    delete m_document;
    delete m_photonMap;
    delete m_random;
    delete m_sceneModel;
}

void ScriptRayTracer::Clear()
{
    delete m_document;
    m_document = 0;
    m_irradiance = -1;
    m_numberOfRays = 0;
    delete m_photonMap;
    m_photonMap = 0;
    delete m_random;
    m_random = 0;
    delete m_sceneModel;
    m_sceneModel = 0;
    m_sunAzimuth = 0;
    m_sunElevation = 0;
    m_wPhoton = 0;
    m_dirName.clear();
}

bool ScriptRayTracer::IsValidRandomGeneratorType(QString type)
{
    if (m_randomFactories.size() == 0) return 0;

    QVector< QString > randomGeneratorsNames;
    for (int i = 0; i < m_randomFactories.size(); i++)
        randomGeneratorsNames << m_randomFactories[i]->name();

    int selectedRandom = randomGeneratorsNames.indexOf(type);

    if (selectedRandom < 0) return 0;

    return 1;
}

bool ScriptRayTracer::IsValidSurface(QString surfaceName)
{
    if (!m_sceneModel) return false;

    QModelIndex surfaceIndex = m_sceneModel->indexFromUrl(surfaceName);
    InstanceNode* selectedSurface = m_sceneModel->getInstance(surfaceIndex);
    if (!selectedSurface) return false;

    return true;
}

int ScriptRayTracer::setDir(QString dir)
{
    m_dirName = dir;
    return 1;
}

int ScriptRayTracer::setIrradiance(double irradiance)
{
    m_irradiance = irradiance;
    return 1;
}

int ScriptRayTracer::setNumberOfRays(double nrays)
{
    m_numberOfRays = nrays;
    return 1;
}

int ScriptRayTracer::SetNumberOfWidthDivisions(int ndivisions)
{
    m_widthDivisions = ndivisions;
    return 1;
}

int ScriptRayTracer::SetNumberOfHeightDivisions(int ndivisions)
{
    m_heightDivisions = ndivisions;
    return 1;
}

int ScriptRayTracer::SetPhotonMapExportMode(QString typeName)
{
    if (typeName == QLatin1String("File") ) m_photonMapToFile = true;
    else if (typeName == QLatin1String("DB") ) m_photonMapToFile = false;
    else return 0;

    return 1;
}

int ScriptRayTracer::SetRandomDeviateType(QString typeName)
{
    QVector< QString > randomGeneratorsNames;
    for (int i = 0; i < m_randomFactories.size(); i++)
        randomGeneratorsNames << m_randomFactories[i]->name();

    int selectedRandom = randomGeneratorsNames.indexOf(typeName);
    if (selectedRandom < 0)
    {
        m_random = 0;
        return 0;
    }

    m_random = m_randomFactories[selectedRandom]->create(0);
    return 1;
}

/*!
 * Saves the sun position \a azimuth value. \a azimuth  is in degrees.
 */
void ScriptRayTracer::SetSunAzimtuh(double azimuth)
{
    m_sunAzimuth = azimuth*gcf::degree;
    m_sunPosistionChanged = true;
}

/*!
 * Saves the sun position \a elevation value. \a elevation  is in degrees.
 */
void ScriptRayTracer::SetSunElevation(double elevation)
{
    m_sunElevation = elevation*gcf::degree;
    m_sunPosistionChanged = true;
}

int ScriptRayTracer::SetSunPositionToScene()
{
    if (m_sceneModel)
    {
        QModelIndex sceneIndex;
        InstanceNode* sceneInstance = m_sceneModel->getInstance(sceneIndex);
        SoSceneKit* coinScene = static_cast<SoSceneKit*>(sceneInstance->getNode() );

        if ((coinScene)&& (coinScene->getPart("lightList[0]", false) ))
        {
//            SunKit* sunKit = static_cast< SunKit* >(coinScene->getPart("world.sun", false) ); //?
//            if (m_sunPosistionChanged) {
//                sunKit->azimuth.setValue(m_sunAzimuth);
//                sunKit->elevation.setValue(m_sunElevation);
//                sunKit->updateTransform();
//            }
//            return 1;
        }
        std::cerr << "ScriptRayTracer::SetSunPositionToScene() light not found in scene" << std::endl;
        return 0;
    }
    std::cerr << "ScriptRayTracer::SetSunPositionToScene() sceneModel not found" << std::endl;
    return 0;
}

int ScriptRayTracer::Save(const QString& fileName)
{
    if (!m_document->WriteFile(fileName) )
    {
        std::cerr << "Saving canceled";
        return 0;
    }

    std::cerr << "File saved";
    return 1;
}

int ScriptRayTracer::openFile(QString filename)
{
    delete m_document;
    m_document = new Document;
    if (!m_document->ReadFile(filename) ) return 0;

    delete m_sceneModel;
    m_sceneModel = new SceneTreeModel;
    m_sceneModel->setDocument(m_document);

    return 1;
}

int ScriptRayTracer::Trace()
{
    /*
           m_sceneModel->UpdateSceneModel();
           delete m_photonMap;
           m_photonMap = 0;

           m_photonMap = new TPhotonMap;
           if( m_photonMapToFile )
           {
                ExportToFilePhotonMap* pExportMode
           }
           //if( m_selectedPhotonMap > -1 )    m_photonMap = m_TPhotonMapFactoryList[m_selectedPhotonMap]->CreateTPhotonMap();
           if( !m_photonMap )
           {
                std::cerr<<"ScriptRayTracer::Trace() no photonMap defined"<<std::endl;
                return 0;
           }
           if( !m_randomDeviate )
           {
                std::cerr<<"ScriptRayTracer::Trace() no random generator defined"<<std::endl;
                return 0;
           }

           if( !m_sceneModel )
           {
                std::cerr<<"ScriptRayTracer::Trace() no model defined"<<std::endl;
                return 0;
           }

           QModelIndex sceneIndex;
           InstanceNode* sceneInstance = m_sceneModel->NodeFromIndex( sceneIndex );
           if ( !sceneInstance )
           {
                std::cerr<<"ScriptRayTracer::Trace() no scene defined"<<std::endl;
                return 0;
           }

           InstanceNode* rootSeparatorInstance = sceneInstance->children[1];
           if( !rootSeparatorInstance )
           {
                std::cerr<<"ScriptRayTracer::Trace() no scene root defined"<<std::endl;
                return 0;
           }

           InstanceNode* lightInstance = sceneInstance->children[0];
           if ( !lightInstance )
           {
                std::cerr<<"ScriptRayTracer::Trace() no light defined"<<std::endl;
                return 0;
           }


           SoSceneKit* coinScene =  static_cast< SoSceneKit* >( sceneInstance->GetNode() );

           if ( !coinScene->getPart( "lightList[0]", false ) )    return 0;
           SunKit* lightKit = static_cast< SunKit* >( coinScene->getPart( "lightList[0]", false ) );
           if( m_sunPosistionChanged )    lightKit->ChangePosition( m_sunAzimuth, gc::Pi/2 - m_sunElevation );
           m_sceneModel->UpdateSceneModel();

           if( !lightKit->getPart( "tsunshape", false ) ) return 0;
           SunShape* sunShape = static_cast< SunShape * >( lightKit->getPart( "tsunshape", false ) );


           if( !lightKit->getPart( "icon", false ) ) return 0;
           SunAperture* raycastingSurface = static_cast< SunAperture * >( lightKit->getPart( "icon", false ) );

           if( !lightKit->getPart( "transform" ,false ) ) return 0;
           SoTransform* lightTransform = static_cast< SoTransform* >( lightKit->getPart( "transform" ,false ) );
           Transform lightToWorld = tgf::TransformFromSoTransform( lightTransform );
           lightInstance->SetIntersectionTransform( lightToWorld. GetInverse() );


           m_sceneModel->PrepareAnalyze();

           //Compute bounding boxes and world to object transforms
           trf::ComputeSceneTreeMap( rootSeparatorInstance, Transform( new Matrix4x4 ), true );

           SunKit* light = static_cast< SunKit* > ( lightInstance->GetNode() );
           QStringList disabledNodes = QString( light->disabledNodes.getValue().getString() ).split( ";", Qt::SkipEmptyParts );
           QVector< QPair< TShapeKit*, Transform > > surfacesList;
           trf::ComputeFistStageSurfaceList( rootSeparatorInstance, disabledNodes, &surfacesList );
           light->ComputeLightSourceArea( m_widthDivisions, m_heightDivisions, surfacesList );
           if( surfacesList.count() < 1 )
           {
                std::cerr<<"There are no surfaces defined for ray tracing"<<std::endl;
                return 0;
           }


           //Check if there is a transmissivity defined
           AirTransmission* transmissivity;
           if ( !coinScene->getPart( "air", false ) )    transmissivity = 0;
           else
                transmissivity = static_cast< AirTransmission* > ( coinScene->getPart( "air", false ) );

           //Compute the valid areas for the raytracing

           QVector< double > raysPerThread;
           const int maximumValueProgressScale = 100;
           unsigned long  t1 = m_numberOfRays / maximumValueProgressScale;
           for( int progressCount = 0; progressCount < maximumValueProgressScale; ++ progressCount )
                raysPerThread<< t1;

           if( ( t1 * maximumValueProgressScale ) < m_numberOfRays )    raysPerThread<< ( m_numberOfRays-( t1* maximumValueProgressScale) );

           // Create a QFutureWatcher and connect signals and slots.
           QFutureWatcher< TPhotonMap* > futureWatcher;

           QMutex mutex;
           QFuture< TPhotonMap* > photonMap;
           if( transmissivity )
                photonMap = QtConcurrent::mappedReduced( raysPerThread, RayTracer(  rootSeparatorInstance, lightInstance, raycastingSurface, sunShape, lightToWorld, transmissivity, *m_randomDeviate, &mutex, m_photonMap ), trf::CreatePhotonMap, QtConcurrent::UnorderedReduce );

           else
                photonMap = QtConcurrent::mappedReduced( raysPerThread, RayTracerNoTr(  rootSeparatorInstance, lightInstance, raycastingSurface, sunShape, lightToWorld, *m_randomDeviate, &mutex, m_photonMap ), trf::CreatePhotonMap, QtConcurrent::UnorderedReduce );

           futureWatcher.setFuture( photonMap );
           futureWatcher.waitForFinished();


           if( !m_photonMap ) return 1;
           //if( m_photonMap->StoredRays() == 0 )    return 1;

           double irradiance  = m_irradiance;
           if( irradiance < 0 ) irradiance = sunShape->GetIrradiance();
           m_area = raycastingSurface->GetValidArea();
           m_wPhoton = ( m_area * irradiance ) / m_numberOfRays;

         */
    return 1;
}
