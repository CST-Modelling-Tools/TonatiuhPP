#include "DifferentialGeometry.h"
#include "TShape.h"

DifferentialGeometry::DifferentialGeometry()
: u(0.0), v(0.0), pShape(0)
{

}

DifferentialGeometry::DifferentialGeometry( const Point3D& P, const Vector3D& DPDU,
            const Vector3D& DPDV, const Vector3D& DNDU,
            const Vector3D& DNDV, double uu, double vv,
            const TShape *sh )
: point(P), u(uu), v(vv), pShape(sh),
  dpdu(DPDU), dpdv(DPDV), dndu(DNDU), dndv(DNDV)
{
    normal = Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );
}

DifferentialGeometry::~DifferentialGeometry()
{

}
