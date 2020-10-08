#include "GraphicRoot.h"

#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoTransformSeparator.h>
#include <Inventor/nodes/SoEnvironment.h>
#include <Inventor/annex/FXViz/nodes/SoShadowGroup.h>
#include <Inventor/annex/FXViz/nodes/SoShadowDirectionalLight.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "libraries/math/gcf.h"
#include "libraries/math/gcf.h"
#include "kernel/scene/TSceneKit.h"
#include "main/Document.h"
#include "SkyNode3D.h"
#include "SunNode3D.h"
#include "OverlayNode.h"
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

#include <Inventor/nodes/SoPerspectiveCamera.h>

#include <Inventor/nodes/SoCube.h>

GraphicRoot::GraphicRoot()
{
    m_root = new SoSeparator;
    m_root->renderCulling = SoSeparator::OFF;
    m_root->ref();

    SoSeparator* sep = new SoSeparator;
    sep->renderCulling = SoSeparator::OFF;
    sep->pickCulling = SoSeparator::OFF;
    m_root->addChild(sep);

    m_sky = new SkyNode3D;
    sep->addChild(m_sky);

    m_sun = new SunNode3D;
    m_sky->getRoot()->addChild(m_sun);



    SoEnvironment* environment = new SoEnvironment;
    environment->ambientIntensity = 1.;
    m_root->addChild(environment);

    SoShadowGroup* group = new SoShadowGroup;
    group->renderCulling = SoSeparator::OFF;
    group->precision = 1.;
    group->quality = 1.;
    m_root->addChild(group);

    SoTransformSeparator* ts = new SoTransformSeparator;
    ts->addChild(m_sun->getTransform());
    SoShadowDirectionalLight* shadow = new SoShadowDirectionalLight;
    shadow->intensity = 1.;
    shadow->direction = SbVec3f(0., 0., 1.);
    ts->addChild(shadow);
    group->addChild(ts);


//    SoDrawStyle* wireStyle;
//    wireStyle = new SoDrawStyle;
//    wireStyle->style = SoDrawStyle::LINES + SoDrawStyleElement::FILLED;
//    wireStyle->setOverride(TRUE);
//    group->addChild(wireStyle);


    m_selection = new SoSelection;
    m_selection->renderCulling = SoSeparator::OFF;
    m_selection->policy = SoSelection::SINGLE;
    m_selection->addFinishCallback(selectionFinishCallback, (void*) this);
    group->addChild(m_selection);

    m_grid = new GridNode3D; // better here for antialiasing
    m_grid->renderCulling = SoSeparator::OFF;
    group->addChild(m_grid);

    m_rays = new SoSeparator; // order important for antialiasing
    m_root->addChild(m_rays);

    m_overlayNode = new OverlayNode;
    m_overlayNode->showAim(false);
    m_root->addChild(m_overlayNode);

    m_sensor = new SoFieldSensor(update, this);
    m_sensor->setPriority(0);
}

GraphicRoot::~GraphicRoot()
{
    m_root->unref();
    delete m_sensor;
}

#include "kernel/scene/TCameraKit.h"

TCameraKit* GraphicRoot::getCameraKit()
{
    return (TCameraKit*) m_scene->getPart("world.camera", false);
}

//void GraphicRoot::setCamera(SoCamera* camera)
//{
//    TCameraKit* ck = (TCameraKit*) m_scene->getPart("world.camera", false);
//    ck->setCamera(camera);
//}

//SoCamera* GraphicRoot::getCamera()
//{
////    return 0;
//    TCameraKit* ck = (TCameraKit*) m_scene->getPart("world.camera", false);
//    return ck->m_camera;
////    return (SoCamera*) ck->m_cameraKit->getPart("camera", false);
//}

void GraphicRoot::setDocument(Document* document)
{
    TSceneKit* scene = document->getSceneKit();
    if (!scene) return;
    m_scene = scene;
    m_selection->removeAllChildren();
    m_selection->addChild(scene);

    SunPosition* sunPosition = (SunPosition*) scene->getPart("world.sun.position", false);
    m_sun->attach(sunPosition);

    GridNode* gridNode = (GridNode*) scene->getPart("world.terrain.grid", true);
    m_grid->attach(gridNode);

    scene->m_graphicRoot = this;

    m_sensor->attach(scene->getPart("world", false)->getField("sun"));
}

void GraphicRoot::updateScene(TSceneKit* scene)
{
    SunPosition* sunPosition = (SunPosition*) scene->getPart("world.sun.position", false);
    m_sun->attach(sunPosition);
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

void GraphicRoot::enableSelection(bool on)
{
    m_selection->policy = on ? SoSelection::SINGLE : SoSelection::DISABLE;
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

void GraphicRoot::updateSkyCamera(SoPerspectiveCamera* camera)
{
    m_sky->updateSkyCamera(camera);
    m_overlayNode->updateSkyCamera(camera);
}

void GraphicRoot::update(void* data, SoSensor*)
{
    GraphicRoot* gr = (GraphicRoot*) data;
    gr->rays()->removeAllChildren();
    gr->updateScene(gr->m_scene);
    gr->m_scene->updateTrackers();
}
