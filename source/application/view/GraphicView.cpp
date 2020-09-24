#include <iostream>
#include <QVariant>

#include <Inventor/actions/SoBoxHighlightRenderAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
//#include <Inventor/Qt/viewers/SoQtConstrainedViewer.h>


//class ConstrainedViewer: public SoQtConstrainedViewer
//{
//    public:
//    ConstrainedViewer(QWidget* parent): SoQtConstrainedViewer(parent, "", TRUE, BUILD_ALL, BROWSER, TRUE) {}
//    ~ConstrainedViewer(){}
//};




#include "GraphicRoot.h"
#include "GraphicView.h"
#include "tree/SoPathVariant.h"

/**
 * Creates a new GraphicView with a \a parerent for the model data 3D representation.
 *
 * Use setModel() to set the model.
 */
GraphicView::GraphicView(QWidget* parent):
    QWidget(parent),
    m_graphicRoot(0),
    m_viewer(0)
{
    m_viewer = new SoQtExaminerViewer(this);

    SoBoxHighlightRenderAction* highlighter = new SoBoxHighlightRenderAction();
    highlighter->setColor(SbColor(100/255., 180/255., 120/255.));
    highlighter->setLineWidth(1.);
    m_viewer->setGLRenderAction(highlighter);

    m_viewer->setTransparencyType(SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_BLEND);

//    SoPerspectiveCamera* camera = dynamic_cast<SoPerspectiveCamera*>(getCamera());
//    camera->scaleHeight(-1.); // left-handed to right-handed
//    camera->viewportMapping = SoCamera::LEAVE_ALONE;

    showAxes(true);
    m_viewer->setHeadlight(false);
    m_viewer->setDecoration(false);
    m_viewer->setAntialiasing(true, 1); // disable if slow
}

GraphicView::~GraphicView()
{
    delete m_viewer;
}

#include <Inventor/nodes/SoPerspectiveCamera.h>
void GraphicView::setSceneGraph(GraphicRoot* sceneGraphRoot)
{
//    m_viewer = new ExaminerViewer2(this);
    m_graphicRoot = sceneGraphRoot;
    m_viewer->setSceneGraph(m_graphicRoot->getRoot());
}

SbViewportRegion GraphicView::getViewportRegion() const
{
    return m_viewer->getViewportRegion();
}

SoCamera* GraphicView::getCamera() const
{
    return m_viewer->getCamera();
}


void GraphicView::showAxes(bool on)
{
    m_viewer->setFeedbackVisibility(on);
}

void GraphicView::showDecoration(bool on)
{
    m_viewer->setDecoration(on);
}



#include "tree/SceneTreeModel.h"
#include "kernel/run/InstanceNode.h"
#include "kernel/scene/TSeparatorKit.h"
void GraphicView::currentChanged(const QModelIndex& current, const QModelIndex& /*previous*/)
{
    if (!m_graphicRoot) return;
    m_graphicRoot->deselectAll();

    QVariant variant = current.data(Qt::UserRole);
    if (!variant.canConvert<SoPathVariant>()) return;
    SoPath* path = variant.value<SoPathVariant>().getPath();
    {
        SceneTreeModel* model = (SceneTreeModel*) current.model();
        if (!model) return;
        InstanceNode* instance = model->getInstance(current);
        if (!instance) return;
        SoNode* node = instance->getNode();
//        if (!node->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId())) return;
        if (!node->getTypeId().isDerivedFrom(SoBaseKit::getClassTypeId()))
        {
            path->unref();
            return;
        }
        if (!m_viewer->isViewing())
            m_graphicRoot->select(path);
        else
            m_graphicRoot->deselectAll();
        path->unref();
    }

//    m_sceneGraphRoot->touch();
}
