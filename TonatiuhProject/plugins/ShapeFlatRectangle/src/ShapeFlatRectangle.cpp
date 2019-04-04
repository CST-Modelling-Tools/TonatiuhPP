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

#include <QString>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>

#include "gf.h"

#include "BBox.h"
#include "DifferentialGeometry.h"
#include "Ray.h"
#include "ShapeFlatRectangle.h"
#include "Vector3D.h"


SO_NODE_SOURCE(ShapeFlatRectangle);

void ShapeFlatRectangle::initClass()
{
	SO_NODE_INIT_CLASS(ShapeFlatRectangle, TShape, "TShape");
}

ShapeFlatRectangle::ShapeFlatRectangle( )
{
	SO_NODE_CONSTRUCTOR(ShapeFlatRectangle);
	SO_NODE_ADD_FIELD( width, (1.0) );
	SO_NODE_ADD_FIELD( height, (1.0) );

	SO_NODE_DEFINE_ENUM_VALUE( Side, FRONT );
	SO_NODE_DEFINE_ENUM_VALUE( Side, BACK );
	SO_NODE_SET_SF_ENUM_TYPE( activeSide, Side );
	SO_NODE_ADD_FIELD( activeSide, (FRONT) );
}

ShapeFlatRectangle::~ShapeFlatRectangle()
{
}

double ShapeFlatRectangle::GetArea() const
{
	return ( width.getValue() * height.getValue() );
}

/*!
 * Return the shape bounding box.
 */
BBox ShapeFlatRectangle::GetBBox() const
{
	Point3D min = Point3D( -height.getValue()/2, 0.0, -width.getValue()/2);
	Point3D max = Point3D( height.getValue()/2, 0.0, width.getValue()/2);

	return BBox( min, max );
}


QString ShapeFlatRectangle::GetIcon() const
{
	return ":/icons/ShapeFlatRectangle.png";
}

bool ShapeFlatRectangle::Intersect(const Ray& objectRay, double *tHit, DifferentialGeometry *dg) const
{
	// Solve equation for _t_ value
	if ( ( objectRay.origin.y == 0 ) && ( objectRay.direction().y == 0 ) ) return false;
	double t = -objectRay.origin.y * objectRay.invDirection().y;

	// Compute intersection distance along ray
	if( t > objectRay.maxt || t < objectRay.mint ) return false;

    //Evaluate Tolerance
	double tol = 0.00001;
	if( (t - objectRay.mint) < tol ) return false;

	// Compute rectangle hit position
    Point3D hitPoint = objectRay( t );

	// Test intersection against clipping parameters
	if( hitPoint.x < -height.getValue()/2 || hitPoint.x > height.getValue()/2 || hitPoint.z < -width.getValue()/2 || hitPoint.z > width.getValue()/2 ) return false;

	// Now check if the fucntion is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	else if( ( tHit == 0 ) || ( dg == 0 ) ) gf::SevereError( "Function Sphere::Intersect(...) called with null pointers" );


	// Find parametric representation of the rectangle hit point
	double u = ( hitPoint.x + height.getValue()/2 ) / ( height.getValue() );
	double v = ( hitPoint.z + width.getValue()/2 ) / ( width.getValue() );

	// Compute rectangle \dpdu and \dpdv
	Vector3D dpdu ( 0.0, 0.0, height.getValue() );
	Vector3D dpdv ( width.getValue(), 0.0, 0.0 );

	NormalVector N = Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );

	// Compute \dndu and \dndv from fundamental form coefficients
	Vector3D dndu ( 0.0, 0.0, 0.0 );
	Vector3D dndv ( 0.0, 0.0, 0.0 );

	// Initialize _DifferentialGeometry_ from parametric information
	*dg = DifferentialGeometry( hitPoint ,
		                        dpdu,
								dpdv,
		                        dndu,
								dndv,
		                        u, v, this );
	dg->shapeFrontSide = ( DotProduct( N, objectRay.direction() ) > 0 ) ? false : true;

    // Update _tHit_ for quadric intersection
    *tHit = t;

	return true;
}

bool ShapeFlatRectangle::IntersectP( const Ray& objectRay ) const
{
	return Intersect( objectRay, 0, 0 );
}

Point3D ShapeFlatRectangle::Sample( double u, double v ) const
{
	return GetPoint3D( u, v );
}

Point3D ShapeFlatRectangle::GetPoint3D (double u, double v) const
{
	if( OutOfRange( u, v ) ) 	gf::SevereError("Function ShapeFlatRectangle::GetPoint3D called with invalid parameters" );

	double x = u * height.getValue() - (height.getValue()/2);
	double z = (v * width.getValue()) - (width.getValue()/2);

	return Point3D( x, 0, z );
}

