#include "TShapeKit.h"

#include <Inventor/nodekits/SoAppearanceKit.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "kernel/profiles/ProfileBox.h"
#include "kernel/profiles/ProfileRectangular.h"
#include "kernel/material/MaterialAbsorber.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "kernel/shape/ShapePlanar.h"
#include "kernel/shape/ShapeSphere.h"
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

    shapeRT = new ShapePlanar;
    profileRT = new ProfileBox;
    materialRT = new MaterialAbsorber;
    material = new MaterialGL;

    m_shapeKit = new SoShapeKit;
//    m_shapeKit->setSearchingChildren(TRUE);
//    m_shapeKit->ref();
    m_shapeKit->setPart("material", material.getValue());
    SoGroup* g = (SoGroup*) topSeparator.getValue();
    g->addChild(m_shapeKit);

    m_sensor_shapeRT = new SoFieldSensor(onSensor, this);
    m_sensor_shapeRT->attach(&shapeRT);

    m_sensor_profileRT = new SoFieldSensor(onSensor, this);
    m_sensor_profileRT->attach(&profileRT);

    m_sensor_material = new SoFieldSensor(onSensor, this);
//    m_sensorProfile->setPriority(0);
    m_sensor_material->attach(&material);

    setName("Shape");
    onSensor(this, 0);
}

//TShapeKit* TShapeKit::copy(SbBool copyConnections) const
//{
//    TShapeKit* kit = dynamic_cast<TShapeKit*>(SoBaseKit::copy(copyConnections));
//    SoGroup* g = (SoGroup*) kit->topSeparator.getValue();
//    qDebug() << "numC copy " << g->getNumChildren();
//    qDebug() << "pointers  " << kit->m_shapeKit << " " << g->getChild(0);
////    kit->m_shapeKit = new SoShapeKit;
////    kit->m_shapeKit = (SoShapeKit*) g->getChild(0);
////    g->removeAllChildren();
////    g->addChild(kit->m_shapeKit);
////    kit->m_shapeKit->setPart("material", kit->material.getValue());
//    return kit;
//}

TShapeKit::~TShapeKit()
{
    delete m_sensor_shapeRT;
    delete m_sensor_profileRT;
    delete m_sensor_material;
//    m_shapeKit->unref();
}

void TShapeKit::onSensor(void* data, SoSensor*)
{
    TShapeKit* kit = (TShapeKit*) data;
    //    qDebug() << "called " << kit->getName();

    SoGroup* g = (SoGroup*) kit->topSeparator.getValue();
    if (kit->m_shapeKit != g->getChild(0)) {
        kit->m_shapeKit = (SoShapeKit*) g->getChild(0);
//        kit->m_shapeKit->setPart("material", kit->material.getValue());
    }

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

SbBool TShapeKit::readInstance(SoInput* in, unsigned short flags)
{
//    shapeRT.enableNotify(FALSE);
    SbBool ans = SoBaseKit::readInstance(in, flags);
//    shapeRT.enableNotify(TRUE);
    return ans;
}

//#include <Inventor/actions/SoRayPickAction.h>
//void TShapeKit::rayPick(SoRayPickAction* action)
//{
//    action->p
//    m_shapeKit->rayPick(action);
//}
