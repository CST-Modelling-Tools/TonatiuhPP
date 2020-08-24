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

#include "ExaminerViewer2.h"



#include "GraphicRoot.h"
#include "GraphicView.h"
#include "tree/SoPathVariant.h"

/**
 * Creates a new GraphicView with a \a parerent for the model data 3D representation.
 *
 * Use setModel() to set the model.
 */
GraphicView::GraphicView(QWidget* parent):
    QAbstractItemView(parent),
    m_graphicRoot(0),
    m_viewer(0)
{

}

GraphicView::~GraphicView()
{
    delete m_viewer;
}

#include <Inventor/nodes/SoPerspectiveCamera.h>
void GraphicView::setSceneGraph(GraphicRoot* sceneGraphRoot)
{
    m_viewer = new SoQtExaminerViewer(this);
//    m_viewer = new ExaminerViewer2(this);

    m_graphicRoot = sceneGraphRoot;
    m_viewer->setSceneGraph(m_graphicRoot->getRoot());

    SoBoxHighlightRenderAction* highlighter = new SoBoxHighlightRenderAction();
    highlighter->setColor(SbColor(100/255., 180/255., 120/255.));
    highlighter->setLineWidth(1.);
    m_viewer->setGLRenderAction(highlighter);

    m_viewer->setTransparencyType(SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_BLEND);

//    SoPerspectiveCamera* camera = dynamic_cast<SoPerspectiveCamera*>(m_myRenderArea->getCamera());
//    camera->scaleHeight(-1.); // left-handed to right-handed

    showAxes(true);
    m_viewer->setHeadlight(false);
    m_viewer->setAntialiasing(true, 1); // disable if slow
}

SbViewportRegion GraphicView::GetViewportRegion() const
{
    return m_viewer->getViewportRegion();
}

SoCamera* GraphicView::getCamera() const
{
    return m_viewer->getCamera();
}

QModelIndex GraphicView::indexAt(const QPoint& /*point*/) const
{
    return QModelIndex();
}

void GraphicView::scrollTo(const QModelIndex& /*index*/, ScrollHint /*hint*/)
{

}

QRect GraphicView::visualRect(const QModelIndex& /*index*/) const
{
    return QRect();
}

void GraphicView::showAxes(bool on)
{
    m_viewer->setFeedbackVisibility(on);
}

void GraphicView::showDecoration(bool on)
{
    m_viewer->setDecoration(on);
}

void GraphicView::dataChanged(const QModelIndex& /*topLeft*/, const QModelIndex& /*bottomRight*/)
{

}

void GraphicView::rowsInserted(const QModelIndex& /*parent*/, int /*start*/, int /*end*/)
{

}

void GraphicView::rowsAboutToBeRemoved(const QModelIndex& /*parent*/, int /*start*/, int /*end*/)
{

}

void GraphicView::setSelection(const QRect& /*rect*/, QItemSelectionModel::SelectionFlags /*flags*/)
{

}

int GraphicView::horizontalOffset() const
{
    return 0;
}

int GraphicView::verticalOffset() const
{
    return 0;
}

bool GraphicView::isIndexHidden(const QModelIndex& /*index*/) const
{
    return false;
}

QModelIndex GraphicView::moveCursor(CursorAction /*cursorAction*/, Qt::KeyboardModifiers /*modifiers*/)
{
    return QModelIndex();
}

QRegion GraphicView::visualRegionForSelection(const QItemSelection& /*selection*/) const
{
    return QRegion();
}

#include "tree/SceneModel.h"
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
        SceneModel* model = (SceneModel*) current.model();
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
        m_graphicRoot->select(path);
        path->unref();
    }

//    m_sceneGraphRoot->touch();
}
