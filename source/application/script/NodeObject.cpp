#include "NodeObject.h"

#include <QScriptEngine>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>

#include "kernel/material/MaterialRT.h"
#include "kernel/profiles/ProfileRT.h"
#include "kernel/scene/TSeparatorKit.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/ShapeRT.h"
#include "kernel/trackers/Tracker.h"
#include "main/PluginManager.h"

PluginManager* NodeObject::m_plugins = 0;


NodeObject::NodeObject(QObject* parent):
    QObject(parent)
{
    m_node = new TSeparatorKit;
}

NodeObject::NodeObject(SoNode* node):
    m_node(node)
{

}

QScriptValue NodeObject::createNode(const QString& name)
{
    if (m_node->getTypeId() != TSeparatorKit::getClassTypeId()) return 0;
    TSeparatorKit* kit = new TSeparatorKit;

    TSeparatorKit* parent = (TSeparatorKit*)(m_node);
    SoGroup* group = (SoGroup*) parent->getPart("group", true);
    group->addChild(kit);

    NodeObject* ans = new NodeObject(kit);
    if (!name.isEmpty()) ans->setName(name);
    return engine()->newQObject(ans);
}

QScriptValue NodeObject::createShape()
{
    if (m_node->getTypeId() != TSeparatorKit::getClassTypeId()) return 0;
    TShapeKit* kit = new TShapeKit;

    TSeparatorKit* parent = (TSeparatorKit*)(m_node);
    SoGroup* group = (SoGroup*) parent->getPart("group", true);
    group->addChild(kit);

    NodeObject* ans = new NodeObject(kit);
    return engine()->newQObject(ans);
}

QScriptValue NodeObject::insertSurface(const QString& name)
{
    if (m_node->getTypeId() != TShapeKit::getClassTypeId()) return 0;

    ShapeFactory* f = m_plugins->getShapeMap().value(name, 0);
    ShapeRT* shape = f->create();
    shape->setName(f->name().toStdString().c_str());

    TShapeKit* parent = (TShapeKit*)(m_node);
    parent->shapeRT = shape;

    NodeObject* ans = new NodeObject(shape);
    return engine()->newQObject(ans);
}

QScriptValue NodeObject::insertProfile(const QString& name)
{
    if (m_node->getTypeId() != TShapeKit::getClassTypeId()) return 0;

    ProfileFactory* f = m_plugins->getProfileMap().value(name, 0);
    ProfileRT* profile = f->create();
    profile->setName(f->name().toStdString().c_str());

    TShapeKit* parent = (TShapeKit*)(m_node);
    parent->profileRT = profile;

    NodeObject* ans = new NodeObject(profile);
    return engine()->newQObject(ans);
}

QScriptValue NodeObject::insertMaterial(const QString& name)
{
    if (m_node->getTypeId() != TShapeKit::getClassTypeId()) return 0;

    MaterialFactory* f = m_plugins->getMaterialMap().value(name, 0);
    MaterialRT* material = f->create();
    material->setName(f->name().toStdString().c_str());

    TShapeKit* parent = (TShapeKit*)(m_node);
    parent->materialRT = material;

    NodeObject* ans = new NodeObject(material);
    return engine()->newQObject(ans);
}

QScriptValue NodeObject::insertTracker(const QString& name)
{
    if (m_node->getTypeId() != TSeparatorKit::getClassTypeId()) return 0;

    TrackerFactory* f = m_plugins->getTrackerMap().value(name, 0);
    Tracker* tracker = f->create();
    tracker->setName(f->name().toStdString().c_str());

    TSeparatorKit* parent = (TSeparatorKit*)(m_node);
    parent->setPart("tracker", tracker);

    NodeObject* ans = new NodeObject(tracker);
    return engine()->newQObject(ans);
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
    }

    SoField* field = node->getField(name.toLatin1().data());
    if (field) field->set(value.toLatin1().data());
}

/*

function makeHeliostat(parent, name, position, focus)
{
    n = parent.createNode(name);
    n.setParameter("translation", position);

    t = n.insertTracker("Dual");
    t.setParameter("aimingPoint", "0 0 8");

    n = n.createNode("primary");
    n = n.createNode("secondary");
    n = n.createNode("facet");
    s = n.createShape();

    q = s.insertSurface("Parabolic");
    q.setParameter("fX", focus);
    q.setParameter("fY", focus);

    p = s.insertProfile("Box");
    p.setParameter("uSize", "2");
    p.setParameter("vSize", "2");

    m = s.insertMaterial("Specular");
    m.setParameter("slope", "3");
}

nodeRoot = NodeObject();
nodeRoot.setName("Script");

n = nodeRoot.createNode("Receiver");
n.setParameter("translation", "0 0 8");
s = n.createShape();
p = s.insertProfile("Box");
p.setParameter("uSize", "3");
p.setParameter("vSize", "3");

print("\n");
nodeHeliostats = nodeRoot.createNode("Heliostats");
iMax = 50;
count = 1;
for (i = -iMax; i <= iMax; i++) {
    printTime(i + "\n");
    nodeTemp = nodeHeliostats.createNode("Row" + i);
    for (j = -iMax; j <= iMax; j++) {
        name = "H_" + count;
        position = "" + 5*i + " " + 5*j + " 0";
        focus = 10.;
        makeHeliostat(nodeTemp, name, position, focus);
        count++;
    }
}

printTime("nodes created\n");
tn.InsertScene(nodeRoot);
printTime("nodes inserted\n");

*/
