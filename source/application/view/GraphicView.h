#pragma once

#include <QAbstractItemView>

class GraphicRoot;
class QModelIndex;
class SbViewportRegion;
class SoCamera;
class SoQtExaminerViewer;
//class ConstrainedViewer;
class TSeparatorKit;

//!  GraphicView class implements a 3D representation of items from a model.

class GraphicView: public QWidget
{
    Q_OBJECT

public:
    GraphicView(QWidget* parent = 0);
    ~GraphicView();

    void setSceneGraph(GraphicRoot* sceneGraphRoot);

    SbViewportRegion getViewportRegion() const;
    SoCamera* getCamera() const;
    void showDecoration(bool on);
    void showAxes(bool on);

public slots:
    void currentChanged(const QModelIndex& current, const QModelIndex& previous);

private:
    GraphicRoot* m_graphicRoot;
    SoQtExaminerViewer* m_viewer;
};
