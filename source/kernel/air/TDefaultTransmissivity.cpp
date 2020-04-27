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

double TDefaultTransmissivity::transmission(double /*distance*/) const
{
    return constant.getValue();
}
