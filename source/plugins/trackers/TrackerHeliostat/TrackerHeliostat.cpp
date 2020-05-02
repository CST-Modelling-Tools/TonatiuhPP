#include "TrackerHeliostat.h"

#include <cmath>
#include <QString>
#include <Inventor/nodes/SoTransform.h>

#include "libraries/geometry/NormalVector.h"
#include "libraries/geometry/Point3D.h"
#include "libraries/geometry/Transform.h"
#include "kernel/scene/TSceneKit.h"
#include "libraries/geometry/Vector3D.h"
#include "kernel/trf.h"


SO_NODEENGINE_SOURCE(TrackerHeliostat)

void TrackerHeliostat::initClass()
{
    SO_NODEENGINE_INIT_CLASS(TrackerHeliostat, TTrackerForAiming, "TTrackerForAiming");
}

TrackerHeliostat::TrackerHeliostat():
    m_previousAimingPointType(0)
{
    SO_NODEENGINE_CONSTRUCTOR(TrackerHeliostat);

    SO_NODE_ADD_FIELD( m_azimuth, (0.f) );
    SO_NODE_ADD_FIELD( m_zenith, (90.f) );

    SO_NODE_DEFINE_ENUM_VALUE(AimingPointType, Absolute);
    SO_NODE_DEFINE_ENUM_VALUE(AimingPointType, Relative);
    SO_NODE_SET_SF_ENUM_TYPE(typeOfAimingPoint, AimingPointType);
	SO_NODE_ADD_FIELD( typeOfAimingPoint, (Absolute) );

    SO_NODE_ADD_FIELD( aimingPoint, ( 0.f, 0.f, 0.f ) );

	SO_NODE_DEFINE_ENUM_VALUE( Rotations, YX );
	SO_NODE_DEFINE_ENUM_VALUE( Rotations, YZ );
	SO_NODE_DEFINE_ENUM_VALUE( Rotations, XZ );
	SO_NODE_DEFINE_ENUM_VALUE( Rotations, ZX );
	SO_NODE_SET_SF_ENUM_TYPE( typeOfRotation, Rotations );
	SO_NODE_ADD_FIELD( typeOfRotation, (YX) );

	//ConstructEngineOutput();
	SO_NODEENGINE_ADD_OUTPUT( outputTranslation, SoSFVec3f);
	SO_NODEENGINE_ADD_OUTPUT( outputRotation, SoSFRotation);
	SO_NODEENGINE_ADD_OUTPUT( outputScaleFactor, SoSFVec3f);
	SO_NODEENGINE_ADD_OUTPUT( outputScaleOrientation, SoSFRotation);
	SO_NODEENGINE_ADD_OUTPUT( outputCenter, SoSFVec3f);
}

void TrackerHeliostat::Evaluate(Vector3D sunVectorW, Transform parentWT0)
{
	Vector3D i = parentWT0( sunVectorW );

	if( i.length() == 0.0f ) return;
	i = Normalize(i);

	Point3D focus( aimingPoint.getValue( )[0], aimingPoint.getValue( )[1],aimingPoint.getValue( )[2] );
	Vector3D r;
	if( typeOfAimingPoint.getValue() == 0 ) //Absolute
	{
		r = Vector3D( parentWT0( focus ) );
	}
	else
		r = Vector3D( focus );


	if( r.length() == 0.0f ) return;
	r = Normalize(r);

    Vector3D n = i + r;
	if( n.length() == 0.0f ) return;
	n = Normalize( n );

	Vector3D Axe1;
	if ((typeOfRotation.getValue() == 0 ) || (typeOfRotation.getValue() == 1 ))// YX or YZ
		Axe1 = Vector3D( 0.0f, 1.0f, 0.0f );

	else if (typeOfRotation.getValue() == 2 ) // XZ
		Axe1 = Vector3D( 1.0f, 0.0f, 0.0f );

	else // ZX
		Axe1 = Vector3D(0.0f, 0.0f, 1.0f);

	Vector3D t = CrossProduct( n, Axe1 );
	//Vector3D t( n[2], 0.0f, -n[0] );
	if( t.length() == 0.0f ) return;
	t = Normalize(t);

	Vector3D p = CrossProduct( t, n );
	if (p.length() == 0.0f) return;
	p = Normalize(p);

	SbMatrix transformMatrix;
	if ((typeOfRotation.getValue() == 0 ) || (typeOfRotation.getValue() == 3 ))// YX ou  ZX
	{
		 transformMatrix = SbMatrix( t[0], t[1], t[2], 0.0,
								  n[0], n[1], n[2], 0.0,
								  p[0], p[1], p[2], 0.0,
								  0.0, 0.0, 0.0, 1.0 );
	}
	else // YZ
	{
		transformMatrix = SbMatrix( p[0], p[1], p[2], 0.0,
								  n[0], n[1], n[2], 0.0,
								  t[0], t[1], t[2], 0.0,
								  0.0, 0.0, 0.0, 1.0 );
	}


	SoTransform* newTransform = new SoTransform();
	newTransform->setMatrix( transformMatrix );

	SetEngineOutput(newTransform);
}

