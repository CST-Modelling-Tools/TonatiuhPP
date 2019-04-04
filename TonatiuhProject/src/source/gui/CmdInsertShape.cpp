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

#include "gf.h"

#include "CmdInsertShape.h"
#include "SceneModel.h"
#include "TShape.h"
#include "TShapeKit.h"

/**
 * Creates a new shape insert command that adds a \a shape to \a shapekit node in the \a model.
 *
 * If \a parent is not null, this command is appended to parent's child list and then owns this command.
 */
CmdInsertShape::CmdInsertShape( TShapeKit* shapeKit, TShape* shape, SceneModel* model, QUndoCommand * parent )
: QUndoCommand("InsertShape", parent), m_shapeKit(shapeKit), m_shape(shape), m_pModel( model ), m_row( -1 )
{
	if( m_shapeKit == 0 ) gf::SevereError( "CmdInsertShape called with NULL TShapeKit*" );
	if( m_shape == 0 ) gf::SevereError( "CmdInsertShape called with NULL TShape*" );
	m_shape->ref();
}

/*!
 * Destroys the CmdInsertShape object.
 */
CmdInsertShape::~CmdInsertShape()
{
	m_shape->unref();
}

/*!
 * Reverts model state. After undo() is called, the \a shapekit node will not contain a shape type node.
 * \sa redo().
 */
void CmdInsertShape::undo()
{
	m_shapeKit->setPart( "shape", NULL );
	m_pModel->RemoveCoinNode( m_row, *m_shapeKit );
}

/*!
 * Applies a change to the model. After redo() the model will contain new shape node located as \a shapekit type node child.
 * \sa undo().
 */
void CmdInsertShape::redo( )
{
	m_shapeKit->setPart( "shape", m_shape );
    m_row = m_pModel->InsertCoinNode( *m_shape, *m_shapeKit );
}
