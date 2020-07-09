#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>

#include "libraries/math/gcf.h"
#include "libraries/math/gcf.h"

#include "GraphicRoot.h"
//#include "kernel/trackers/GraphicRootTracker.h"
#include "kernel/scene/TSceneKit.h"


void selectionFinishCallback(void* userData, SoSelection* selection)
{
    GraphicRoot* root = static_cast<GraphicRoot*>(userData);
    if (root) root->SelectionChanged(selection);
}


#include "SkyBackground.h"

/*
m_nodeRoot SoSeparator
    nodeSky SkyBackground
    m_pSceneSeparator SoSeparator
        m_pRootTransform SoTransform
            m_pTracker GraphicRootTracker(TSceneKit)
        m_pSelectionNode SoSelection
            TSceneKit(azel, from TSceneTracker)

            lightlist
                tlightkit (azel dialog)
            childList
                SunNode
                   TSceneTracker(lightKit)
                    Layout

*/

GraphicRoot::GraphicRoot():
    m_nodeRoot(0),
    m_pGrid(0),
    m_pRays(0),
    m_pRootTransform(0),
    m_pSceneSeparator(0),
    m_pSelectionNode(0)
{
    m_nodeRoot = new SoSeparator;
    m_nodeRoot->ref();

    SkyBackground* nodeSky = new SkyBackground;
    m_nodeRoot->addChild(nodeSky);

    m_pSceneSeparator = new SoSeparator;
    m_nodeRoot->addChild(m_pSceneSeparator);

    m_pRootTransform = new SoTransform;
    m_pRootTransform->ref();
    m_pSceneSeparator->addChild(m_pRootTransform);

    m_pSelectionNode = new SoSelection;
    m_pSelectionNode->ref();
    m_pSelectionNode->policy = SoSelection::SINGLE;
    m_pSelectionNode->addFinishCallback( selectionFinishCallback, static_cast< void*>( this ) );
    m_pSceneSeparator->addChild(m_pSelectionNode);
}

GraphicRoot::~GraphicRoot()
{
    if (m_pGrid) {
        while (m_pGrid->getRefCount( ) > 1)    m_pGrid->unref();
        m_pGrid = 0;
    }
    if (m_pRays)
    {
        while (m_pRays->getRefCount( ) > 1)    m_pRays->unref();
        m_pRays = 0;
    }
    if( m_pSelectionNode)
    {
        while ( m_pSelectionNode->getRefCount( ) > 1 )    m_pSelectionNode->unref();
        m_pSelectionNode = 0;
    }
    if( m_pRootTransform)
    {
        while ( m_pRootTransform->getRefCount( ) > 1 )    m_pRootTransform->unref();
        m_pRootTransform = 0;
    }
    if( m_nodeRoot)
    {
        while ( m_nodeRoot->getRefCount( ) > 1 )    m_nodeRoot->unref();
        m_nodeRoot = 0;
    }
}

void GraphicRoot::AddGrid(SoSeparator* grid)
{
    m_pGrid = grid;
    grid->ref();
}

void GraphicRoot::AddRays(SoSeparator* rays)
{
    RemoveRays();
    m_pRays = rays;
    m_pRays->ref();
}

void GraphicRoot::AddModel(TSceneKit* sceneKit)
{
    if (!sceneKit) return;
    m_pSelectionNode->addChild(sceneKit);
    sceneKit->m_root = m_nodeRoot; // cycle?
}


void GraphicRoot::DeselectAll()
{
    m_pSelectionNode->deselectAll();
}

SoSeparator* GraphicRoot::GetNode() const
{
    return m_nodeRoot;
}

void GraphicRoot::RemoveGrid()
{
    if( m_pGrid )
    {
        ShowGrid( false );
        m_pGrid->removeAllChildren();
        while(m_pGrid->getRefCount() > 1) m_pGrid->unref();
        if (m_pGrid->getRefCount() > 1)
            gcf::SevereError( "RemoveGrid: m_pGrid referenced in excess ");
        m_pGrid->unref();
        m_pGrid = 0;
    }
}

void GraphicRoot::RemoveRays()
{
    if (m_pRays) {
        m_pRays->removeAllChildren();
        if (m_pRays->getRefCount() > 1)
            gcf::SevereError( "RemoveRays: m_pRays referenced in excess");
        m_pRays->unref();
        m_pRays = 0;
    }
}

void GraphicRoot::RemoveModel()
{
    if (m_pSelectionNode->getNumChildren() > 0)
    {
//        m_pTracker->SetSceneKit(0);
//        m_pTracker->Disconnect();

        m_pSelectionNode->removeAllChildren();
    }
}

void GraphicRoot::Select(const SoPath* path)
{
    m_pSelectionNode->select(path);
    m_pSelectionNode->touch();
}

void GraphicRoot::SelectionChanged(SoSelection* selection)
{
    emit ChangeSelection(selection);
}

void GraphicRoot::ShowBackground( bool /*view*/ ) //?
{
//    SoVRMLBackground* vrmlBackground = static_cast< SoVRMLBackground* > ( m_nodeRoot->getChild( 0 ) );

//    if( view )
//    {
//        //float gcolor[][3] = { {0.9843f, 0.8862f, 0.6745f}, {0.7843f, 0.6157f, 0.4785f} };
//        float gcolor[][3] = { {0.7, 0.42, 0.15f},{ 0.7f, 0.42f, 0.15f } };
//        float gangle= 1.570f;

//        vrmlBackground->groundColor.setValues( 0, 6, gcolor );
//        vrmlBackground->groundAngle.setValue( gangle );
//        float scolor[][3] = { {0.0157f, 0.0235f, 0.4509f}, {0.5569f, 0.6157f, 0.8471f} };
//        float sangle= 1.570f;
//        vrmlBackground->skyColor.setValues( 0,6,scolor );
//        vrmlBackground->skyAngle.setValue( sangle );
//    }
//    else
//    {
//        float color[][3] = { {0.1f, 0.1f, 0.1f}, {0.1f, 0.1f, 0.1f} };
//        float angle= 1.570f;
//        vrmlBackground->groundColor.setValues( 0, 6, color );
//        vrmlBackground->groundAngle.setValue( angle );
//        vrmlBackground->skyColor.setValues( 0,6,color );
//        vrmlBackground->skyAngle.setValue( angle );
//    }
}

void GraphicRoot::ShowGrid(bool view)
{
    if (view && m_pGrid)
        m_nodeRoot->addChild(m_pGrid);
    else if (!view)
        if (m_pGrid->getRefCount() > 0)
            m_nodeRoot->removeChild(m_pGrid);
}

void GraphicRoot::ShowRays(bool view)
{
    if (view && m_pRays)
        m_pSceneSeparator->addChild(m_pRays);
    else if (!view)
        if (m_pRays->getRefCount() > 0)
            m_pSceneSeparator->removeChild(m_pRays);
}

