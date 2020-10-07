#include "GridNode3D.h"

#include <QVector>

#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoLineSet.h>

GridNode3D::GridNode3D()
{
    m_sensor = new SoNodeSensor(update, this);
    addChild(new SoLineSet);

    m_grid = new SoSwitch;
    m_grid->setName("grid");
    addChild(m_grid);
}

GridNode3D::~GridNode3D()
{
    delete m_sensor;
}

void GridNode3D::attach(GridNode* grid)
{
    m_sensor->detach();
    m_sensor->attach(grid);
    create();
}

void GridNode3D::create()
{
    GridNode* grid = (GridNode*) m_sensor->getAttachedNode();
    m_grid->removeAllChildren();

    double dx = grid->step.getValue();
    int divs = grid->divisions.getValue();

    double xMin = grid->xRange.getValue()[0];
    double xMax = grid->xRange.getValue()[1];
    int nxMin = floor(xMin/dx);
    int nxMax = ceil(xMax/dx);
    xMin = nxMin*dx;
    xMax = nxMax*dx;

    double yMin = grid->yRange.getValue()[0];
    double yMax = grid->yRange.getValue()[1];
    int nyMin = floor(yMin/dx);
    int nyMax = ceil(yMax/dx);
    yMin = nyMin*dx;
    yMax = nyMax*dx;

    SoTransform* transform = new SoTransform;
    transform->translation.setValue(0., 0., -0.01);
    m_grid->addChild(transform);


    // ground
    if (grid->fill.getValue())
        makeGround(xMin, xMax, yMin, yMax);

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
    m_grid->addChild(sMaterial);

    SoCoordinate3* sPoints = new SoCoordinate3;
    sPoints->point.setValues(0, pointsMajor.size(), pointsMajor.data());
    m_grid->addChild(sPoints);

    SoLineSet* sLines = new SoLineSet;
    sLines->numVertices.setValues(0, sizesMajor.size(), sizesMajor.data());
    m_grid->addChild(sLines);

    // minor grid
    sMaterial = new SoMaterial;
    sMaterial->diffuseColor.setValue(0., 0., 0.);
    sMaterial->transparency = 0.95;
    m_grid->addChild(sMaterial);

    sPoints = new SoCoordinate3;
    sPoints->point.setValues(0, pointsMinor.size(), pointsMinor.data());
    m_grid->addChild(sPoints);

    sLines = new SoLineSet;
    sLines->numVertices.setValues(0, sizesMinor.size(), sizesMinor.data());
    m_grid->addChild(sLines);

    // axes
    makeAxes(xMin - dx, xMax + dx, yMin - dx, yMax + dx);



    m_grid->whichChild = grid->show.getValue() ? SO_SWITCH_ALL : SO_SWITCH_NONE;
}

void GridNode3D::makeAxes(double xMin, double xMax, double yMin, double yMax)
{
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
    m_grid->addChild(sMaterial);

    SoDrawStyle* sStyle = new SoDrawStyle;
    sStyle->lineWidth = 1.5;
    m_grid->addChild(sStyle);

    SoCoordinate3* sPoints = new SoCoordinate3;
    sPoints->point.setValues(0, points.size(), points.data());
    m_grid->addChild(sPoints);

    SoLineSet* sLines = new SoLineSet;
    sLines->numVertices.setValues(0, sizes.size(), sizes.data());
    m_grid->addChild(sLines);
}

#include <Inventor/nodekits/SoShapeKit.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>

void GridNode3D::makeGround(double xMin, double xMax, double yMin, double yMax)
{
    SoShapeKit* shapeKit = new SoShapeKit;

    float z0 = -0.01;
    float sv[][3] = {
        {float(xMin), float(yMin), z0},
        {float(xMax), float(yMin), z0},
        {float(xMax), float(yMax), z0},
        {float(xMin), float(yMax), z0}
    };
    SoCoordinate3* sVertices = new SoCoordinate3;
    sVertices->point.setValues(0, 4, sv);
    shapeKit->setPart("coordinate3", sVertices);

    int faces[] = {
        0, 1, 2, 3, -1
    };
    SoIndexedFaceSet* sMesh = new SoIndexedFaceSet;
    sMesh->coordIndex.setValues(0, 5, faces);
    shapeKit->setPart("shape", sMesh);

    SoMaterial* material = new SoMaterial;
    shapeKit->setPart("material", material);
    material->ambientColor = SbColor(0.85, 0.85, 0.8);
    material->diffuseColor = SbColor(0.05, 0.05, 0.05);
    material->specularColor = SbColor(0.1, 0.1, 0.1);
    material->emissiveColor = SbColor(0, 0, 0);
    material->shininess = 0.1;
    material->transparency = 0.;

    SoShapeHints* hints = new SoShapeHints;
    hints->shapeType = SoShapeHints::SOLID;
    hints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
    shapeKit->setPart("shapeHints", hints);

    m_grid->addChild(shapeKit);
}

void GridNode3D::update(void* data, SoSensor*)
{
    GridNode3D* node = (GridNode3D*) data;
    node->create();
}
