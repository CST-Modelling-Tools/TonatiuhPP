#include "GridNode.h"

#include <QVector>

#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoLineSet.h>

SO_NODE_SOURCE(GridNode);


void GridNode::initClass()
{
    SO_NODE_INIT_CLASS(GridNode, SoNode, "Node");
}

GridNode::GridNode()
{
    SO_NODE_CONSTRUCTOR(GridNode);

    SO_NODE_ADD_FIELD(show, (TRUE) );
    SO_NODE_ADD_FIELD(step, (1.) );
    SO_NODE_ADD_FIELD(divisions, (5) );
    SO_NODE_ADD_FIELD(xRange, (SbVec2d(-10., 10.)) );
    SO_NODE_ADD_FIELD(yRange, (SbVec2d(-10., 10.)) );

    setName("Grid");

    m_sensor = new SoNodeSensor(update, this);
    m_sensor->attach(this);

    m_root = new SoSwitch;
    m_root->setName("grid");
    m_root->ref();
    create();
}

GridNode::~GridNode()
{
    delete m_sensor;
    m_root->unref();
}

void GridNode::create()
{
    m_root->removeAllChildren();

    double dx = step.getValue();
    int divs = divisions.getValue();

    double xMin = xRange.getValue()[0];
    double xMax = xRange.getValue()[1];
    int nxMin = floor(xMin/dx);
    int nxMax = ceil(xMax/dx);
    xMin = nxMin*dx;
    xMax = nxMax*dx;

    double yMin = yRange.getValue()[0];
    double yMax = yRange.getValue()[1];
    int nyMin = floor(yMin/dx);
    int nyMax = ceil(yMax/dx);
    yMin = nyMin*dx;
    yMax = nyMax*dx;

    SoTransform* transform = new SoTransform;
    transform->translation.setValue(0., 0., -0.01);
    m_root->addChild(transform);

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
    m_root->addChild(sMaterial);

    SoCoordinate3* sPoints = new SoCoordinate3;
    sPoints->point.setValues(0, pointsMajor.size(), pointsMajor.data());
    m_root->addChild(sPoints);

    SoLineSet* sLines = new SoLineSet;
    sLines->numVertices.setValues(0, sizesMajor.size(), sizesMajor.data());
    m_root->addChild(sLines);

    // minor grid
    sMaterial = new SoMaterial;
    sMaterial->diffuseColor.setValue(0., 0., 0.);
    sMaterial->transparency = 0.95;
    m_root->addChild(sMaterial);

    sPoints = new SoCoordinate3;
    sPoints->point.setValues(0, pointsMinor.size(), pointsMinor.data());
    m_root->addChild(sPoints);

    sLines = new SoLineSet;
    sLines->numVertices.setValues(0, sizesMinor.size(), sizesMinor.data());
    m_root->addChild(sLines);

    // axes
    m_root->addChild(makeAxes(xMin - dx, xMax + dx, yMin - dx, yMax + dx));

    m_root->whichChild = show.getValue() ? SO_SWITCH_ALL : SO_SWITCH_NONE;
}

SoSeparator* GridNode::makeAxes(double xMin, double xMax, double yMin, double yMax)
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

void GridNode::update(void* data, SoSensor*)
{
    GridNode* node = (GridNode*) data;
    node->create();
}
