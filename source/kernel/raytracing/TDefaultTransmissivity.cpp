#include "TDefaultTransmissivity.h"

SO_NODE_SOURCE( TDefaultTransmissivity );


void TDefaultTransmissivity::initClass()
{
    SO_NODE_INIT_CLASS( TDefaultTransmissivity, TTransmissivity, "TTransmissivity" );
}

TDefaultTransmissivity::TDefaultTransmissivity( )
{
    SO_NODE_CONSTRUCTOR( TDefaultTransmissivity );
    SO_NODE_ADD_FIELD( constant, ( 0.001 ) );
}

TDefaultTransmissivity::~TDefaultTransmissivity()
{

}

bool TDefaultTransmissivity::transmission( double /*distance*/, RandomDeviate& /*rand*/ ) const
{
    return true;
}
