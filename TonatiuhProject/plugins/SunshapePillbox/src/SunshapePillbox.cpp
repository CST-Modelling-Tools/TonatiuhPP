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

#include "gc.h"

#include "SunshapePillbox.h"

SO_NODE_SOURCE(SunshapePillbox);

void SunshapePillbox::initClass()
{
	SO_NODE_INIT_CLASS(SunshapePillbox, TSunShape, "TSunShape");
}

SunshapePillbox::SunshapePillbox( )
{
	SO_NODE_CONSTRUCTOR( SunshapePillbox );
	SO_NODE_ADD_FIELD( irradiance, ( 1000.0 ) );
	SO_NODE_ADD_FIELD( thetaMax, (0.00465));

}

SunshapePillbox::~SunshapePillbox()
{
}

//Light Interface
void SunshapePillbox::GenerateRayDirection( Vector3D& direction, RandomDeviate& rand ) const
{
	double phi = gc::TwoPi * rand.RandomDouble();
    double theta = asin( sin( thetaMax.getValue() )*sqrt( rand.RandomDouble() ) );
    double sinTheta = sin( theta );
    double cosTheta = cos( theta );
    double cosPhi = cos( phi );
    double sinPhi = sin( phi );

    direction.x = sinTheta*sinPhi;
    direction.y = -cosTheta;
    direction.z = sinTheta*cosPhi;
}

double SunshapePillbox::GetIrradiance( void ) const
{
	return irradiance.getValue();
}

double SunshapePillbox::GetThetaMax() const
{
	return thetaMax.getValue();
}

SoNode* SunshapePillbox::copy( SbBool copyConnections ) const
{
	// Use the standard version of the copy method to create
	// a copy of this instance, including its field data
	SunshapePillbox* newSunShape = dynamic_cast< SunshapePillbox* >( SoNode::copy( copyConnections ) );

	// Copy the m_thetaMin, m_thetaMax private members explicitly
	newSunShape->irradiance = irradiance;
	newSunShape->thetaMax = thetaMax;

	return newSunShape;
}
