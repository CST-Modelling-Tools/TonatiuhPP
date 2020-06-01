#include "TShapeKit.h"

#include <Inventor/nodekits/SoAppearanceKit.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "kernel/apertures/ApertureRectangle.h"
#include "kernel/material/MaterialAbsorber.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "kernel/shape/ShapePlanar.h"
#include "libraries/geometry/Ray.h"


SO_KIT_SOURCE(TShapeKit)

/**
 * Sets up initialization for data common to all instances of this class, like submitting necessary information to the Coin type system.
 */
void TShapeKit::initClass()
{
    SO_KIT_INIT_CLASS(TShapeKit, SoShapeKit, "ShapeKit");
}

TShapeKit::TShapeKit()
{
    SO_KIT_CONSTRUCTOR(TShapeKit);
    isBuiltIn = TRUE;

    SO_NODE_ADD_FIELD( aperture, (0) );
    SO_NODE_ADD_FIELD( materialRT, (0) );
    SO_KIT_INIT_INSTANCE();

    ShapeRT* shape = new ShapePlanar;
    shape->setName(shape->getTypeName());
    setPart("shape", shape);

    Aperture* a = new ApertureRectangle;
    a->setName(a->getTypeName());
    aperture = a;

    MaterialRT* mRT = new MaterialAbsorber;
    mRT->setName(mRT->getTypeName());
    materialRT = mRT;

    SoMaterial* materialGL = new SoMaterial;
    materialGL->setName("MaterialGL");
    setPart("material", materialGL);

    m_sensor = new SoFieldSensor(onSensor, this);
    m_sensor->attach(&aperture);
}

TShapeKit::~TShapeKit()
{
    delete m_sensor;
}

#include <QDebug>

void TShapeKit::onSensor(void* data, SoSensor*)
{
    TShapeKit* kit = (TShapeKit*) data;
    qDebug() << "called " << kit->getName();
    ShapeRT* shape = (ShapeRT*) kit->getPart("shape", false);
    shape->updateShapeGL(kit);
}
