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

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#ifndef TRF_H_
#define TRF_H_

#include <QMap>
#include <QPair>
#include <QStringList>

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoNode.h>

#include "Photon.h"
#include "TPhotonMap.h"
#include "Ray.h"
#include "tgf.h"
#include "TShape.h"
#include "TSunShape.h"
#include "Transform.h"
#include "TSeparatorKit.h"
#include "TShapeKit.h"



class InstanceNode;
class RandomDeviate;
class TPhotonMap;

namespace trf
{
	void ComputeSceneTreeMap( InstanceNode* instanceNode, Transform parentWTO, bool insertInSurfaceList );
	void ComputeFistStageSurfaceList( InstanceNode* instanceNode, QStringList disabledNodesURL, QVector< QPair< TShapeKit*, Transform > >* surfacesList);
	void CreatePhotonMap( TPhotonMap*& photonMap, QPair< TPhotonMap* ,  std::vector < Photon  > > photonsList );

	SoSeparator* DrawPhotonMapPoints( const TPhotonMap& map);
	SoSeparator* DrawPhotonMapRays( const TPhotonMap& map, unsigned long numberOfRays );
	Transform GetObjectToWorld(SoPath* nodePath);
}

/**
 * Compute a map with the InstanceNodes of sub-tree with top node \a instanceNode.
 *
 *The map stores for each InstanceNode its BBox and its transform in global coordinates.
 **/
inline void trf::ComputeSceneTreeMap( InstanceNode* instanceNode, Transform parentWTO, bool insertInSurfaceList )
{

	if( !instanceNode ) return;
	SoBaseKit* coinNode = static_cast< SoBaseKit* > ( instanceNode->GetNode() );
	if( !coinNode ) return;

	if( coinNode->getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) )
	{
		SoTransform* nodeTransform = static_cast< SoTransform* >(coinNode->getPart( "transform", true ) );
		Transform objectToWorld = tgf::TransformFromSoTransform( nodeTransform );
		Transform worldToObject = objectToWorld.GetInverse();

		BBox nodeBB;
		Transform nodeWTO(worldToObject * parentWTO );
		instanceNode->SetIntersectionTransform( nodeWTO );

		bool insertChildInSurfaceList=insertInSurfaceList;
		for( int index = 0; index < instanceNode->children.count() ; ++index )
		{
			InstanceNode* childInstance = instanceNode->children[index];
			ComputeSceneTreeMap(childInstance, nodeWTO, insertChildInSurfaceList );

			nodeBB = Union( nodeBB, childInstance->GetIntersectionBBox() );
		}

		instanceNode->SetIntersectionBBox( nodeBB );

	}
	else if (coinNode->getTypeId().isDerivedFrom( TShapeKit::getClassTypeId()))
	{
		Transform shapeTransform;
		Transform shapeToWorld;

		SoTransform* nodeTransform = static_cast< SoTransform* >(((SoBaseKit*)coinNode)->getPart( "transform", false ) );
		if (nodeTransform)
		{
			shapeToWorld = tgf::TransformFromSoTransform( nodeTransform );
			Transform worldToObject = shapeToWorld.GetInverse();
			shapeTransform = worldToObject * parentWTO ;
		}
		else
		{
			shapeTransform = parentWTO;
			shapeToWorld = shapeTransform.GetInverse();
		}


		BBox shapeBB;

		if(  instanceNode->children.count() > 0 )
		{
			InstanceNode* shapeInstance = 0;
			if( instanceNode->children[0]->GetNode()->getTypeId().isDerivedFrom( TShape::getClassTypeId() ) )
				shapeInstance =  instanceNode->children[0];
			else if(  instanceNode->children.count() > 1 )	shapeInstance =  instanceNode->children[1];

			if( shapeInstance )
			{
				TShape* shapeNode = static_cast< TShape* > ( shapeInstance->GetNode() );
				shapeBB = shapeToWorld( shapeNode->GetBBox() );

				instanceNode->SetIntersectionTransform( shapeTransform );
				instanceNode->SetIntersectionBBox( shapeBB );

			}
		}

	}
}

inline void trf::ComputeFistStageSurfaceList( InstanceNode* instanceNode, QStringList disabledNodesURL, QVector< QPair< TShapeKit*, Transform > >* surfacesList)
{
	if( !instanceNode ) return;
	if( disabledNodesURL.contains( instanceNode->GetNodeURL() ) )	return;

	SoBaseKit* coinNode = static_cast< SoBaseKit* > ( instanceNode->GetNode() );
	if( !coinNode ) return;

	if( coinNode->getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) )
	{
		for( int index = 0; index < instanceNode->children.count() ; ++index )
		{
			InstanceNode* childInstance = instanceNode->children[index];
			ComputeFistStageSurfaceList( childInstance, disabledNodesURL, surfacesList );
		}

	}
	else if( coinNode->getTypeId().isDerivedFrom( TShapeKit::getClassTypeId() ) )
	{

		TShapeKit* surface = static_cast< TShapeKit* > ( coinNode );
		Transform shapeTransform = instanceNode->GetIntersectionTransform();

		surfacesList->push_back( QPair< TShapeKit*, Transform >( surface, shapeTransform ) );

	}

}

inline void trf::CreatePhotonMap( TPhotonMap*& photonMap, QPair< TPhotonMap* , std::vector< Photon >  > photonsList )
{
	if( !photonMap )  photonMap = photonsList.first;
	photonMap->StoreRays( photonsList.second );

}


inline Transform trf::GetObjectToWorld(SoPath* nodePath)
{

	SoGetMatrixAction* getmatrixAction = new SoGetMatrixAction( SbViewportRegion () );
	getmatrixAction->apply( nodePath );

	Transform transform =  tgf::TransformFromMatrix( getmatrixAction->getMatrix( ) );
	delete getmatrixAction;

	return transform;
}

#endif /* TRF_H_ */
