#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/actions/SoSearchAction.h>

#include "libraries/math/gcf.h"
#include "libraries/math/gcf.h"

#include "GraphicRoot.h"
#include "kernel/scene/TSceneKit.h"
#include "main/Document.h"
#include "SkyBackground.h"
#include "kernel/scene/GridNode.h"

/*
SoSelection
selectionFinishCallback
GraphicRoot::onSelectionChanged
emit selectionChanged(SoSelection* selection);
MainWindow
    connect(
        m_graphicsRoot, SIGNAL(selectionChanged(SoSelection*)),
        this, SLOT(SelectionFinish(SoSelection*))
    );
    SelectionFinish::
     m_selectionModel->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);

     ---
   GraphicView
      void currentChanged(const QModelIndex& current, const QModelIndex& previous);

*/
void selectionFinishCallback(void* userData, SoSelection* selection)
{
    GraphicRoot* root = (GraphicRoot*) userData;
    if (root) root->onSelectionChanged(selection);
}


GraphicRoot::GraphicRoot()
{
    m_root = new SoSeparator;
    m_root->ref();

    SkyBackground* sky = new SkyBackground;
    m_root->addChild(sky);

    m_grid = new SoSeparator;
    m_grid->addChild(new SoLineSet); // empty scene hides sky
    m_root->addChild(m_grid);

    m_selection = new SoSelection;
    m_selection->policy = SoSelection::SINGLE;
    m_selection->addFinishCallback(selectionFinishCallback, (void*) this);
    m_root->addChild(m_selection);

    m_rays = new SoSeparator; // order important for antialiasing
    m_root->addChild(m_rays);
}

GraphicRoot::~GraphicRoot()
{
    m_root->unref();
}

void GraphicRoot::setDocument(Document* document)
{
    if (!document->getSceneKit()) return;
    m_selection->removeAllChildren();
    m_selection->addChild(document->getSceneKit());

    GridNode* gridNode = (GridNode*) document->getSceneKit()->getPart("world.terrain.grid", true);
    m_grid->addChild(gridNode->getRoot());

    document->m_root = m_root;
}

void GraphicRoot::removeScene()
{
    m_grid->removeAllChildren();
    m_rays->removeAllChildren();
    m_selection->removeAllChildren();
}

void GraphicRoot::showGrid(bool on)
{
    SoSearchAction search;
    search.setName("grid");
    search.apply(m_grid);
    SoPath* path = search.getPath();
    if (!path) return;
    SoSwitch* group = (SoSwitch*) path->getTail();
    group->whichChild = on ? SO_SWITCH_ALL : SO_SWITCH_NONE;
}

void GraphicRoot::showRays(bool on)
{
    SoSearchAction search;
    search.setName("rays");
    search.apply(m_rays);
    SoPath* path = search.getPath();
    if (!path) return;
    SoSwitch* group = (SoSwitch*) path->getTail();
    group->whichChild = on ? SO_SWITCH_ALL : SO_SWITCH_NONE;
}

void GraphicRoot::showPhotons(bool on)
{
    SoSearchAction search;
    search.setName("photons");
    search.apply(m_rays);
    SoPath* path = search.getPath();
    if (!path) return;
    SoSwitch* group = (SoSwitch*) path->getTail();
    group->whichChild = on ? SO_SWITCH_ALL : SO_SWITCH_NONE;
}

void GraphicRoot::select(const SoPath* path)
{
    m_selection->select(path);
    m_selection->touch();
}

void GraphicRoot::deselectAll()
{
    m_selection->deselectAll();
}

void GraphicRoot::onSelectionChanged(SoSelection* selection)
{
    emit selectionChanged(selection);
}
