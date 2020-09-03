#include "CmdSunModified.h"

#include "kernel/sun/SunKit.h"
#include "tree/SceneTreeModel.h"
#include "kernel/scene/TSceneKit.h"
#include "application/view/GraphicRoot.h"


CmdSunModified::CmdSunModified(SunKit* sun, SceneTreeModel* model, QUndoCommand* parent):
    QUndoCommand("Sun modified", parent),
    m_model(model)
{
    m_sunOld = (SunKit*) m_model->getSceneKit()->getPart("world.sun", false);
    m_sunOld->ref();

    m_sun = sun;
    m_sun->ref();
}

CmdSunModified::~CmdSunModified()
{
    m_sunOld->unref();
    m_sun->unref();
}

void CmdSunModified::undo()
{
    set(m_sunOld);
}

void CmdSunModified::redo()
{
    set(m_sun);
}

void CmdSunModified::set(SunKit* sun)
{
    m_model->replaceSun(sun);
    TSceneKit* scene = m_model->getSceneKit();
    GraphicRoot* root = scene->m_graphicRoot;
    root->updateScene(scene);
    root->rays()->removeAllChildren();
    scene->updateTrackers();
}
