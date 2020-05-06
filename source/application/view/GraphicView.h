#pragma once

#include <QAbstractItemView>

class GraphicRoot;
class QModelIndex;
class SbViewportRegion;
class SoCamera;
class SoQtExaminerViewer;
class TSeparatorKit;

//!  GraphicView class implements a 3D representation of items from a model.
/*!
  GraphicView provides a model/view implementation of a 3D view. Implements the interfaces defined by
  the QAbstractItemView class to allow it to display data provided by models derived from the QAbstractItemModel class.
*/

class GraphicView : public QAbstractItemView
{
    Q_OBJECT

public:
    GraphicView( QWidget* parent = 0 );
    ~GraphicView();

    void SetSceneGraph( GraphicRoot* sceneGraphRoot );

    SbViewportRegion GetViewportRegion() const;
    SoCamera* GetCamera() const;
    virtual QModelIndex indexAt ( const QPoint & point ) const;
    virtual void scrollTo ( const QModelIndex & index, ScrollHint hint = EnsureVisible );
    virtual QRect visualRect ( const QModelIndex & index ) const;

    void ViewDecoration( bool view );
    void ViewCoordinateSystem( bool view );

protected slots:
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void rowsInserted(const QModelIndex &parent, int start, int end);
    void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);
    void currentChanged ( const QModelIndex & current, const QModelIndex & previous );

protected:
    virtual void setSelection ( const QRect & rect, QItemSelectionModel::SelectionFlags flags );
    virtual int horizontalOffset () const;
    virtual int verticalOffset() const;
    virtual bool isIndexHidden ( const QModelIndex & index ) const;
    virtual QModelIndex moveCursor ( CursorAction cursorAction, Qt::KeyboardModifiers modifiers );
    virtual QRegion visualRegionForSelection ( const QItemSelection & selection ) const;

private:
    GraphicRoot* m_sceneGraphRoot;
    SoQtExaminerViewer* m_viewer;
};
