#include "NodeObject.h"

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include "kernel/scene/TSeparatorKit.h"
#include "kernel/scene/TShapeKit.h"
#include <QScriptEngine>


NodeObject::NodeObject(QObject* parent):
    QObject(parent)
{
    m_node = new TSeparatorKit;
}

NodeObject::NodeObject(SoNode* node):
    m_node(node)
{

}

QScriptValue NodeObject::createNode()
{
    if (!m_node->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId())) return 0;
    TSeparatorKit* parent = (TSeparatorKit*)(m_node);
    SoGroup* childList = (SoGroup*) parent->getPart("group", true);

    TSeparatorKit* kit = new TSeparatorKit;
    childList->addChild(kit);
    NodeObject* ans = new NodeObject(kit);
    return engine()->newQObject(ans);
}

QScriptValue NodeObject::createShape()
{
    if (!m_node->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId())) return 0;
    TSeparatorKit* parent = (TSeparatorKit*)(m_node);
    SoGroup* childList = (SoGroup*) parent->getPart("group", true);

    TShapeKit* kit = new TShapeKit;
    childList->addChild(kit);
    NodeObject* ans = new NodeObject(kit);
    return engine()->newQObject(ans);
}

void NodeObject::setName(const QString& name)
{
    m_node->setName(name.toLatin1().data());
}

void NodeObject::setParameter(const QString& name, const QString& value)
{
    SoNode* node = m_node;
    if (m_node->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId())) {
        TSeparatorKit* parent = (TSeparatorKit*) m_node;
        node = parent->getPart("transform", true);
    }

    SoField* field = node->getField(name.toStdString().c_str());
    if (field)
        field->set(value.toStdString().c_str() );
}

/*
q = NodeObject();
q.setName("sdfgsdf");
q.setParameter("translation", "0 0 8");
tn.InsertScene(q);
*/
