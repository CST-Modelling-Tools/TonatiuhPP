#pragma once

#include <QWidget>

#include "kernel/sun/sunpos.h"

class QLabel;
class SoSeparator;


class CelestialWidget: public QWidget
{
    Q_OBJECT
public:
    CelestialWidget(QWidget* parent = 0);
    ~CelestialWidget();

    double getSphereRadio() {return sphereRadio;}
    double getDeclination() {
        return m_declination;
    }
    double getRightAscension() {
        return m_rightAscension;
    }

public slots:
    void CoordinatesChanged(cSunCoordinates coordinates);

private:
    QLabel* m_rightValue;
    QLabel* m_declinationValue;

    SoSeparator* CelestialEquator() const;
    SoSeparator* Declination();
    SoSeparator* Ecliptic() const;
    SoSeparator* Ejes() const;
    SoSeparator* Points() const;
    SoSeparator* RightAscension();
    SoSeparator* Sphere() const;
    SoSeparator* Star();

    SoSeparator* m_rootNode;

    const double sphereRadio;

    double m_declination;
    double m_rightAscension;
};
