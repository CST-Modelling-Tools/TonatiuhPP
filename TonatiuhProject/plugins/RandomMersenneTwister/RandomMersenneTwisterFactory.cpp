#include <QIcon>
#include <QString>
#include <QTime>

#include "RandomMersenneTwister.h"
#include "RandomMersenneTwisterFactory.h"

QString RandomMersenneTwisterFactory::RandomDeviateName() const
{
	return QString( "Mersenne Twister" );
}

QIcon  RandomMersenneTwisterFactory::RandomDeviateIcon() const
{
	return QIcon();
}

RandomMersenneTwister* RandomMersenneTwisterFactory::CreateRandomDeviate( ) const
{
	unsigned long seed = QTime::currentTime().msec();
	return new RandomMersenneTwister( seed );
	//return new RandomMersenneTwister( 123 );
}


