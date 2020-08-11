#include "GraphicRoot.h"

#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoTransformSeparator.h>
#include <Inventor/nodes/SoEnvironment.h>
#include <Inventor/annex/FXViz/nodes/SoShadowGroup.h>
#include <Inventor/annex/FXViz/nodes/SoShadowDirectionalLight.h>

#include "libraries/math/gcf.h"
#include "libraries/math/gcf.h"
#include "kernel/scene/TSceneKit.h"
#include "main/Document.h"
#include "SkyNode3D.h"
#include "SunNode3D.h"
#include "kernel/scene/GridNode.h"
#include "kernel/sun/SunPosition.h"

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


/*
 SoSeparator m_root
    SkyBackground m_sky
    SoSeparator m_grid
    SoEnvironment env
    SoTransformSeparator ts // transform for direct light
        SoTransform m_transformSun
        SoDirectionalLight light // should be outside ShadowGroup
    SoShadowGroup sg
        SoTransformSeparator ts2 // transform for shadow light
            SoTransform m_transformSun
            SoShadowDirectionalLight shadow // should be inside ShadowGroup
        m_selection // objects below SoSelection can be selected
            TSceneKit m_scene // for ray tracing
   SoSeparator m_rays
*/

GraphicRoot::GraphicRoot()
{
    m_root = new SoSeparator;
    m_root->ref();

    m_sky = new SkyNode3D;
    m_root->addChild(m_sky);

    m_sun = new SunNode3D;
    m_sky->getRoot()->addChild(m_sun);

    m_grid = new GridNode3D;
    m_root->addChild(m_grid);

    SoEnvironment* environment = new SoEnvironment;
    environment->ambientIntensity = 1.;
    m_root->addChild(environment);

    SoTransformSeparator* ts = new SoTransformSeparator;
    ts->addChild(m_sun->getTransform());
    SoDirectionalLight* light = new SoDirectionalLight;
    light->intensity = 0.7;
    light->direction = SbVec3f(0., 0., 1.);
    ts->addChild(light);
    m_root->addChild(ts);

    SoShadowGroup* group = new SoShadowGroup;
    group->precision = 1.;
    group->quality = 1.;
    m_root->addChild(group);

    SoTransformSeparator* ts2 = new SoTransformSeparator;
    ts2->addChild(m_sun->getTransform());
    SoShadowDirectionalLight* shadow = new SoShadowDirectionalLight;
    shadow->intensity = 0.3;
    shadow->direction = SbVec3f(0., 0., 1.);
    ts2->addChild(shadow);
    group->addChild(ts2);

    m_selection = new SoSelection;
    m_selection->policy = SoSelection::SINGLE;
    m_selection->addFinishCallback(selectionFinishCallback, (void*) this);
    group->addChild(m_selection);

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

    SunPosition* sunPosition = (SunPosition*) document->getSceneKit()->getPart("world.sun.position", false);
    m_sun->attach(sunPosition);

    GridNode* gridNode = (GridNode*) document->getSceneKit()->getPart("world.terrain.grid", true);
    m_grid->attach(gridNode);

    document->m_root = m_root;
}

void GraphicRoot::removeScene()
{
//    m_grid->removeAllChildren();
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
