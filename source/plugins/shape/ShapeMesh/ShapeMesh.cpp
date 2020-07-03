#include "ShapeMesh.h"

#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <QFileInfo>

#include "kernel/profiles/ProfileRT.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/math/3D/Box3D.h"
#include "libraries/math/3D/Ray.h"
#include "libraries/auxiliary/tiny_obj_loader.h"
#include "kernel/TonatiuhFunctions.h"
using gcf::pow2;

SO_NODE_SOURCE(ShapeMesh)


void ShapeMesh::initClass()
{
    SO_NODE_INIT_CLASS(ShapeMesh, ShapeRT, "ShapeRT");
}

ShapeMesh::ShapeMesh()
{
    SO_NODE_CONSTRUCTOR(ShapeMesh);

    float vs[][3] = {
        {1., 0., 0.},
        {0., 1., 0.},
        {0., 0., 0.}
    };
    vertices.setValues(0, 3, vs);
    vertices.setContainer(this);
    fieldData->addField(this, "vertices", &vertices);

    float ns[][3] = {
        {0., 0., 1.},
        {0., 0., 1.},
        {0., 0., 1.}
    };
    normals.setValues(0, 3, ns);
    normals.setContainer(this);
    fieldData->addField(this, "normals", &normals);

    int fvs[] = {
        0, 1, 2, -1
    };
    facesVertices.setValues(0, 4, fvs);
    facesVertices.setContainer(this);
    fieldData->addField(this, "facesVertices", &facesVertices);

    int fns[] = {
        0, 1, 2, -1
    };
    facesNormals.setValues(0, 4, fvs);
    facesNormals.setContainer(this);
    fieldData->addField(this, "facesNormals", &facesNormals);

    SO_NODE_ADD_FIELD( file, ("") );

    m_sensor = QSharedPointer<SoFieldSensor>::create(onSensor, this);
    m_sensor->attach(&file);
    onSensor(this, 0);
}

Box3D ShapeMesh::getBox(ProfileRT* profile) const
{
    Q_UNUSED(profile)
    return m_box;
}

bool ShapeMesh::intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* profile) const
{
    // r0_z + d_z*t = 0
    double t = -ray.origin.z*ray.invDirection().z;

    if (t < ray.tMin + 1e-5 || t > ray.tMax) return false;

    vec3d pHit = ray.point(t);
    if (!profile->isInside(pHit.x, pHit.y)) return false;

    if (tHit == 0 && dg == 0)
        return true;
    else if (tHit == 0 || dg == 0)
        gcf::SevereError("ShapePlanar::intersect");

    *tHit = t;
    dg->point = pHit;
    dg->u = pHit.x;
    dg->v = pHit.y;
    dg->dpdu = vec3d(1., 0., 0.);
    dg->dpdv = vec3d(0., 1., 0.);
    dg->normal = vec3d(0., 0., 1.);
    dg->shape = this;
    dg->isFront = dot(dg->normal, ray.direction()) <= 0.;
    return true;
}

void ShapeMesh::updateShapeGL(TShapeKit* parent)
{
    SoCoordinate3* sVertices = new SoCoordinate3;
    sVertices->point.setValues(0, vertices.getNum(), vertices.getValues(0));
    parent->setPart("coordinate3", sVertices);

    SoNormal* sNormals = new SoNormal;
    sNormals->vector.setValues(0, normals.getNum(), normals.getValues(0));
    parent->setPart("normal", sNormals);

    SoIndexedFaceSet* sMesh = new SoIndexedFaceSet;
    sMesh->coordIndex.setValues(0, facesVertices.getNum(), facesVertices.getValues(0));
    sMesh->normalIndex.setValues(0, facesNormals.getNum(), facesNormals.getValues(0));

    parent->setPart("shape", sMesh);

    m_box = Box3D();
    for (int n = 0; n < vertices.getNum(); ++n) {
       const SbVec3f& v = *vertices.getValues(n);
        m_box << tgf::makeVector3D(v);
    }
}

void ShapeMesh::onSensor(void* data, SoSensor*)
{
    ShapeMesh* shapeMesh = (ShapeMesh*) data;
    QString fileName = shapeMesh->file.getValue().getString();

    if (fileName.isEmpty()) return;
    QFileInfo info(fileName);
    if (info.suffix() != "obj") return;

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

    int nMax = attrib.vertices.size()/3;
    shapeMesh->vertices.setNum(nMax);
    for (int n = 0, m = 0; n < nMax; n++, m += 3)
        shapeMesh->vertices.set1Value(n, &attrib.vertices[m]);

    nMax = attrib.normals.size()/3;
    shapeMesh->normals.setNum(nMax);
    for (int n = 0, m = 0; n < nMax; n++, m += 3)
        shapeMesh->normals.set1Value(n, &attrib.normals[m]);

    nMax = mesh.num_face_vertices.size()*4;
    shapeMesh->facesVertices.setNum(nMax);
    shapeMesh->facesNormals.setNum(nMax);
    int m = 0;
    size_t v0 = 0;
    for (size_t f = 0; f < mesh.num_face_vertices.size(); f++) {
        int vMax = mesh.num_face_vertices[f];
        for (size_t v = 0; v < vMax; v++) {
            tinyobj::index_t idx = mesh.indices[v0 + v];
            shapeMesh->facesVertices.set1Value(m, idx.vertex_index);
            shapeMesh->facesNormals.set1Value(m, idx.normal_index);
            m++;
        }
        shapeMesh->facesVertices.set1Value(m, -1);
        shapeMesh->facesNormals.set1Value(m, -1);
        m++;
        v0 += vMax;
    }
}
