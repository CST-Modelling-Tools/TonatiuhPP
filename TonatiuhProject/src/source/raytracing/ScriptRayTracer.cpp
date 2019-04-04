/***************************************************************************
Copyright (C) 2008 by the Tonatiuh Software Development Team.

This file is part of Tonatiuh.

Tonatiuh program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.


Acknowledgments:

The development of Tonatiuh was started on 2004 by Dr. Manuel J. Blanco,
then Chair of the Department of Engineering of the University of Texas at
Brownsville. From May 2004 to July 2008, it was supported by the Department
of Energy (DOE) and the National Renewable Energy Laboratory (NREL) under
the Minority Research Associate (MURA) Program Subcontract ACQ-4-33623-06.
During 2007, NREL also contributed to the validation of Tonatiuh under the
framework of the Memorandum of Understanding signed with the Spanish
National Renewable Energy Centre (CENER) on February, 20, 2007 (MOU#NREL-07-117).
Since June 2006, the development of Tonatiuh is being led by the CENER, under the
direction of Dr. Blanco, now Director of CENER Solar Thermal Energy Department.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <QFutureWatcher>
#include <QMutex>
#include <QPoint>
#include <QScriptContext>
#include <QtConcurrentMap>

#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodekits/SoSceneKit.h>
#include <Inventor/nodes/SoSelection.h>

#include "Document.h"
#include "GraphicRoot.h"
#include "SceneModel.h"
#include "ScriptRayTracer.h"
#include "RandomDeviate.h"
#include "RandomDeviateFactory.h"
#include "RayTracer.h"
#include "RayTracerNoTr.h"
#include "tgf.h"
#include "TLightKit.h"
#include "TLightShape.h"
#include "TPhotonMap.h"
#include "trf.h"
#include "TSeparatorKit.h"
#include "TShape.h"
#include "TSunShape.h"
#include "TTransmissivity.h"

ScriptRayTracer::ScriptRayTracer(  QVector< RandomDeviateFactory* > listRandomDeviateFactory )
:
m_document( 0 ),
m_irradiance( -1 ),
m_numberOfRays( 0 ),
m_photonMap( 0 ),
m_RandomDeviateFactoryList( listRandomDeviateFactory ),
m_randomDeviate( 0 ),
m_sceneModel ( 0 ),
m_widthDivisions(200),
m_heightDivisions(200),
m_sunPosistionChanged( false ),
m_sunAzimuth( 0 ),
m_sunElevation( 0 ),
m_wPhoton( 0 ),
m_dirName( "" )
{

}

ScriptRayTracer::~ScriptRayTracer()
{
	delete m_document;
	delete m_photonMap;
	delete m_randomDeviate;
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
	delete m_randomDeviate;
	m_randomDeviate = 0;
	delete m_sceneModel;
	m_sceneModel = 0;
	m_sunAzimuth = 0;
	m_sunElevation = 0;
	m_wPhoton = 0;
	m_dirName.clear();
}

QString ScriptRayTracer::GetDir()
{
	return m_dirName;
}


bool ScriptRayTracer::IsValidRandomGeneratorType( QString type )
{
	if( m_RandomDeviateFactoryList.size() == 0 )	 return 0;

	QVector< QString > randomGeneratorsNames;
	for( int i = 0; i < m_RandomDeviateFactoryList.size(); i++ )
		randomGeneratorsNames<< m_RandomDeviateFactoryList[i]->RandomDeviateName();

	int selectedRandom = randomGeneratorsNames.indexOf( type );

	if( selectedRandom < 0 )	return 0;

	return 1;
}

bool ScriptRayTracer::IsValidSurface( QString surfaceName )
{
	if( !m_sceneModel )	return false;

	QModelIndex surfaceIndex = m_sceneModel->IndexFromNodeUrl( surfaceName );
	InstanceNode* selectedSurface = m_sceneModel->NodeFromIndex( surfaceIndex );
	if( !selectedSurface )	return false;

	return true;
}

int ScriptRayTracer::SetDir( QString dir )
{
	m_dirName = dir;
	return 1;
}

int ScriptRayTracer::SetIrradiance( double irradiance )
{
	m_irradiance = irradiance;
	return 1;
}

int ScriptRayTracer::SetNumberOfRays( double nrays )
{
	m_numberOfRays = nrays;
	return 1;
}

int ScriptRayTracer::SetNumberOfWidthDivisions( int ndivisions )
{
	m_widthDivisions = ndivisions;
	return 1;
}

int ScriptRayTracer::SetNumberOfHeightDivisions( int ndivisions )
{
	m_heightDivisions = ndivisions;
	return 1;
}

int ScriptRayTracer::SetPhotonMapExportMode( QString typeName )
{
	if( typeName == QLatin1String( "File" ) ) 	m_photonMapToFile = true;
	else if( typeName == QLatin1String( "DB" ) ) 	m_photonMapToFile = false;
	else	return 0;

	return 1;
}

int ScriptRayTracer::SetRandomDeviateType( QString typeName )
{
	QVector< QString > randomGeneratorsNames;
	for( int i = 0; i < m_RandomDeviateFactoryList.size(); i++ )
		randomGeneratorsNames<< m_RandomDeviateFactoryList[i]->RandomDeviateName();

	int selectedRandom = randomGeneratorsNames.indexOf( typeName );
	if(  selectedRandom < 0 )
	{
		m_randomDeviate = 0;
		return 0;
	}

	m_randomDeviate = m_RandomDeviateFactoryList[selectedRandom]->CreateRandomDeviate();
	return 1;
}

/*!
 * Saves the sun position \a azimuth value. \a azimuth  is in degrees.
 */