void TrackerHeliostat::evaluate()
{
	/*
	if( !IsConnected() )	return;


	SoSearchAction coinSearch;
	coinSearch.setNode( this );


	SoPath* nodePath = m_scene->GetSoPath( &coinSearch );
	if( !nodePath || nodePath == 0 || nodePath->getLength() < 1)
		return;

	SoNodeKitPath* parentPath = static_cast<SoNodeKitPath*>( nodePath );
	parentPath->pop();

	Transform objectToWorld = trf::GetObjectToWorld( parentPath );


	Transform worldToObject = objectToWorld.GetInverse();


	Vector3D i = worldToObject( GetGobalSunVector() );

	if( i.length() == 0.0f ) return;
	i = Normalize(i);

	Point3D focus( aimingPoint.getValue( )[0], aimingPoint.getValue( )[1],aimingPoint.getValue( )[2] );
	Vector3D r;
	if( typeOfAimingPoint.getValue() == 0 ) //Absolute
	{
		r = Vector3D( worldToObject( focus ) );
	}
	else
		r = Vector3D( focus );


	if( r.length() == 0.0f ) return;
	r = Normalize(r);

	Vector3D n = ( i + r );
	if( n.length() == 0.0f ) return;
	n = Normalize( n );

	Vector3D Axe1;
	if ((typeOfRotation.getValue() == 0 ) || (typeOfRotation.getValue() == 1 ))// YX or YZ
		Axe1 = Vector3D( 0.0f, 1.0f, 0.0f );

	else if (typeOfRotation.getValue() == 2 ) // XZ
		Axe1 = Vector3D( 1.0f, 0.0f, 0.0f );

	else // ZX
		Axe1 = Vector3D(0.0f, 0.0f, 1.0f);

	Vector3D t = CrossProduct( n, Axe1 );
	//Vector3D t( n[2], 0.0f, -n[0] );
	if( t.length() == 0.0f ) return;
	t = Normalize(t);

	Vector3D p = CrossProduct( t, n );
	if (p.length() == 0.0f) return;
	p = Normalize(p);

	SbMatrix transformMatrix;
	if ((typeOfRotation.getValue() == 0 ) || (typeOfRotation.getValue() == 3 ))// YX ou  ZX
	{
		 transformMatrix = SbMatrix( t[0], t[1], t[2], 0.0,
								  n[0], n[1], n[2], 0.0,
								  p[0], p[1], p[2], 0.0,
								  0.0, 0.0, 0.0, 1.0 );
	}
	else // YZ
	{
		transformMatrix = SbMatrix( p[0], p[1], p[2], 0.0,
								  n[0], n[1], n[2], 0.0,
								  t[0], t[1], t[2], 0.0,
								  0.0, 0.0, 0.0, 1.0 );
	}

	SoTransform* newTransform = new SoTransform();
	newTransform->setMatrix( transformMatrix );

	SetEngineOutput(newTransform);
	*/
}
