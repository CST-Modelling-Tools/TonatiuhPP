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


SO_KIT_SOURCE(TShapeKit)


void TShapeKit::initClass()
{
    SO_KIT_INIT_CLASS(TShapeKit, SoBaseKit, "BaseKit");

    ShapeRT::initClass();
    ProfileRT::initClass();
    MaterialRT::initClass();
}

TShapeKit::TShapeKit()
{
    SO_KIT_CONSTRUCTOR(TShapeKit);
//    isBuiltIn = TRUE;

    SO_KIT_ADD_CATALOG_ENTRY(topSeparator, SoSeparator, FALSE, this, "", TRUE);

    SO_NODE_ADD_FIELD( shapeRT, (0) );
    SO_NODE_ADD_FIELD( profileRT, (0) );
    SO_NODE_ADD_FIELD( materialRT, (0) );
    SO_NODE_ADD_FIELD( material, (0) );
    SO_KIT_INIT_INSTANCE();


    SoGroup* g = (SoGroup*) topSeparator.getValue();
    m_shapeKit = new SoShapeKit;
    g->addChild(m_shapeKit);

    m_sensorShape = new SoFieldSensor(onSensor, this);
    m_sensorShape->attach(&shapeRT);

    shapeRT = new ShapePlanar;
    profileRT = new ProfileBox;
    materialRT = new MaterialAbsorber;

//    SoShapeKit* kit = (SoShapeKit*) getPart("topSeparator", true);
//    material = ((SoShapeKit*)shapeKit2.getValue())->getPart("material", true);
//    material = kit->getPart("material", true);

    m_sensorProfile = new SoFieldSensor(onSensor, this);
    m_sensorProfile->attach(&profileRT);
}

TShapeKit::~TShapeKit()
{
    delete m_sensorShape;
    delete m_sensorProfile;
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
//    shapeKit.setDefault(TRUE);
    SoBaseKit::setDefaultOnNonWritingFields();
}