void ScriptRayTracer::SetSunAzimtuh( double azimuth )
{
	m_sunAzimuth = azimuth * gc::Degree;
	m_sunPosistionChanged = true;

}

/*!
 * Saves the sun position \a elevation value. \a elevation  is in degrees.
 */
void ScriptRayTracer::SetSunElevation( double elevation )
{
	m_sunElevation = elevation * gc::Degree;
	m_sunPosistionChanged = true;
}

int  ScriptRayTracer::SetSunPositionToScene()
{
	if (m_sceneModel)
	{
		QModelIndex sceneIndex;
		InstanceNode* sceneInstance = m_sceneModel->NodeFromIndex( sceneIndex );
		SoSceneKit* coinScene =  static_cast< SoSceneKit* >( sceneInstance->GetNode() );

		if ((coinScene)&& ( coinScene->getPart( "lightList[0]", false ) ))
		{
			TLightKit* lightKit = static_cast< TLightKit* >( coinScene->getPart( "lightList[0]", false ) );
			if( m_sunPosistionChanged )	lightKit->ChangePosition( m_sunAzimuth, gc::Pi/2 - m_sunElevation );
			return 1;
		}
		std::cerr<<"ScriptRayTracer::SetSunPositionToScene() light not found in scene"<<std::endl;
		return 0;
	}
	std::cerr<<"ScriptRayTracer::SetSunPositionToScene() sceneModel not found"<<std::endl;
	return 0;
}

int  ScriptRayTracer::SetDisconnectAllTrackers(bool disconnect)
{
	if (m_sceneModel)
	{
		if (disconnect) m_sceneModel->DisconnectAllTrackers();
		else m_sceneModel->ReconnectAllTrackers();
		return 1;
	}
	std::cerr<<"ScriptRayTracer::SetSunPositionToScene() sceneModel not found"<<std::endl;
	return 0;
}

int   ScriptRayTracer::Save( const QString& fileName)
{
 	if( !m_document->WriteFile( fileName ) )
	{
 		std::cerr<< "Saving canceled";
		return 0;
	}

	std::cerr<< "File saved";
	return 1;
}


