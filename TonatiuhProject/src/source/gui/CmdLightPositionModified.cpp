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

#include "CmdLightPositionModified.h"
#include "TLightKit.h"

/**
 * Creates a new lightKit position command that represents a \a light move with \a parent parent.
 *
 * The new position is defined by \a azimuth and \a zenith in radians.
 */
CmdLightPositionModified::CmdLightPositionModified( TLightKit* light, double azimuth, double zenith, QUndoCommand* parent )
:QUndoCommand( "Sun position changed", parent ), lightKit( light ), m_newAzimuth( azimuth ), m_newZenith( zenith )
{
	if( light == 0 ) gf::SevereError( "CmdLinghtPositionModified called with NULL TLightKit" );

	m_oldAzimuth = light->azimuth.getValue();
	m_oldZenith = light->zenith.getValue();
}

/*!
 * Destroys the CmdLightPositionModified object.
 */
CmdLightPositionModified::~CmdLightPositionModified()
{
}

/*!
 * Reverts to the previous light positions. After undo() is called, the light position will be the same as before redo() was called.
 *
 * \sa redo().
 */
void CmdLightPositionModified::undo()
{
	lightKit->ChangePosition( m_oldAzimuth, m_oldZenith );
}

/*!
 * Applies a change to the scene. After redo() the light position will be the position defined by the constructor parameters.
 * \sa undo().
 */
void CmdLightPositionModified::redo()
{
	lightKit->ChangePosition( m_newAzimuth, m_newZenith );
}
