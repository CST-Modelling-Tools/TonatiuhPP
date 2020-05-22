#pragma once

#include <QImage>
#include <QVector>
#include <QWidget>

#include "libraries/sun/sunpos.h"

class Image;
class QLabel;
class QMouseEvent;
class QPaintEvent;
class QPoint;

class WorldMap: public QWidget
{
    Q_OBJECT

public:
    WorldMap(QWidget* widget);
    ~WorldMap();

public slots:
    void mouseMoveEvent(QMouseEvent* e);
    void LocalLatitudeChanged(double latitude);
    void LocalLongitudeChanged(double longitude);
    void SunChanged(cSunCoordinates coordinates);

protected:
    void paintEvent(QPaintEvent* event);

private:
    Image* m_gray;
    QImage m_map;
    QImage m_sun;
    QImage m_position;

    QVector<QPoint> m_points;
    double m_hourAngle;
    double m_rightAscension;
    double m_declination;
    double m_azimuth;
    double m_zenith;
    double m_latitude;
    double m_longitude;
};
