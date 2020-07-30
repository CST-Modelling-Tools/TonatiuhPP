#include "TShapeKit.h"

#include <Inventor/nodekits/SoAppearanceKit.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "kernel/profiles/ProfileBox.h"
#include "kernel/material/MaterialAbsorber.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "kernel/shape/ShapePlanar.h"
#include "libraries/math/3D/Ray.h"


SO_KIT_SOURCE(TShapeKit)

/**
 * Sets up initialization for data common to all instances of this class, like submitting necessary information to the Coin type system.
 */
void TShapeKit::initClass()
{
    SO_KIT_INIT_CLASS(TShapeKit, SoShapeKit, "ShapeKit");

    ShapeRT::initClass();
    ProfileRT::initClass();
    MaterialRT::initClass();
}

TShapeKit::TShapeKit()
{
    SO_KIT_CONSTRUCTOR(TShapeKit);
    isBuiltIn = TRUE;

    SO_NODE_ADD_FIELD( shapeRT, (0) );
    SO_NODE_ADD_FIELD( profileRT, (0) );
    SO_NODE_ADD_FIELD( materialRT, (0) );
    SO_KIT_INIT_INSTANCE();

    m_sensorShape = new SoFieldSensor(onSensor, this);
    m_sensorShape->attach(&shapeRT);

    ShapeRT* sRT = new ShapePlanar;
    sRT->setName(sRT->getTypeName());
    shapeRT = sRT;

    ProfileRT* a = new ProfileBox;
    a->setName(a->getTypeName());
    profileRT = a;

    MaterialRT* mRT = new MaterialAbsorber;
    mRT->setName(mRT->getTypeName());
    materialRT = mRT;

    SoMaterial* materialGL = new SoMaterial;
    materialGL->setName("MaterialGL");
    setPart("material", materialGL);

    m_sensorProfile = new SoFieldSensor(onSensor, this);
    m_sensorProfile->attach(&profileRT);
}

TShapeKit::~TShapeKit()
{
    delete m_sensorShape;
    delete m_sensorProfile;
}

void TShapeKit::setDefaultOnNonWritingFields()
{
    coordinate3.setDefault(TRUE); // do not save
    normal.setDefault(TRUE);
    shape.setDefault(TRUE);

    SoShapeKit::setDefaultOnNonWritingFields();
}

#include <QDebug>

void TShapeKit::onSensor(void* data, SoSensor*)
{
    TShapeKit* kit = (TShapeKit*) data;
//    qDebug() << "called " << kit->getName();

    ShapeRT* shape = (ShapeRT*) kit->shapeRT.getValue();
    shape->updateShapeGL(kit);
}
