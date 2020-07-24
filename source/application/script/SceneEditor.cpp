#include "SceneEditor.h"

#include <Inventor/nodes/SoSeparator.h>
#include "kernel/scene/TSeparatorKit.h"
#include "kernel/scene/TShapeKit.h"


SceneEditor::SceneEditor(QObject* parent) : QObject(parent)
{
    m_root = new TSeparatorKit;
    m_root->ref();
    m_selection = m_root;
}

SceneEditor::~SceneEditor()
{
    m_root->unref();
}

void SceneEditor::insertNode(QString name)
{
    TSeparatorKit* kit = new TSeparatorKit;
    SoGroup* childList = (SoGroup*) m_selection->getPart("group", true);
    childList->addChild(kit);
    m_selection = kit;
}

void SceneEditor::insertShape(QString name)
{
    TShapeKit* kit = new TShapeKit;
    SoGroup* childList = (SoGroup*) m_selection->getPart("group", true);
    childList->addChild(kit);
    m_selection = kit;
}

void SceneEditor::insertSurface(QString name)
{

}

void SceneEditor::insertProfile(QString name)
{

}

void SceneEditor::insertMaterial(QString name)
{

}

void SceneEditor::insertTracker(QString name)
{

}

void SceneEditor::select(QString name)
{

}

void SceneEditor::setValue(QString name, QString value)
{

}

void SceneEditor::setName(QString name)
{
    m_selection->setName(name.toLatin1().data());
}
