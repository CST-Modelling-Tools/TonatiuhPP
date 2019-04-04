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

#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/VRMLnodes/SoVRMLBackground.h>

#include "gf.h"

#include "GraphicRoot.h"
#include "GraphicRootTracker.h"
#include "TSceneKit.h"

void selectionFinishCallback( void * userData, SoSelection* selection )
{
	GraphicRoot* root = static_cast< GraphicRoot* >( userData  );
    if ( root ) root->SelectionChanged( selection );
}

GraphicRoot::GraphicRoot()
:m_graphicsRoot( 0 ),
 m_pGrid( 0 ),
 m_pRays( 0 ),
 m_pRootTransform( 0 ),
 m_pSceneSeparator( 0 ),
 m_pSelectionNode( 0 ),
 m_pTracker( 0 )
{
	m_graphicsRoot = new SoSeparator;
	m_graphicsRoot->ref();

	SoVRMLBackground* vrmlBackground = new SoVRMLBackground;
	//float gcolor[][3] = { {0.9843f, 0.8862f, 0.6745f},{ 0.7843f, 0.6157f, 0.4785f } };
	float gcolor[][3] = { {0.7, 0.42, 0.15f},{ 0.7f, 0.42f, 0.15f } };

	float gangle= 1.570f;
	vrmlBackground->groundColor.setValues( 0, 6, gcolor );
	vrmlBackground->groundAngle.setValue( gangle );
	float scolor[][3] = { {0.0157f, 0.0235f, 0.4509f}, {0.5569f, 0.6157f, 0.8471f} };
	float sangle= 1.570f;
	vrmlBackground->skyColor.setValues( 0,6,scolor );
	vrmlBackground->skyAngle.setValue( sangle );
	m_graphicsRoot->addChild( vrmlBackground );

	m_pSceneSeparator = new SoSeparator;
	m_graphicsRoot->addChild( m_pSceneSeparator );

	m_pRootTransform = new SoTransform;
	m_pRootTransform->ref();
	m_pSceneSeparator->addChild( m_pRootTransform );

	m_pTracker = new GraphicRootTracker;
	//m_pTracker->ref();
	m_pTracker->ConnectParentTranform(m_pRootTransform);

	m_pSelectionNode = new SoSelection;
	m_pSelectionNode->ref();
	m_pSelectionNode->policy = SoSelection::SINGLE;
	m_pSelectionNode->addFinishCallback( selectionFinishCallback, static_cast< void*>( this ) );
	m_pSceneSeparator->addChild( m_pSelectionNode );



}

GraphicRoot::~GraphicRoot()
{

	if( m_pGrid )
	{
		while ( m_pGrid->getRefCount( ) > 1 )	m_pGrid->unref();
		m_pGrid = 0;
	}
	if( m_pRays )
	{
		while ( m_pRays->getRefCount( ) > 1 )	m_pRays->unref();
		m_pRays = 0;
	}
	if( m_pSelectionNode)
	{
		while ( m_pSelectionNode->getRefCount( ) > 1 )	m_pSelectionNode->unref();
		m_pSelectionNode = 0;
	}
	if( m_pRootTransform)
	{
		while ( m_pRootTransform->getRefCount( ) > 1 )	m_pRootTransform->unref();
		m_pRootTransform = 0;
	}
	if( m_graphicsRoot)
	{
		while ( m_graphicsRoot->getRefCount( ) > 1 )	m_graphicsRoot->unref();
		m_graphicsRoot = 0;
	}


}

void GraphicRoot::AddGrid( SoSeparator* grid )
{
	m_pGrid = grid;
	grid->ref();
}

void GraphicRoot::AddRays( SoSeparator* rays )
{
	RemoveRays();
	m_pRays = rays;
	m_pRays->ref();
}

void GraphicRoot::AddModel( TSceneKit* sceneModel )
{
	if( sceneModel )
	{
		m_pSelectionNode->addChild( sceneModel );
		m_pTracker->SetSceneKit( sceneModel );
		m_pTracker->SetAzimuthAngle( sceneModel->GetAzimuthAngle() );
		m_pTracker->SetZenithAngle( sceneModel->GetZenithAngle() );
	}
}


void GraphicRoot::DeselectAll()
{
	m_pSelectionNode->deselectAll();
}

SoSeparator* GraphicRoot::GetNode() const
{
	return m_graphicsRoot;
}

void GraphicRoot::RemoveGrid()
{
	if( m_pGrid )
	{
		ShowGrid( false );
		m_pGrid->removeAllChildren();
		while( m_pGrid->getRefCount() > 1 ) m_pGrid->unref();
		if ( m_pGrid->getRefCount() > 1 ) gf::SevereError( "RemoveGrid: m_pGrid referenced in excess ");
		m_pGrid->unref();
		m_pGrid = 0;
	}

}

void GraphicRoot::RemoveRays()
{
	if( m_pRays )
	{
		m_pRays->removeAllChildren();
		if ( m_pRays->getRefCount() > 1 ) gf::SevereError( "RemoveRays: m_pRays referenced in excess ");
		m_pRays->unref();
		m_pRays = 0;
	}

}

void GraphicRoot::RemoveModel()
{
	if( m_pSelectionNode->getNumChildren() > 0 )
	{
		m_pTracker->SetSceneKit( 0 );
		m_pTracker->Disconnect();

		m_pSelectionNode->removeAllChildren();
	}
}

void GraphicRoot::Select(const SoPath* path )
{
	m_pSelectionNode->select( path );
	m_pSelectionNode->touch();

}

void GraphicRoot::SelectionChanged( SoSelection* selection )
{
    emit ChangeSelection( selection );

}

void GraphicRoot::ShowBackground( bool view )
{
	SoVRMLBackground* vrmlBackground = static_cast< SoVRMLBackground* > ( m_graphicsRoot->getChild( 0 ) );

	if( view )
	{
		//float gcolor[][3] = { {0.9843f, 0.8862f, 0.6745f}, {0.7843f, 0.6157f, 0.4785f} };
		float gcolor[][3] = { {0.7, 0.42, 0.15f},{ 0.7f, 0.42f, 0.15f } };
		float gangle= 1.570f;

		vrmlBackground->groundColor.setValues( 0, 6, gcolor );
		vrmlBackground->groundAngle.setValue( gangle );
		float scolor[][3] = { {0.0157f, 0.0235f, 0.4509f}, {0.5569f, 0.6157f, 0.8471f} };
		float sangle= 1.570f;
		vrmlBackground->skyColor.setValues( 0,6,scolor );
		vrmlBackground->skyAngle.setValue( sangle );
	}
	else
	{
		float color[][3] = { {0.1f, 0.1f, 0.1f}, {0.1f, 0.1f, 0.1f} };
		float angle= 1.570f;
		vrmlBackground->groundColor.setValues( 0, 6, color );
		vrmlBackground->groundAngle.setValue( angle );
		vrmlBackground->skyColor.setValues( 0,6,color );
		vrmlBackground->skyAngle.setValue( angle );
	}

}

void GraphicRoot::ShowGrid( bool view )
{
	if( view && m_pGrid )
		m_graphicsRoot->addChild( m_pGrid );
	else if( !view )
		if ( m_pGrid->getRefCount( ) > 0 )	m_graphicsRoot->removeChild( m_pGrid );

}

void GraphicRoot::ShowRays( bool view )
{
	if( view && ( m_pRays ) )
		m_pSceneSeparator->addChild( m_pRays );
	else if( !view )
		if ( m_pRays->getRefCount( ) > 0 )	m_pSceneSeparator->removeChild( m_pRays );
}

