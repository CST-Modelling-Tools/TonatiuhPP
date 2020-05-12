#include <QImage>
#include <QBrush>
#include <QColor>
#include <QImage>
#include <QPaintEngine>

#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoLabel.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodekits/SoNodekitCatalog.h>

#include "libraries/geometry/gc.h"
#include "libraries/geometry/BoundingBox.h"
#include "libraries/geometry/Matrix4x4.h"
#include "libraries/geometry/Point3D.h"
#include "sun/sunpos.h"
#include "sun/SunPillbox.h"
#include "TLightKit.h"
#include "TLightShape.h"
#include "libraries/geometry/Transform.h"
#include "scene/TShapeKit.h"
#include "kernel/TonatiuhFunctions.h"



SO_KIT_SOURCE(TLightKit)

/**
 * Initializates TLightKit componets
 */
void TLightKit::initClass()
{
    SO_KIT_INIT_CLASS(TLightKit, SoLightKit, "LightKit");
}

/**
 * Creates a new TLightKit.
 */
TLightKit::TLightKit()
{
    SO_KIT_CONSTRUCTOR(TLightKit);

    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(iconMaterial, SoNode, SoMaterial, TRUE, iconSeparator, icon, TRUE);
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(iconTexture, SoNode, SoTexture2, TRUE, iconSeparator, iconMaterial, TRUE);
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(tsunshape, SunAbstract, SunPillbox, TRUE, transformGroup, "", TRUE);

    SO_NODE_ADD_FIELD(disabledNodes, ("") );

    SO_KIT_INIT_INSTANCE();

    SoDirectionalLight* light = static_cast<SoDirectionalLight*>(getPart("light", true) );
    light->direction.setValue(SbVec3f(0, 0, 1) );

    SoTransform* transform = new SoTransform;
    setPart("transform", transform);

    SoMaterial* lightMaterial = static_cast<SoMaterial*>(getPart("iconMaterial", true) );
    lightMaterial->transparency = 0.75f;
    setPart("iconMaterial", lightMaterial);

    int widthPixeles = 10;
    int heightPixeles = 10;
    QVector<uchar> bitmap(widthPixeles * heightPixeles);
    bitmap.fill(255);

    SoTexture2* texture = new SoTexture2;
    texture->image.setValue(SbVec2s(heightPixeles, widthPixeles), 1, bitmap.data());
    texture->model = SoTexture2::BLEND;
    texture->blendColor.setValue(1.f, 1.f, 1.f);
    setPart("iconTexture", texture);

    TLightShape* iconShape = new TLightShape;
    setPart("icon", iconShape);

    setName("Light");
    setPosition(0., gc::Pi/2.);
}

/**
 * Destructor.
 */
TLightKit::~TLightKit()
{
    //void ChangePosition( QDateTime time, double longitude, double latitude );
    //void SetDateTime( QDateTime time );
}

/*!
 * Changes the light position to \a azimuth and \a zenith from the scene centre.
 * Azimuth and Zenith are in radians.
 * \sa redo().
 */
void TLightKit::setPosition(double azimuth, double elevation)
{
    SoTransform* transform = (SoTransform*) getPart("transform", false);

    SbRotation elRotation(SbVec3f(1., 0., 0.), gc::Pi/2. + elevation);
    SbRotation azRotation(SbVec3f(0., 0., -1.), azimuth);

    transform->rotation = elRotation*azRotation;
}

void TLightKit::setBox(BoundingBox box)
{
    SoTransform* transform = (SoTransform*) getPart("transform", false);
    SbMatrix mr;
    mr.setRotate(transform->rotation.getValue());

    Transform tr = tgf::TransformFromMatrix(mr).GetInverse();
    box = tr(box);


    //box is global

    double xMax = box.pMax.x;
    double xMin = box.pMin.x;
    double yMin = box.pMin.y;
    double yMax = box.pMax.y;

    double distMax = box.pMax.z + 10. - box.pMin.z;
    double back = box.pMin.z - 10.;

    if (-gc::Infinity == box.Volume() )
    {
        xMin = 0.;
        xMax = 0.;
        yMin = 0.;
        yMax = 0.;
        distMax = 0.;
    }

    double delta = 0.01;
    SunAbstract* sunshape = static_cast<SunAbstract*>(getPart("tsunshape", false));
    if (!sunshape) return;
    double thetaMax = sunshape->getThetaMax();
    if (thetaMax > 0.)
        delta = distMax*tan(thetaMax);

    TLightShape* shape = static_cast<TLightShape*>(getPart("icon", false));
    if (!shape) return;
    shape->xMin.setValue(xMin - delta);
    shape->xMax.setValue(xMax + delta);
    shape->yMin.setValue(yMin - delta);
    shape->yMax.setValue(yMax + delta);
    shape->delta.setValue(delta);

    if (!transform) return;
    SbVec3f res;
    mr.multVecMatrix(SbVec3f(0., 0., back), res);
    transform->translation = res;
}

