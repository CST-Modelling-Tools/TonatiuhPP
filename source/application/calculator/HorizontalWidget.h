#pragma once

#include <QWidget>

#include "kernel/sun/sunpos.h"

class QLabel;
class SoSeparator;


class HorizontalWidget: public QWidget
{
    Q_OBJECT

public:
    HorizontalWidget(QWidget* parent = 0);
    ~HorizontalWidget();

public slots:
    void CoordinatesChanged(cSunCoordinates coordinates);

private:
    QLabel* m_azimuthValue;
    QLabel* m_zenithValue;

    SoSeparator* AzimuthLine();
    SoSeparator* Ejes() const;
    SoSeparator* Horizon() const;
    SoSeparator* Sphere() const;
    SoSeparator* Star();
    SoSeparator* Text();
    SoSeparator* ZenithLine();

    SoSeparator* m_rootNode;

    const double sphereRadio;

    double m_azimuth;
    double m_zenith;
};
