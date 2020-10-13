#include "WorldMap.h"

#include <iostream>

#include <QColor>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QString>
#include <QToolTip>
#include <QVBoxLayout>

#include "libraries/math/gcf.h"

#include "Image.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>


WorldMap::WorldMap(QWidget* parent):
    QWidget(parent),
    m_rightAscension(0),
    m_declination(0),
    m_azimuth(0),
    m_zenith(0)
{
//    m_gray.load(":/images/map/worldMapGray.bmp"); // countries
//    m_map.load(":/images/map/worldMap.jpg");

    m_latitude = 0;
    m_longitude = 0;

    ////

    QVBoxLayout* vLayout = new QVBoxLayout;
    setLayout(vLayout);

    QGraphicsScene* scene = new QGraphicsScene(this);

    // map
    QPixmap pixmap(":/images/map/worldMap.jpg");
    QGraphicsPixmapItem* itemWorld = new QGraphicsPixmapItem(pixmap);
    scene->addItem(itemWorld);
    QTransform transform;
    transform.translate(-180., -90.);
    transform.scale(360./pixmap.width(), 180./pixmap.height()); // applied first
    itemWorld->setTransform(transform);

    // clip
    QGraphicsRectItem* clipRect = new QGraphicsRectItem(-180., -90., 360., 180.);
    clipRect->setFlag(QGraphicsItem::ItemClipsChildrenToShape);
    clipRect->setPen(Qt::NoPen);
    scene->addItem(clipRect);

    // shape
    m_itemShadow = new QGraphicsPolygonItem(clipRect);
    m_itemShadow->setPen(QPen(QColor("#829d82"), 1.));
    m_itemShadow->setBrush(QColor("#90246e5a")); // ARGB

    // location
    pixmap.load(":/images/scene/nodeLocation.png");
    m_itemLocation = new QGraphicsPixmapItem(pixmap);
    scene->addItem(m_itemLocation);
    transform.reset();
    transform.translate(0., -10.);
    transform.scale(10./pixmap.width(), 10./pixmap.height());
    m_itemLocation->setTransform(transform);
    m_itemLocation->setPos(m_longitude, -m_latitude);

    // sun
//    pixmap.load(":/images/map/sun.png");
    pixmap.load(":/images/scene/nodeSun.png");
    m_itemSun = new QGraphicsPixmapItem(pixmap);
    scene->addItem(m_itemSun);
    transform.reset();
    transform.translate(-5., -5.);
    transform.scale(10./pixmap.width(), 10./pixmap.height());
    m_itemSun->setTransform(transform);
    m_itemSun->setPos(0, 0);

    // labels
//    QGraphicsTextItem* textItem = new QGraphicsTextItem("AAA");
//    scene->addItem(textItem);
//    textItem->setPos(60, 60);

    m_graphicsView = new QGraphicsView;
    m_graphicsView->setRenderHint(QPainter::Antialiasing);
    m_graphicsView->setRenderHint(QPainter::TextAntialiasing);
    m_graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);
    m_graphicsView->setFrameStyle(QFrame::NoFrame);
    m_graphicsView->setScene(scene);
    m_graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

    vLayout->addWidget(m_graphicsView);
}

WorldMap::~WorldMap()
{

}

void WorldMap::LatitudeChanged(double latitude)
{
    m_latitude = latitude;
    m_itemLocation->setPos(m_longitude, -m_latitude);
}

void WorldMap::LongitudeChanged(double longitude)
{
    m_longitude = longitude;
    m_itemLocation->setPos(m_longitude, -m_latitude);
}

void WorldMap::SunChanged(cSunCoordinates coordinates)
{
    m_rightAscension = coordinates.dRightAscension;
    m_declination = coordinates.dDeclination;
    double temp = coordinates.dHourAngle - m_longitude*gcf::degree;
    m_hourAngle = gcf::normalizeAngle(temp, -gcf::pi); // [0, 2pi]

    m_azimuth = coordinates.dAzimuth;
    m_zenith = coordinates.dZenithAngle;

    double sx = cos(m_declination)*cos(m_hourAngle);
    double sy = -cos(m_declination)*sin(m_hourAngle);
    double sz = sin(m_declination);
    if (sz == 0) sz = 1e-12;

    QPolygonF poly;
    for (int x = -180; x <= 180; x += 5)
    {
        double lon = x*gcf::degree;
        double lat = -atan((sx*cos(lon) + sy*sin(lon))/sz);
        poly << QPointF(x, -lat/gcf::degree);
    }
    double yClip = 92;
    if (m_declination < 0) yClip = -yClip;
    poly << QPointF(180, yClip);
    poly << QPointF(-180, yClip);

    m_itemSun->setPos(-m_hourAngle/gcf::degree, -m_declination/gcf::degree);
    m_itemShadow->setPolygon(poly);
}

void WorldMap::resizeEvent(QResizeEvent*)
{
    m_graphicsView->viewport()->resize(0.95*m_graphicsView->size());
    QRectF rect = m_graphicsView->scene()->sceneRect();
    rect = QRectF(-185., -95., 370., 190.);
//    m_graphicsView->fitInView(rect, Qt::KeepAspectRatio);
    m_graphicsView->fitInView(rect, Qt::KeepAspectRatioByExpanding);
}

/*
void WorldMap::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    // draw text
    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap));
    // painter.setFont( QFont("MS Shell Dlg 2", 8) );
    QString azimuth = QString("Azimuth: %1").arg(m_azimuth);
    painter.drawText(20, 280, azimuth);
    QString zenith = QString("Zenith: %1").arg(m_zenith);
    painter.drawText(20, 310, zenith);
    QString ascension = QString("Right Ascension: %1").arg(m_rightAscension);
    painter.drawText(20, 340, ascension);
    QString declination = QString("Declination: %1").arg(m_declination);
    painter.drawText(20, 370, declination);
}
*/

//void WorldMap::mouseMoveEvent(QMouseEvent* e)
//{
//    int x = e->x();
//    int y = e->y();

//    if (x < m_map.width() && y < m_map.height())
//    {
//        // tool tip
//        QString country = m_gray->country(x, y);
//        if (country != "EMPTY")
//            QToolTip::showText(e->globalPos(), country);
//        else
//            QToolTip::showText(e->globalPos(), "");
//    }
//}
