#include <iostream>
#include <QVariant>

#include <Inventor/actions/SoBoxHighlightRenderAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>

#include "gui/GraphicRoot.h"
#include "GraphicView.h"
#include "gui/PathWrapper.h"

/**
 * Creates a new GraphicView with a \a parerent for the model data 3D representation.
 *
 * Use setModel() to set the model.
 */
GraphicView::GraphicView(QWidget* parent)
    : QAbstractItemView(parent),
      m_sceneGraphRoot(0),
      m_myRenderArea(0)
{

}

GraphicView::~GraphicView()
{
    delete m_myRenderArea;
}

void GraphicView::SetSceneGraph(GraphicRoot* sceneGraphRoot)
{

    m_sceneGraphRoot = sceneGraphRoot;
    m_myRenderArea = new SoQtExaminerViewer(this);
    m_myRenderArea->setGLRenderAction(new SoBoxHighlightRenderAction() );
    m_myRenderArea->setTransparencyType(SoGLRenderAction::SORTED_OBJECT_BLEND);
    m_myRenderArea->setSceneGraph(m_sceneGraphRoot->GetNode() );

    ViewCoordinateSystem(true);
}

SbViewportRegion GraphicView::GetViewportRegion() const
{
    return m_myRenderArea->getViewportRegion();
}

SoCamera* GraphicView::GetCamera() const
{
    return m_myRenderArea->getCamera();
}

QModelIndex GraphicView::indexAt(const QPoint& /*point*/) const
{
    return QModelIndex();
}

void GraphicView::scrollTo(const QModelIndex& /*index*/, ScrollHint /*hint*/)
{
}

QRect GraphicView::visualRect (const QModelIndex& /*index*/) const
{
    return QRect();
}

void GraphicView::ViewCoordinateSystem(bool view)
{
    m_myRenderArea->setFeedbackVisibility(view);
}

void GraphicView::ViewDecoration(bool view)
{
    m_myRenderArea->setDecoration(view);
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

void GraphicView::currentChanged(const QModelIndex& current, const QModelIndex& /*previous*/)
{
    if (m_sceneGraphRoot)
    {
        m_sceneGraphRoot->DeselectAll();

        SoFullPath* path;
        QVariant variant = current.data(Qt::UserRole);

        if (variant.canConvert<PathWrapper>() )
        {
            path = static_cast< SoFullPath*>(variant.value< PathWrapper >().GetPath() );
            m_sceneGraphRoot->Select(path);
        }

        //m_sceneGraphRoot->touch();
    }
}
