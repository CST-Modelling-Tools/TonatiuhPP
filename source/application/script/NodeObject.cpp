#include "NodeObject.h"

#include <QJSEngine>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>

#include "kernel/scene/TSeparatorKit.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/scene/TSceneKit.h"
#include "kernel/shape/ShapeRT.h"
#include "kernel/profiles/ProfileRT.h"
#include "kernel/material/MaterialRT.h"
#include "kernel/trackers/TrackerArmature.h"
#include "kernel/trackers/TrackerKit.h"
#include "kernel/sun/SunKit.h"
#include "kernel/air/AirKit.h"
#include "kernel/sun/SunShape.h"
#include "kernel/air/AirTransmission.h"

#include "main/MainWindow.h"
#include "main/PluginManager.h"
#include <QQmlEngine>

MainWindow* NodeObject::s_mainWindow = 0;
QJSEngine* NodeObject::s_engine = 0;


NodeObject::NodeObject(QObject* parent):
    QObject(parent)
{
    m_node = new TSeparatorKit;
}

NodeObject::NodeObject(SoNode* node):
    QObject(0),
    m_node(node)
{

}

QJSValue NodeObject::getScene() // move to MainWindow
{
    NodeObject* ans = new NodeObject(s_mainWindow->getSceneKit());
    return s_engine->newQObject(ans);
}

QJSValue NodeObject::getRoot()
{
    NodeObject* ans = new NodeObject(s_mainWindow->getSceneKit()->getLayout());
    return s_engine->newQObject(ans);
}

QJSValue NodeObject::createNode(const QString& name)
{
    if (m_node->getTypeId() != TSeparatorKit::getClassTypeId())
        return QJSValue();
    TSeparatorKit* kit = new TSeparatorKit;

    TSeparatorKit* parent = (TSeparatorKit*) m_node;
    SoGroup* group = (SoGroup*) parent->getPart("group", true);
    group->addChild(kit);

    NodeObject* ans = new NodeObject(kit);
    if (!name.isEmpty()) ans->setName(name);
//    return s_engine->newQObject(ans);
    return qjsEngine(this)->newQObject(ans);
}

QJSValue NodeObject::createShape()
{
    if (m_node->getTypeId() != TSeparatorKit::getClassTypeId())
        return QJSValue();
    TShapeKit* kit = new TShapeKit;

    TSeparatorKit* parent = (TSeparatorKit*) m_node;
    SoGroup* group = (SoGroup*) parent->getPart("group", true);
    group->addChild(kit);

    NodeObject* ans = new NodeObject(kit);
    return s_engine->newQObject(ans);
}

QJSValue NodeObject::createTracker()
{
    if (m_node->getTypeId() != TSeparatorKit::getClassTypeId())
        return 0;
    TrackerKit* kit = new TrackerKit;

    TSeparatorKit* parent = (TSeparatorKit*) m_node;
    SoGroup* group = (SoGroup*) parent->getPart("group", true);
    kit->m_parent = parent;
    group->addChild(kit);

    NodeObject* ans = new NodeObject(kit);
    return s_engine->newQObject(ans);
}

QJSValue NodeObject::getPart(const QString& name)
{
    if (!m_node->getTypeId().isDerivedFrom(SoBaseKit::getClassTypeId())) return 0;

    SoBaseKit* kit = (SoBaseKit*)(m_node);
    SoNode* node = kit->getPart(name.toLatin1().data(), false);
    if (!node) {
        SoField* field = m_node->getField(name.toLatin1().data());
        SoSFNode* fnode = dynamic_cast<SoSFNode*>(field);
        if (fnode) node = fnode->getValue();
    }
    if (!node) return 0;
    NodeObject* ans = new NodeObject(node);
    return s_engine->newQObject(ans);
}

