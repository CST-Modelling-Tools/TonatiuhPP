#include "SunPosition.h"

#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoTransparencyType.h>

#include "libraries/math/gcf.h"

SO_NODE_SOURCE(SunPosition)


void SunPosition::initClass()
{
    SO_NODE_INIT_CLASS(SunPosition, SoNode, "Node");
}

SunPosition::SunPosition()
{
    SO_NODE_CONSTRUCTOR(SunPosition);
//    isBuiltIn = TRUE;
    SO_NODE_ADD_FIELD( azimuth, (0.) );
    SO_NODE_ADD_FIELD( elevation, (90.) );
    SO_NODE_ADD_FIELD( irradiance, (1000.) );

    m_root = new SoSeparator;
    m_root->ref();
    create();

    m_sensor = new SoNodeSensor(update, this);
    m_sensor->attach(this);
}

SunPosition::~SunPosition()
{
    delete m_sensor;
    m_root->unref();
}

vec3d SunPosition::getSunVector()
{
    double gamma = azimuth.getValue()*gcf::degree;
    double alpha = elevation.getValue()*gcf::degree;

    return vec3d(
        sin(gamma)*cos(alpha),
        cos(gamma)*cos(alpha),
        sin(alpha)
    );
}

void SunPosition::create()
{
    m_root->removeAllChildren();

    // Rz(-gamma) Rx(alpha)
    m_transform = new SoTransform;
    update(this, 0);
    m_root->addChild(m_transform);

    // from {0, 1, 0}
    SoTransform* sTransform = new SoTransform;
    sTransform->translation = SbVec3f(0., 1., 0.);
    sTransform->rotation = SbRotation(SbVec3f(1., 0., 0.), 90.*gcf::degree);
    m_root->addChild(sTransform);

//    SoShapeHints* hints = new SoShapeHints;
//    hints->shapeType = SoShapeHints::SOLID;
//    m_root->addChild(hints);

    SoMaterial* material = new SoMaterial;
    material->diffuseColor = SbVec3f(1, 1, 1);
    m_root->addChild(material);

    SoTexture2* texture = new SoTexture2;
    texture->filename.setValue("../images/sun.png"); // relative to exe file
    texture->model = SoTexture2::MODULATE;
    m_root->addChild(texture);

    SoTextureCoordinate2* tCoords = new SoTextureCoordinate2;
    tCoords->point.set1Value(0, SbVec2f(0, 0));
    tCoords->point.set1Value(1, SbVec2f(1, 0));
    tCoords->point.set1Value(2, SbVec2f(1, 1));
    tCoords->point.set1Value(3, SbVec2f(0, 1));
    m_root->addChild(tCoords);

    SoTextureCoordinateBinding* tBind = new SoTextureCoordinateBinding;
    tBind->value.setValue(SoTextureCoordinateBinding::PER_VERTEX);
    m_root->addChild(tBind);

    SoCoordinate3* sCoords = new SoCoordinate3;
    double s = 0.05;
    sCoords->point.set1Value(0, SbVec3f(-s, -s, 0.));
    sCoords->point.set1Value(1, SbVec3f( s, -s, 0.));
    sCoords->point.set1Value(2, SbVec3f( s, s, 0.));
    sCoords->point.set1Value(3, SbVec3f(-s, s, 0.));
    m_root->addChild(sCoords);

//    SoNormal* normal = new SoNormal;
//    normal->vector.set1Value(0, SbVec3f(0, 0, 1));
//    m_root->addChild(normal);

//    SoNormalBinding* nBind = new SoNormalBinding;
//    nBind->value.setValue(SoNormalBinding::OVERALL);
//    m_root->addChild(nBind);

    SoFaceSet* shape = new SoFaceSet;
    shape->numVertices.set1Value(0, 4);
    m_root->addChild(shape);
}

void SunPosition::update(void* data, SoSensor*)
{
    SunPosition* node = (SunPosition*) data;
    node->m_transform->rotation =
        SbRotation(SbVec3f(1., 0., 0.), node->elevation.getValue()*gcf::degree) *
        SbRotation(SbVec3f(0., 0., 1.), -node->azimuth.getValue()*gcf::degree);
}
