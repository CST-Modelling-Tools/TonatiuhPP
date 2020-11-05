#include "ShapeMesh.h"

#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <QFileInfo>
#include <QMessageBox>

#include "kernel/profiles/ProfileRT.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/math/3D/Box3D.h"
#include "libraries/math/3D/Ray.h"
#include "libraries/auxiliary/tiny_obj_loader.h"
#include "kernel/node/TonatiuhFunctions.h"
using gcf::pow2;

SO_NODE_SOURCE(ShapeMesh)


void ShapeMesh::initClass()
{
    SO_NODE_INIT_CLASS(ShapeMesh, ShapeRT, "ShapeRT");
}

ShapeMesh::ShapeMesh()
{  
    m_bvh = 0;
    SO_NODE_CONSTRUCTOR(ShapeMesh);

    float vs[][3] = {
        {1., 0., 0.},
        {0., 1., 0.},
        {0., 0., 0.}
    };
    vertices.setValues(0, 3, vs);
    vertices.setContainer(this);
//    fieldData->addField(this, "vertices", &vertices);

    float ns[][3] = {
        {0., 0., 1.},
        {0., 0., 1.},
        {0., 0., 1.}
    };
    normals.setValues(0, 3, ns);
    normals.setContainer(this);
//    fieldData->addField(this, "normals", &normals);

    int fvs[] = {
        0, 1, 2, -1
    };
    facesVertices.setValues(0, 4, fvs);
    facesVertices.setContainer(this);
//    fieldData->addField(this, "facesVertices", &facesVertices);

    int fns[] = {
        0, 1, 2, -1
    };
    facesNormals.setValues(0, 4, fns);
    facesNormals.setContainer(this);
//    fieldData->addField(this, "facesNormals", &facesNormals);

    SO_NODE_ADD_FIELD( file, ("") );

    m_sensor = QSharedPointer<SoFieldSensor>::create(onSensor, this);
    m_sensor->attach(&file);
    onSensor(this, 0);
}

Box3D ShapeMesh::getBox(ProfileRT* profile) const
{
    Q_UNUSED(profile)
    if (m_bvh) return m_bvh->box();
    return Box3D();
}

bool ShapeMesh::intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* profile) const
{  
    Q_UNUSED(profile)
    if (!m_bvh) return false;
    double tHitT = ray.tMax;
    DifferentialGeometry dgT;
    if (!m_bvh->intersect(ray, &tHitT, &dgT)) return false;

    if (tHit == 0 && dg == 0) return true;
    if (tHit == 0 || dg == 0) gcf::SevereError( "ShapeMesh::intersect");

    *tHit = tHitT;
    *dg = dgT;
    dg->shape = this;
    return true;
}

#include "kernel/scene/MaterialGL.h"
void ShapeMesh::updateShapeGL(TShapeKit* parent)
{
    MaterialGL* mGL = (MaterialGL*) parent->material.getValue();
    bool reverseNormals = mGL->reverseNormals.getValue();

    SoShapeKit* shapeKit = parent->m_shapeKit;

    SoCoordinate3* sVertices = new SoCoordinate3;
    sVertices->point.setValues(0, vertices.getNum(), vertices.getValues(0));
    shapeKit->setPart("coordinate3", sVertices);

//    SoShapeHints* sHints = new SoShapeHints;
//    sHints->shapeType = SoShapeHints::SOLID;
//    sHints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
//    sHints->creaseAngle = 30*gcf::degree;
//    shapeKit->setPart("shapeHints", sHints);

    SoNormal* sNormals = new SoNormal;
    if (reverseNormals) {
        SoMFVec3f temp;
        temp.setValues(0, normals.getNum(), normals.getValues(0));
        for (int n = 0; n < normals.getNum(); ++n) {
            const SbVec3f& v = *temp.getValues(n);
            temp.set1Value(n, -v[0], -v[1], -v[2]);
        }
        sNormals->vector.setValues(0, temp.getNum(), temp.getValues(0));
    } else
        sNormals->vector.setValues(0, normals.getNum(), normals.getValues(0));
    shapeKit->setPart("normal", sNormals);

//    SoNorm
    SoIndexedFaceSet* sMesh = new SoIndexedFaceSet;
    sMesh->coordIndex.setValues(0, facesVertices.getNum(), facesVertices.getValues(0));
    sMesh->normalIndex.setValues(0, facesNormals.getNum(), facesNormals.getValues(0));

    shapeKit->setPart("shape", sMesh);
}

ShapeMesh::~ShapeMesh()
{
    delete m_bvh;
    for (Triangle* t : m_triangles)
        delete t;
}

void ShapeMesh::onSensor(void* data, SoSensor*)
{
    ShapeMesh* shape = (ShapeMesh*) data;
    QString fileName = shape->file.getValue().getString();
    if (fileName.isEmpty()) return;

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
    shape->vertices.setNum(nMax);
    for (int n = 0, m = 0; n < nMax; n++, m += 3)
        shape->vertices.set1Value(n, &attrib.vertices[m]);

    nMax = attrib.normals.size()/3;
    shape->normals.setNum(nMax);
    for (int n = 0, m = 0; n < nMax; n++, m += 3) {
//        shape->normals.set1Value(n, &attrib.normals[m]);
        SbVec3f nv(&attrib.normals[m]);
        nv.normalize();
        shape->normals.set1Value(n, nv);
    }

    nMax = mesh.num_face_vertices.size()*4;
    shape->facesVertices.setNum(nMax);
    shape->facesNormals.setNum(nMax);
    int n = 0;
    size_t v0 = 0;
    for (size_t f = 0; f < mesh.num_face_vertices.size(); f++) {
        uchar vMax = mesh.num_face_vertices[f]; // 3?
        for (size_t v = 0; v < vMax; v++) {
            const tinyobj::index_t& index = mesh.indices[v0 + v];
            shape->facesVertices.set1Value(n, index.vertex_index);
            shape->facesNormals.set1Value(n, index.normal_index);
            n++;
        }
        shape->facesVertices.set1Value(n, -1);
        shape->facesNormals.set1Value(n, -1);
        n++;
        v0 += vMax;
    }


    for (Triangle* t : shape->m_triangles)
        delete t;
    shape->m_triangles.clear();

    v0 = 0;
    for (size_t f = 0; f < mesh.num_face_vertices.size(); f++) {
        int vMax = mesh.num_face_vertices[f];
        const tinyobj::index_t& i0 = mesh.indices[v0];
        const tinyobj::index_t& i1 = mesh.indices[v0 + 1];
        const tinyobj::index_t& i2 = mesh.indices[v0 + 2];

        vec3d vA(&attrib.vertices[3*i0.vertex_index]);
        vec3d vB(&attrib.vertices[3*i1.vertex_index]);
        vec3d vC(&attrib.vertices[3*i2.vertex_index]);
        vec3d nA(&attrib.normals[3*i0.normal_index]);
        vec3d nB(&attrib.normals[3*i1.normal_index]);
        vec3d nC(&attrib.normals[3*i2.normal_index]);
        Triangle* triangle = new Triangle(vA, vB, vC, nA, nB, nC);
        shape->m_triangles.push_back(triangle);
        v0 += vMax;
    }

    if (shape->m_bvh) delete shape->m_bvh;
    shape->m_bvh = new BVH(&shape->m_triangles, 1);
}