NormalVector ShapeFlatRectangle::GetNormal (double /*u*/,double /*v*/ ) const
{
	Vector3D dpdu ( 0.0, 0.0, height.getValue() );
	Vector3D dpdv ( width.getValue(), 0.0, 0.0 );

	return Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );

}

bool ShapeFlatRectangle::OutOfRange( double u, double v ) const
{
	return ( ( u < 0.0 ) || ( u > 1.0 ) || ( v < 0.0 ) || ( v > 1.0 ) );
}

void ShapeFlatRectangle::computeBBox(SoAction*, SbBox3f& box, SbVec3f& center )
{
	BBox bBox = GetBBox();
	// These points define the min and max extents of the box.
    SbVec3f min, max;

    min.setValue( bBox.pMin.x, bBox.pMin.y, bBox.pMin.z );
    max.setValue( bBox.pMax.x, bBox.pMax.y, bBox.pMax.z );;

    // Set the box to bound the two extreme points.
    box.setBounds(min, max);
	center.setValue(0.0, 0.0, 0.0);

}

void ShapeFlatRectangle::generatePrimitives(SoAction *action)
{

    SoPrimitiveVertex   pv;

    SoState  *state = action->getState();

    SbBool useTexFunc = ( SoTextureCoordinateElement::getType(state) ==
                          SoTextureCoordinateElement::FUNCTION );

    const SoTextureCoordinateElement* tce = 0;
    if( useTexFunc ) tce = SoTextureCoordinateElement::getInstance(state);


	SbVec3f  point;
	const int rows = 2; // Number of points per row
    const int columns = 2; // Number of points per column
    const int totalPoints = (rows)*(columns); // Total points in the grid

    float vertex[totalPoints][6];

    int h = 0;
    double ui = 0;
	double vj = 0;

    for (int i = 0; i < rows; ++i )
    {
    	ui =( 1.0 /(double)(rows-1) ) * i;

    	for ( int j = 0 ; j < columns ; j++ )
    	{

    		vj = ( 1.0 /(double)(columns-1) ) * j;

    		Point3D point = GetPoint3D(ui, vj);
    		NormalVector normal;
    		if( activeSide.getValue() == 0 )	normal = GetNormal(ui, vj);
    		else	normal = -GetNormal(ui, vj);

    		vertex[h][0] = point.x;
    		vertex[h][1] = point.y;
    		vertex[h][2] = point.z;
    		vertex[h][3] = normal.x;
    		vertex[h][4] = normal.y;
    		vertex[h][5] = normal.z;

    		pv.setPoint( vertex[h][0], vertex[h][1], vertex[h][2] );
    		h++; //Increase h to the next point.
    	}
    }


	const int totalIndices  = (rows-1)*(columns-1)*4;
    int32_t* indices = new int32_t[totalIndices];
    int k = 0;
    for( int irow = 0; irow < (rows-1); ++irow )
           for( int icolumn = 0; icolumn < (columns-1); ++icolumn )
           {
           	indices[k] = irow*columns + icolumn;
        	indices[k+1] = indices[k] + 1;
        	indices[k+3] = indices[k] + columns;
        	indices[k+2] = indices[k+3] + 1;

        	k+=4; //Set k to the first point of the next face.
           }

    float finalvertex[totalIndices][6];
    for( int ivert = 0; ivert<totalIndices; ++ivert )
    {
    	finalvertex[ivert][0] = vertex[indices[ivert]][0];
    	finalvertex[ivert][1] = vertex[indices[ivert]][1];
    	finalvertex[ivert][2] = vertex[indices[ivert]][2];
    	finalvertex[ivert][3] = vertex[indices[ivert]][3];
    	finalvertex[ivert][4] = vertex[indices[ivert]][4];
    	finalvertex[ivert][5] = vertex[indices[ivert]][5];
    }
    delete[] indices;

    float u = 1;
    float v = 1;

	beginShape(action, QUADS );

    for( int i = 0; i < totalIndices; ++i )
    {
    	SbVec3f  point( finalvertex[i][0], finalvertex[i][1],  finalvertex[i][2] );
    	SbVec3f normal(finalvertex[i][3],finalvertex[i][4], finalvertex[i][5] );
		SbVec4f texCoord = useTexFunc ? tce->get(point, normal): SbVec4f( u,v, 0.0, 1.0 );

		pv.setPoint(point);
		pv.setNormal(normal);
		pv.setTextureCoords(texCoord);
		shapeVertex(&pv);
    }
    endShape();

}
