#pragma once

#include <Inventor/fields/SoSFDouble.h>
#include "kernel/sun/SunAbstract.h"

class SoSensor;
class SoFieldSensor;


class SunBuie: public SunAbstract
{
    SO_NODE_HEADER(SunBuie);

public:
    SunBuie();
    static void initClass();
    SoNode* copy(SbBool copyConnections) const;

    void GenerateRayDirection(Vector3D& direction, RandomDeviate& rand) const;
	double GetIrradiance() const;
    double GetThetaMax() const;

    SoSFDouble irradiance;
    SoSFDouble csr;

    NAME_ICON_FUNCTIONS("Buie", ":/SunBuie.png")

protected:
    static void updateCSR(void* data, SoSensor*);
     ~SunBuie();

private:
     double chiValue(double csr) const;
     double phi(double theta) const;
     double pdfTheta(double theta) const;
     double zenithAngle(RandomDeviate& rand) const;
     void updateState(double csrValue);

     SoFieldSensor* m_sensorCSR;

	 double m_chi;
	 double m_k;
	 double m_gamma;
     double m_exp;

     double m_thetaSD; // solar disk
     double m_thetaCS; // circumsolar
     double m_deltaThetaCSSD; // difference

	 double m_alpha;
     double m_heightSD;
     double m_heightCS;
     double m_probabilitySD;

     static const double m_minCRSValue;
     static const double m_maxCRSValue;
};


#include "kernel/sun/SunFactory.h"

class SunPillboxFactory:
    public QObject, public SunFactoryT<SunBuie>
{
    Q_OBJECT
    Q_INTERFACES(SunFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.SunFactory")
};
