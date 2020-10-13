#pragma once

#include <QImage>
#include <QVector>
#include <QWidget>

#include "libraries/sun/sunpos.h"
#include "Image.h"

class QGraphicsView;
class QGraphicsPixmapItem;
class QGraphicsPolygonItem;


class WorldMap: public QWidget
{
    Q_OBJECT

public:
    WorldMap(QWidget* parent = 0);
    ~WorldMap();

public slots:
    void LatitudeChanged(double latitude);
    void LongitudeChanged(double longitude);
    void SunChanged(cSunCoordinates coordinates);

protected:
//    void paintEvent(QPaintEvent* event);
//    void mouseMoveEvent(QMouseEvent* e);
    void resizeEvent(QResizeEvent* event);

private:
//    Image m_gray;
//    QImage m_map;

    double m_hourAngle;
    double m_rightAscension;
    double m_declination;

    double m_azimuth;
    double m_zenith;

    double m_latitude;
    double m_longitude;

    QGraphicsView* m_graphicsView;
    QGraphicsPixmapItem* m_itemLocation;
    QGraphicsPixmapItem* m_itemSun;
    QGraphicsPolygonItem* m_itemShadow;
};
