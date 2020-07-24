#include "GroundGrid.h"

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoText3.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/sensors/SoNodeSensor.h>

#include <QVector>


GroundGrid::GroundGrid()
{

}

SoGroup* GroundGrid::makeGrid(
    double step, int divs,
    double xMin, double xMax,
    double yMin, double yMax
)
{
    SoSwitch* ans = new SoSwitch;
    ans->setName("grid");

    SoTransform* transform = new SoTransform;
    transform->translation.setValue(0., 0., -0.001);
    ans->addChild(transform);

    double dx = step;

    int nxMin = floor(xMin/dx);
    int nxMax = ceil(xMax/dx);
    xMin = nxMin*dx;
    xMax = nxMax*dx;

    int nyMin = floor(yMin/dx);
    int nyMax = ceil(yMax/dx);
    yMin = nyMin*dx;
    yMax = nyMax*dx;

    // points
    QVector<SbVec3f> pointsMajor;
    QVector<SbVec3f> pointsMinor;
    QVector<int> sizesMajor;
    QVector<int> sizesMinor;

    for (int n = nxMin; n <= nxMax; ++n) {
        double x = n*dx;
        if (n % divs == 0) {
            if (n == 0) continue;
            pointsMajor << SbVec3f(x, yMin, 0.);
            pointsMajor << SbVec3f(x, yMax, 0.);
            sizesMajor << 2;
        } else {
            pointsMinor << SbVec3f(x, yMin, 0.);
            pointsMinor << SbVec3f(x, yMax, 0.);
            sizesMinor << 2;
        }
    }

    for (int n = nyMin; n <= nyMax; ++n) {
        double y = n*dx;
        if (n % divs == 0) {
            if (n == 0) continue;
            pointsMajor << SbVec3f(xMin, y, 0.);
            pointsMajor << SbVec3f(xMax, y, 0.);
            sizesMajor << 2;
        } else {
            pointsMinor << SbVec3f(xMin, y, 0.);
            pointsMinor << SbVec3f(xMax, y, 0.);
            sizesMinor << 2;
        }
    }

    // major grid
    SoMaterial* sMaterial = new SoMaterial;
    sMaterial->diffuseColor.setValue(0., 0., 0.);
    sMaterial->transparency = 0.8;
    ans->addChild(sMaterial);

    SoCoordinate3* sPoints = new SoCoordinate3;
    sPoints->point.setValues(0, pointsMajor.size(), pointsMajor.data());
    ans->addChild(sPoints);

    SoLineSet* sLines = new SoLineSet;
    sLines->numVertices.setValues(0, sizesMajor.size(), sizesMajor.data());
    ans->addChild(sLines);

    // minor grid
    sMaterial = new SoMaterial;
    sMaterial->diffuseColor.setValue(0., 0., 0.);
    sMaterial->transparency = 0.95;
    ans->addChild(sMaterial);

    sPoints = new SoCoordinate3;
    sPoints->point.setValues(0, pointsMinor.size(), pointsMinor.data());
    ans->addChild(sPoints);

    sLines = new SoLineSet;
    sLines->numVertices.setValues(0, sizesMinor.size(), sizesMinor.data());
    ans->addChild(sLines);

    // axes
    ans->addChild( makeAxes(xMin - dx, xMax + dx, yMin - dx, yMax + dx) );

    return ans;
}

SoGroup* GroundGrid::makeAxes(double xMin, double xMax, double yMin, double yMax)
{
    SoSeparator* ans = new SoSeparator;

    // axes
    QVector<SbVec3f> points;
    QVector<int> sizes;

    points << SbVec3f(xMin, 0., 0.);
    points << SbVec3f(xMax, 0., 0.);
    sizes << 2;

    points << SbVec3f(0., yMin, 0.);
    points << SbVec3f(0., yMax, 0.);
    sizes << 2;

    SoMaterial* sMaterial = new SoMaterial;
    sMaterial->diffuseColor.setValue(0., 0., 0.);
    sMaterial->transparency = 0.7;
    ans->addChild(sMaterial);

    SoDrawStyle* sStyle = new SoDrawStyle;
    sStyle->lineWidth = 1.5;
    ans->addChild(sStyle);

    SoCoordinate3* sPoints = new SoCoordinate3;
    sPoints->point.setValues(0, points.size(), points.data());
    ans->addChild(sPoints);

    SoLineSet* sLines = new SoLineSet;
    sLines->numVertices.setValues(0, sizes.size(), sizes.data());
    ans->addChild(sLines);

    return ans;
}
