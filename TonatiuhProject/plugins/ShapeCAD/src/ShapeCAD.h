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
Juana Amieva, Azael Mancillas, Cesar Cantu, I�igo Les.
***************************************************************************/

#ifndef SHAPECAD_H_
#define SHAPECAD_H_

#include <vector>

#include <Inventor/sensors/SoFieldSensor.h>

#include "BVH.h"
#include "Point3D.h"
#include "NormalVector.h"
#include "trt.h"
#include "Triangle.h"
#include "TShape.h"

/*! *****************************
 * class ShapeCAD
 * **************************** */

class ShapeCAD : public TShape
{
	SO_NODE_HEADER(ShapeCAD);

	/*! *****************************
	 * class sorter
	 * **************************** */
	class Sorter {
	public:
		Sorter( Point3D referencePoint) : m_referencePoint( referencePoint) {}
	      bool operator()( const Triangle* t1, const Triangle* t2)
	      {
	    	  Point3D cT1 = t1->GetCentroid();
	    	  double dT1 = Distance( cT1, m_referencePoint );
	    	  Point3D cT2 = t2->GetCentroid();
	    	  double dT2 = Distance( cT2, m_referencePoint );

	    	  if( dT1 < dT2 )	return ( true );
	    	  return ( false );
	      }

	private:
	      Point3D m_referencePoint;
	};

public:
	ShapeCAD( );
	static void initClass();
	double GetArea() const;
	double GetVolume() const {return 0.0;};
	BBox GetBBox() const;
	QString GetIcon() const;

	bool Intersect(const Ray& objectRay, double *tHit, DifferentialGeometry *dg ) const;
	bool IntersectP( const Ray &ray ) const;

	Point3D Sample( double u, double v ) const;

	bool SetFacetList( std::vector< Triangle* > triangleList );

	int	getFields(SoFieldList & fields) const;


protected:
	static void updateTrinaglesList(void *data, SoSensor *);
	//bool Near( const Triangle* t1, const Triangle* t2, Point3D referencePoint ) const;
	void computeBBox(SoAction *action, SbBox3f &box, SbVec3f &center);
	void generatePrimitives(SoAction *action);
	virtual ~ShapeCAD();



private:

	trt::TONATIUH_CONTAINERREALVECTOR3 v1VertexList;
	trt::TONATIUH_CONTAINERREALVECTOR3 v2VertexList;
	trt::TONATIUH_CONTAINERREALVECTOR3 v3VertexList;
	trt::TONATIUH_CONTAINERREALVECTOR3 normalVertexList;

	std::vector< Triangle*> m_pTriangleList;

	SoFieldSensor* m_v1Sensor;
	SoFieldSensor* m_v2Sensor;
	SoFieldSensor* m_v3Sensor;
	SoFieldSensor* m_normalSensor;

/*
	double m_xMin;
	double m_xMax;
	double m_yMin;
	double m_yMax;
	double m_zMin;
	double m_zMax;
	*/


	BVH* m_pBVH;

};




#endif /* SHAPECAD_H_ */
