#include "TShapeKit.h"

#include <Inventor/nodekits/SoAppearanceKit.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "kernel/profiles/ProfileBox.h"
#include "kernel/material/MaterialAbsorber.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "kernel/shape/ShapePlanar.h"
#include "libraries/math/3D/Ray.h"
#include "scene/MaterialGL.h"

SO_KIT_SOURCE(TShapeKit)


void TShapeKit::initClass()
{
    SO_KIT_INIT_CLASS(TShapeKit, SoBaseKit, "BaseKit");

    ShapeRT::initClass();
    ProfileRT::initClass();
    MaterialRT::initClass();
    MaterialGL::initClass();
}

TShapeKit::TShapeKit()
{
    SO_KIT_CONSTRUCTOR(TShapeKit);
    isBuiltIn = TRUE;

    SO_KIT_ADD_CATALOG_ENTRY(topSeparator, SoGroup, FALSE, this, "", FALSE);
    SO_NODE_ADD_FIELD( shapeRT, (0) );
    SO_NODE_ADD_FIELD( profileRT, (0) );
    SO_NODE_ADD_FIELD( materialRT, (0) );
    SO_NODE_ADD_FIELD( material, (0) );
    SO_KIT_INIT_INSTANCE();

    m_shapeKit = new SoShapeKit;
    m_shapeKit->ref();
    SoGroup* g = (SoGroup*) topSeparator.getValue();
    g->addChild(m_shapeKit);

    profileRT = new ProfileBox;
    materialRT = new MaterialAbsorber;
    SoMaterial* sm = new MaterialGL;
    material = sm;
    m_shapeKit->setPart("material", material.getValue());

    m_sensorShape = new SoFieldSensor(onSensor, this);
//    m_sensorShape->setPriority(0);
    m_sensorShape->attach(&shapeRT);

    m_sensorProfile = new SoFieldSensor(onSensor, this);
//    m_sensorProfile->setPriority(0);
    m_sensorProfile->attach(&profileRT);

    m_sensorMaterial = new SoFieldSensor(onSensor, this);
//    m_sensorProfile->setPriority(0);
    m_sensorMaterial->attach(&material);

    shapeRT = new ShapePlanar;
    setName("Shape");
}

TShapeKit* TShapeKit::copy(SbBool copyConnections) const
{
    TShapeKit* kit = dynamic_cast<TShapeKit*>(SoBaseKit::copy(copyConnections));
    kit->m_shapeKit = new SoShapeKit;
    SoGroup* g = (SoGroup*) kit->topSeparator.getValue();
    g->removeAllChildren();
    g->addChild(kit->m_shapeKit);
    kit->m_shapeKit->setPart("material", kit->material.getValue());
    return kit;
}

TShapeKit::~TShapeKit()
{
    delete m_sensorShape;
    delete m_sensorProfile;
    delete m_sensorMaterial;
    m_shapeKit->unref();
}

void TShapeKit::onSensor(void* data, SoSensor*)
{
    TShapeKit* kit = (TShapeKit*) data;
//    qDebug() << "called " << kit->getName();

    ShapeRT* shape = (ShapeRT*) kit->shapeRT.getValue();
    shape->updateShapeGL(kit);
}

void TShapeKit::setDefaultOnNonWritingFields()
{
    topSeparator.setDefault(TRUE);
    SoBaseKit::setDefaultOnNonWritingFields();
}

SbBool TShapeKit::setUpConnections(SbBool onoff, SbBool doitalways)
{
    m_shapeKit->setPart("material", material.getValue());
    return SoBaseKit::setUpConnections(onoff, doitalways);
}

//#include <Inventor/actions/SoRayPickAction.h>
//void TShapeKit::rayPick(SoRayPickAction* action)
//{
//    action->p
//    m_shapeKit->rayPick(action);
//}
