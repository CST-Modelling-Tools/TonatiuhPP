#pragma once

#include "RandomMersenneTwister.h"
#include "kernel/statistics/RandomDeviateFactory.h"

class RandomMersenneTwisterFactory: public QObject, public RandomDeviateFactory
{
	Q_OBJECT
	Q_INTERFACES(RandomDeviateFactory)

    Q_PLUGIN_METADATA(IID "tonatiuh.RandomDeviateFactory")

public:
	QString RandomDeviateName() const;
	QIcon RandomDeviateIcon() const;
    RandomMersenneTwister* CreateRandomDeviate() const;

};