void TLightKit::findTexture(int xPixels, int yPixels, QVector< QPair< TShapeKit*, Transform > > surfacesList)
{
    SoTransform* transform = (SoTransform*) getPart("transform", false);
    SbMatrix mr;
    mr.setRotate(transform->rotation.getValue());
    Transform tr = tgf::TransformFromMatrix(mr).GetInverse();

    TLightShape* shape = static_cast<TLightShape*>(getPart("icon", false));
    if (!shape) return;

    double xWidth = shape->xMax.getValue() - shape->xMin.getValue();
    double yWidth = shape->yMax.getValue() - shape->yMin.getValue();

    while ( xWidth / xPixels < shape->delta.getValue() ) xPixels--;
    double xWidthPixel = xWidth/xPixels;

    while ( yWidth / yPixels < shape->delta.getValue() ) yPixels--;
    double yWidthPixel = yWidth/yPixels;

    QImage* sourceImage = new QImage(xPixels, yPixels, QImage::Format_RGB32);
    sourceImage->setOffset(QPoint(0.5, 0.5));
    sourceImage->fill(Qt::white);

    QPainter painter(sourceImage);

    QBrush brush(Qt::black);
    painter.setBrush(brush);

    QPen pen(Qt::black);
    painter.setPen(pen);

    //painter.setRenderHint(   QPainter::Antialiasing);

    //QPen pen( Qt::black, Qt::NoPen );
    //painter.setPen( pen );

    for (int s = 0; s < surfacesList.size(); s++)
    {
        TShapeKit* surfaceKit = surfacesList[s].first;
        Transform surfaceTransform = surfacesList[s].second;
        Transform transformOtW = surfaceTransform.GetInverse();

        ShapeAbstract* shapeNode = static_cast<ShapeAbstract*>(surfaceKit->getPart("shape", false));
        if (shapeNode)
        {
            BoundingBox shapeBox = shapeNode->getBox();

            QVector<Point3D> ps;
            ps << Point3D(shapeBox.pMin.x, shapeBox.pMin.y, shapeBox.pMin.z);
            ps << Point3D(shapeBox.pMax.x, shapeBox.pMin.y, shapeBox.pMin.z);
            ps << Point3D(shapeBox.pMax.x, shapeBox.pMin.y, shapeBox.pMax.z);
            ps << Point3D(shapeBox.pMin.x, shapeBox.pMin.y, shapeBox.pMax.z);
            ps << Point3D(shapeBox.pMin.x, shapeBox.pMax.y, shapeBox.pMin.z);
            ps << Point3D(shapeBox.pMax.x, shapeBox.pMax.y, shapeBox.pMin.z);
            ps << Point3D(shapeBox.pMax.x, shapeBox.pMax.y, shapeBox.pMax.z);
            ps << Point3D(shapeBox.pMin.x, shapeBox.pMax.y, shapeBox.pMax.z);

            QVector<QPointF> qps;
            for (Point3D& p : ps) {
                p = transformOtW(p);
                p = tr(p);
                qps <<  QPoint((p.x - shape->xMin.getValue())/xWidthPixel, (p.y - shape->yMin.getValue())/yWidthPixel);
            }

            QPolygonF polygon1({qps[0], qps[1], qps[2], qps[3]});
            QPolygonF polygon2({qps[0], qps[1], qps[5], qps[4]});
            QPolygonF polygon3({qps[0], qps[3], qps[7], qps[4]});
            QPolygonF polygon4({qps[1], qps[2], qps[6], qps[5]});
            QPolygonF polygon5({qps[2], qps[3], qps[7], qps[6]});
            QPolygonF polygon6({qps[4], qps[5], qps[6], qps[7]});

            painter.drawPolygon(polygon1);
            painter.drawPolygon(polygon2);
            painter.drawPolygon(polygon3);
            painter.drawPolygon(polygon4);
            painter.drawPolygon(polygon5);
            painter.drawPolygon(polygon6);
        }
    }

    int** areaMatrix = new int*[yPixels];
    for (int i = 0; i < yPixels; i++)
        areaMatrix[i] = new int[xPixels];

    unsigned char* bitmap = new unsigned char[xPixels*yPixels];

    QRgb black = qRgb(0, 0, 0);

    for (int i = 0; i < xPixels; i++)
    {
        for (int j = 0; j < yPixels; j++)
        {
            int pixelIntensity = 0;
            for (int qi = i - 1; qi <= i + 1; ++qi)
            for (int qj = j - 1; qj <= j + 1; ++qj)
            {
                if (qi < 0 || qi >= xPixels) continue;
                if (qj < 0 || qj >= yPixels) continue;
                pixelIntensity += sourceImage->pixel(qi, qj) == black ? 1 : 0;
            }

            if (pixelIntensity > 0)
            {
                areaMatrix[j][i] = 1;
                bitmap[i*yPixels + j] = 0;
            }
            else
            {
                areaMatrix[j][i] = 0;
                bitmap[i*yPixels + j] = 255;
            }
        }
    }

//    SoTexture2* texture = static_cast< SoTexture2* >(getPart("iconTexture", true) );
//    texture->image.setValue(SbVec2s(heightPixeles, widthPixeles), 1, bitmap);
//    texture->wrapS = SoTexture2::CLAMP;
//    texture->wrapT = SoTexture2::CLAMP;

    delete[] bitmap;

    shape->SetLightSourceArea(yPixels, xPixels, areaMatrix);
}
