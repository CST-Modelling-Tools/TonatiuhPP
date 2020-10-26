#include "GridNode3D.h"

#include <QVector>

#include <Inventor/nodes/SoSeparator.h>

#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodekits/SoShapeKit.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoPolygonOffset.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>

#include <Inventor/nodes/SoShaderProgram.h>

#include <Inventor/sensors/SoNodeSensor.h>


GridNode3D::GridNode3D()
{
    m_grid = new SoSwitch;
    m_grid->setName("grid");
    addChild(m_grid);

    m_sensor = new SoNodeSensor(update, this);
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
    // grid parameters
    GridNode* grid = (GridNode*) m_sensor->getAttachedNode();
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

    // init
    m_grid->removeAllChildren();
    m_grid->whichChild = grid->show.getValue() ? SO_SWITCH_ALL : SO_SWITCH_NONE;
//    SoTransform* transform = new SoTransform;
//    transform->translation.setValue(0., 0., 0*-0.01);
//    m_grid->addChild(transform);

    // ground
    if (grid->fill.getValue())
        makeGround(xMin, xMax, yMin, yMax);

    // terrain
    QString fileName = grid->file.getValue().getString();
    if (!fileName.isEmpty())
        makeTerrain(fileName);

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

void GridNode3D::makeGround(double xMin, double xMax, double yMin, double yMax)
{
    SoShapeKit* kit = new SoShapeKit;

    float sv[][3] = {
        {float(xMin), float(yMin), 0.},
        {float(xMax), float(yMin), 0.},
        {float(xMax), float(yMax), 0.},
        {float(xMin), float(yMax), 0.}
    };
    SoCoordinate3* sVertices = new SoCoordinate3;
    sVertices->point.setValues(0, 4, sv);
    kit->setPart("coordinate3", sVertices);

    int faces[] = {
        0, 1, 2, 3, -1
    };
    SoIndexedFaceSet* sMesh = new SoIndexedFaceSet;
    sMesh->coordIndex.setValues(0, 5, faces);
    kit->setPart("shape", sMesh);

    SoMaterial* material = new SoMaterial;
    material->ambientColor = SbColor(0.85, 0.85, 0.8);
    material->diffuseColor = SbColor(0.05, 0.05, 0.05);
    material->specularColor = SbColor(0.02, 0.02, 0.02);
    material->emissiveColor = SbColor(0, 0, 0);
    material->shininess = 0.1;
//    material->transparency = 0.;
    kit->setPart("material", material);

    // shift fill from grid
    SoPolygonOffset* po = new SoPolygonOffset;
    po->styles = SoPolygonOffset::FILLED;
    po->factor = 1.;
    po->units = 1.;
    m_grid->addChild(po);

    // cull from bottom
    SoShapeHints* hints = new SoShapeHints;
    hints->shapeType = SoShapeHints::SOLID;
    hints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
    kit->setPart("shapeHints", hints);

    m_grid->addChild(kit);
}

#include "libraries/auxiliary/tiny_obj_loader.h"
#include <QFileInfo>
#include <QMessageBox>
void GridNode3D::makeTerrain(QString fileName)
{
    fileName = QString("project:") + fileName;
    QFileInfo info(fileName);
    if (info.suffix() != "obj") {
        QMessageBox::warning(0, "Warning", "File in not in obj-format");
        return;
    }
    if (!info.exists()) {
        QMessageBox::warning(0, "Warning", QString("File not found:\n") + fileName);
        return;
    }
    fileName = info.absoluteFilePath();

    SoMFVec3f vertices;
    SoMFVec3f normals;
    SoMFInt32 facesVertices;
    SoMFInt32 facesNormals;

    // import
    std::string inputfile = fileName.toStdString();
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str());
    if (!ret) return;
    if (shapes.empty()) return;
    tinyobj::mesh_t& mesh = shapes[0].mesh;

    // conversion
    int nMax = attrib.vertices.size()/3;
    vertices.setNum(nMax);
    for (int n = 0, m = 0; n < nMax; n++, m += 3)
        vertices.set1Value(n, &attrib.vertices[m]);

    nMax = attrib.normals.size()/3;
    normals.setNum(nMax);
    for (int n = 0, m = 0; n < nMax; n++, m += 3) {
//        shape->normals.set1Value(n, &attrib.normals[m]);
        SbVec3f nv(&attrib.normals[m]);
        nv.normalize();
        normals.set1Value(n, nv);
    }

    nMax = mesh.num_face_vertices.size()*4;
    facesVertices.setNum(nMax);
    facesNormals.setNum(nMax);
    int n = 0;
    size_t v0 = 0;
    for (size_t f = 0; f < mesh.num_face_vertices.size(); f++) {
        uchar vMax = mesh.num_face_vertices[f]; // 3?
        for (size_t v = 0; v < vMax; v++) {
            const tinyobj::index_t& index = mesh.indices[v0 + v];
            facesVertices.set1Value(n, index.vertex_index);
            facesNormals.set1Value(n, index.normal_index);
            n++;
        }
        facesVertices.set1Value(n, -1);
        facesNormals.set1Value(n, -1);
        n++;
        v0 += vMax;
    }

    // shape
    SoShapeKit* kit = new SoShapeKit;

    SoMaterial* material = new SoMaterial;
    material->ambientColor = SbColor(0.85, 0.85, 0.8);
    material->diffuseColor = SbColor(0.05, 0.05, 0.05);
    material->specularColor = SbColor(0.02, 0.02, 0.02);
    material->emissiveColor = SbColor(0, 0, 0);
    material->shininess = 0.1;
//    material->transparency = 0.;
    kit->setPart("material", material);

    SoCoordinate3* sVertices = new SoCoordinate3;
    sVertices->point.setValues(0, vertices.getNum(), vertices.getValues(0));
    kit->setPart("coordinate3", sVertices);

//    SoShapeHints* sHints = new SoShapeHints;
//    sHints->shapeType = SoShapeHints::SOLID;
//    sHints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
//    sHints->creaseAngle = 30*gcf::degree;
//    shapeKit->setPart("shapeHints", sHints);

    SoNormal* sNormals = new SoNormal;
    sNormals->vector.setValues(0, normals.getNum(), normals.getValues(0));
    kit->setPart("normal", sNormals);

    SoIndexedFaceSet* sMesh = new SoIndexedFaceSet;
    sMesh->coordIndex.setValues(0, facesVertices.getNum(), facesVertices.getValues(0));
    sMesh->normalIndex.setValues(0, facesNormals.getNum(), facesNormals.getValues(0));
    kit->setPart("shape", sMesh);

    SoSeparator* sep = new SoSeparator;
    sep->addChild(kit);

    m_grid->addChild(sep);
}

void GridNode3D::update(void* data, SoSensor*)
{
    GridNode3D* node = (GridNode3D*) data;
    node->create();
}