int ScriptRayTracer::SetTonatiuhModelFile ( QString filename )
{
	delete m_document;
	m_document = 0;

	m_document = new Document;
	if( !m_document->ReadFile( filename ) )	return 0;

	delete m_sceneModel;
	m_sceneModel = 0;
	m_sceneModel = new SceneModel;

	m_sceneModel->SetCoinScene( *m_document->GetSceneKit() );

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
	//if( m_selectedPhotonMap > -1 )	m_photonMap = m_TPhotonMapFactoryList[m_selectedPhotonMap]->CreateTPhotonMap();
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

	if ( !coinScene->getPart( "lightList[0]", false ) )	return 0;
	TLightKit* lightKit = static_cast< TLightKit* >( coinScene->getPart( "lightList[0]", false ) );
	if( m_sunPosistionChanged )	lightKit->ChangePosition( m_sunAzimuth, gc::Pi/2 - m_sunElevation );
	m_sceneModel->UpdateSceneModel();

	if( !lightKit->getPart( "tsunshape", false ) ) return 0;
	TSunShape* sunShape = static_cast< TSunShape * >( lightKit->getPart( "tsunshape", false ) );


	if( !lightKit->getPart( "icon", false ) ) return 0;
	TLightShape* raycastingSurface = static_cast< TLightShape * >( lightKit->getPart( "icon", false ) );

	if( !lightKit->getPart( "transform" ,false ) ) return 0;
	SoTransform* lightTransform = static_cast< SoTransform* >( lightKit->getPart( "transform" ,false ) );
	Transform lightToWorld = tgf::TransformFromSoTransform( lightTransform );
	lightInstance->SetIntersectionTransform( lightToWorld. GetInverse() );


	m_sceneModel->PrepareAnalyze();

	//Compute bounding boxes and world to object transforms
	trf::ComputeSceneTreeMap( rootSeparatorInstance, Transform( new Matrix4x4 ), true );

	TLightKit* light = static_cast< TLightKit* > ( lightInstance->GetNode() );
	QStringList disabledNodes = QString( light->disabledNodes.getValue().getString() ).split( ";", QString::SkipEmptyParts );
	QVector< QPair< TShapeKit*, Transform > > surfacesList;
	trf::ComputeFistStageSurfaceList( rootSeparatorInstance, disabledNodes, &surfacesList );
	light->ComputeLightSourceArea( m_widthDivisions, m_heightDivisions, surfacesList );
	if( surfacesList.count() < 1 )
	{
		std::cerr<<"There are no surfaces defined for ray tracing"<<std::endl;
		return 0;
	}


	//Check if there is a transmissivity defined
	TTransmissivity* transmissivity;
	if ( !coinScene->getPart( "transmissivity", false ) )	transmissivity = 0;
	else
		transmissivity = static_cast< TTransmissivity* > ( coinScene->getPart( "transmissivity", false ) );

	//Compute the valid areas for the raytracing

	QVector< double > raysPerThread;
	const int maximumValueProgressScale = 100;
	unsigned long  t1 = m_numberOfRays / maximumValueProgressScale;
	for( int progressCount = 0; progressCount < maximumValueProgressScale; ++ progressCount )
		raysPerThread<< t1;

	if( ( t1 * maximumValueProgressScale ) < m_numberOfRays )	raysPerThread<< ( m_numberOfRays-( t1* maximumValueProgressScale) );

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
	//if( m_photonMap->StoredRays() == 0 )	return 1;

	double irradiance  = m_irradiance;
	if( irradiance < 0 ) irradiance = sunShape->GetIrradiance();
	m_area = raycastingSurface->GetValidArea();
	m_wPhoton = ( m_area * irradiance ) / m_numberOfRays;

*/
	return 1;
}

double ScriptRayTracer::GetArea(){
	return m_area;
}

double ScriptRayTracer::GetNumrays(){
	return m_numberOfRays;
}
