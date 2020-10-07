#pragma once

#include <QAbstractItemView>

class GraphicRoot;
class QModelIndex;
class SbViewportRegion;
class SoCamera;
class SoQtExaminerViewer;
//class ConstrainedViewer;
class TSeparatorKit;
class SoPerspectiveCamera;
//!  GraphicView class implements a 3D representation of items from a model.

class TPerspectiveCamera;
class QPushButton;

class GraphicView: public QFrame
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
    void render();

public slots:
    void currentChanged(const QModelIndex& current, const QModelIndex& previous);

private:
    GraphicRoot* m_graphicRoot;
    SoQtExaminerViewer* m_viewer;

    QPoint m_mousePressed;
    Qt::KeyboardModifiers m_modifiersPressed;
    Qt::KeyboardModifiers m_modifiersKeys;
    TPerspectiveCamera* m_camera;
    QMap<QString, QCursor> m_cursors;

public:
QWidget* m_window;
QObject* m_filter;
//QPushButton* m_label;

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);

    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    void focusInEvent(QFocusEvent* event);
    void focusOutEvent(QFocusEvent* event);
    void resizeEvent(QResizeEvent* event);

//    void paintEvent(QPaintEvent* event);

private:
    QMenu* m_menu;
    QAction* actionViewAll;
    QAction* actionViewSelected;
    QAction* actionViewHome;

    QAction* actionViewTop;
    QAction* actionViewSun;
    QAction* actionLookNorth;
    QAction* actionLookEast;
    QAction* actionLookSouth;
    QAction* actionLookWest;

public slots:
    void showContextMenu(QPoint);

    void on_actionViewAll_triggered();
    void on_actionViewSelected_triggered();
    void on_actionViewHome_triggered();

    void on_actionViewTop_triggered();
    void on_actionViewSun_triggered();

    void on_actionLookNorth_triggered();
    void on_actionLookEast_triggered();
    void on_actionLookSouth_triggered();
    void on_actionLookWest_triggered();
};