void NodeObject::setPart(const QString& name, QJSValue node)
{
    if (m_node->getTypeId() == TrackerKit::getClassTypeId() && name == "armature") {
        TrackerKit* kit = (TrackerKit*) m_node;
        NodeObject* nodeTracker = (NodeObject*) node.toQObject();
        kit->armature = nodeTracker->m_node;
    }
    return;
}

QJSValue NodeObject::insertSurface(const QString& name)
{
    if (m_node->getTypeId() != TShapeKit::getClassTypeId()) return 0;

    ShapeFactory* f = s_mainWindow->getPlugins()->getShapeMap().value(name, 0);
    ShapeRT* shape = f->create();

    TShapeKit* parent = (TShapeKit*) m_node;
    parent->shapeRT = shape;
    parent->profileRT = shape->getDefaultProfile();

    NodeObject* ans = new NodeObject(shape);
    return s_engine->newQObject(ans);
}

QJSValue NodeObject::insertProfile(const QString& name)
{
    if (m_node->getTypeId() != TShapeKit::getClassTypeId()) return 0;

    ProfileFactory* f = s_mainWindow->getPlugins()->getProfileMap().value(name, 0);
    ProfileRT* profile = f->create();

    TShapeKit* parent = (TShapeKit*) m_node;
    parent->profileRT = profile;

    NodeObject* ans = new NodeObject(profile);
    return s_engine->newQObject(ans);
}

QJSValue NodeObject::insertMaterial(const QString& name)
{
    if (m_node->getTypeId() != TShapeKit::getClassTypeId()) return 0;

    MaterialFactory* f = s_mainWindow->getPlugins()->getMaterialMap().value(name, 0);
    MaterialRT* material = f->create();

    TShapeKit* parent = (TShapeKit*) m_node;
    parent->materialRT = material;

    NodeObject* ans = new NodeObject(material);
    return s_engine->newQObject(ans);
}

QJSValue NodeObject::insertArmature(const QString& name)
{
    if (m_node->getTypeId() != TrackerKit::getClassTypeId()) return 0;

    TrackerFactory* f = s_mainWindow->getPlugins()->getTrackerMap().value(name, 0);
    TrackerArmature* tracker = f->create();

    TrackerKit* parent = (TrackerKit*)(m_node);
    parent->armature = tracker;

    NodeObject* ans = new NodeObject(tracker);
    return s_engine->newQObject(ans);
}

void NodeObject::setName(const QString& name)
{
    m_node->setName(name.toLatin1().data());
}

void NodeObject::setParameter(const QString& name, const QString& value)
{
    SoNode* node = m_node;
    if (m_node->getTypeId() == TSeparatorKit::getClassTypeId()) {
        TSeparatorKit* parent = (TSeparatorKit*) m_node;
        node = parent->getPart("transform", true);
    } else if (m_node->getTypeId() == SunKit::getClassTypeId() && name == "shape") {
        SunFactory* sf = s_mainWindow->getPlugins()->getSunMap().value(value, 0);
        SunShape* s = sf->create();
        SoSFNode* f = (SoSFNode*) m_node->getField(name.toLatin1().data());
        f->setValue(s);
        return;
    } else if (m_node->getTypeId() == AirKit::getClassTypeId() && name == "transmission") {
        AirFactory* sf = s_mainWindow->getPlugins()->getAirMap().value(value, 0);
        AirTransmission* s = sf->create();
        SoSFNode* f = (SoSFNode*) m_node->getField(name.toLatin1().data());
        f->setValue(s);
        return;
    }

    SoField* field = node->getField(name.toLatin1().data());
    if (!field) return;
    if (field->getTypeId() == SoSFString::getClassTypeId()) {
        SoSFString* sf = (SoSFString*) field;
        sf->setValue(value.toLatin1().data());
    } else
        field->set(value.toLatin1().data());
}

QJSValue NodeObject::FindInterception(QJSValue surface, QJSValue rays)
{
    return ::findInterception(surface.toString(), rays.toUInt(), s_mainWindow);
}
