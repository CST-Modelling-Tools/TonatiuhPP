#include <iostream>
#include <QVariant>
#include <QVBoxLayout>
#include <QAction>
#include <QMenu>

#include <Inventor/actions/SoBoxHighlightRenderAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/actions/SoGetMatrixAction.h>
//#include <Inventor/Qt/viewers/SoQtConstrainedViewer.h>


//class ConstrainedViewer: public SoQtConstrainedViewer
//{
//    public:
//    ConstrainedViewer(QWidget* parent): SoQtConstrainedViewer(parent, "", TRUE, BUILD_ALL, BROWSER, TRUE) {}
//    ~ConstrainedViewer(){}
//};


#include "KeyboardHook.h"
//    HHOOK m_hook;
//void MainWindow::setHook()
//{
//    //If parameter is NULL, GetModuleHandle returns a pseudo handle to the current process.
//    m_hook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), NULL); // Ставим хук WH_KEYBOARD_LL
//    if (!m_hook)
//    { // Проверка на правильность установки хука
//        QMessageBox::critical(this, QString("Error in keyboard lock"),QString("Unable to set keyboard lock! [keyHook enable error]"), QMessageBox::Ok);
//        return;
//    }
//    //MessageBox(0,TEXT("Press OK for exit"),TEXT("OK"),MB_OK); // ждем пока надоест играться с хуком
//    //UnhookWindowsHookEx(hHook);
//    return;
//}

//void MainWindow::unsetHook()
//{
//    if (m_hook)
//    { // Если хук был установлен, снимемс его
//        UnhookWindowsHookEx(m_hook);
//        return;
//    }
//}

#include "GraphicRoot.h"
#include "GraphicView.h"
#include "tree/SoPathVariant.h"
#include "TPerspectiveCamera.h"
#include "KeyFilter.h"
#include "kernel/scene/TSceneKit.h"
#include <QLabel>
#include <QPushButton>
//#include <QGLWidget>
/**
 * Creates a new GraphicView with a \a parerent for the model data 3D representation.
 *
 * Use setModel() to set the model.
 */
GraphicView::GraphicView(QWidget* parent):
    QFrame(parent),
    m_graphicRoot(0),
    m_viewer(0)
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    QWidget* w = new QWidget;
    layout->addWidget(w);


    m_viewer = new SoQtExaminerViewer(w);
    m_viewer->setAntialiasing(true, 1); // disable if slow
    m_viewer->setTransparencyType(SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_BLEND); // do not move
    m_viewer->setViewing(false);

    SoBoxHighlightRenderAction* highlighter = new SoBoxHighlightRenderAction(m_viewer->getViewportRegion());
    highlighter->setColor(SbColor(100/255., 180/255., 120/255.));
    highlighter->setLineWidth(2.);
    m_viewer->setGLRenderAction(highlighter);

    showAxes(true);
    m_viewer->setHeadlight(false);
    m_viewer->setDecoration(false);
    m_camera = new TPerspectiveCamera;

    // do not propogate key and mouse events
//    m_viewer->getGLWidget()->setFocusPolicy(Qt::NoFocus);
//    w->setFocusPolicy(Qt::NoFocus);
    m_viewer->getGLWidget()->setEnabled(false);
    setFocusPolicy(Qt::StrongFocus);
    m_filter = new KeyFilter(this);

    QPixmap pixmap;

    pixmap.load(":/images/cursors/cursorShiftA.png");
    pixmap = pixmap.scaledToWidth(48, Qt::SmoothTransformation);
    m_cursors["shiftA"] = QCursor(pixmap);

    pixmap.load(":/images/cursors/cursorShiftB.png");
    pixmap = pixmap.scaledToWidth(48, Qt::SmoothTransformation);
    m_cursors["shiftB"] = QCursor(pixmap);

    pixmap.load(":/images/cursors/cursorRotation.png");
    pixmap = pixmap.scaledToWidth(48, Qt::SmoothTransformation);
    m_cursors["rotation"] = QCursor(pixmap);

    pixmap.load(":/images/cursors/cursorRotationA.png");
    pixmap = pixmap.scaledToWidth(48, Qt::SmoothTransformation);
    m_cursors["rotationA"] = QCursor(pixmap);

    pixmap.load(":/images/cursors/cursorRotationB.png");
    pixmap = pixmap.scaledToWidth(48, Qt::SmoothTransformation);
    m_cursors["rotationB"] = QCursor(pixmap);

    pixmap.load(":/images/cursors/cursorOrbit.png");
    pixmap = pixmap.scaledToWidth(48, Qt::SmoothTransformation);
    m_cursors["orbit"] = QCursor(pixmap);

    pixmap.load(":/images/cursors/cursorOrbitA.png");
    pixmap = pixmap.scaledToWidth(48, Qt::SmoothTransformation);
    m_cursors["orbitA"] = QCursor(pixmap);

    pixmap.load(":/images/cursors/cursorOrbitB.png");
    pixmap = pixmap.scaledToWidth(48, Qt::SmoothTransformation);
    m_cursors["orbitB"] = QCursor(pixmap);

    m_modifiersKeys = Qt::NoModifier; //delete
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showContextMenu(QPoint)));

    setStyleSheet(R"(
QFrame {
    border: 1px solid lightgray;
}

QFrame[inFocus=true] {
    border: 1px solid #828790;
}
    )");

//    QGLWidget* gw = () m_viewer->getGLWidget();

//    QPushButton* label  = new QPushButton("asdvasdv", w);
//    m_label  = new QPushButton("asdvasdv", w);
//    label->setGeometry(QRect(0, 0, 504, 14));
//    connect(m_viewer->getGLWidget(),SIGNAL()

    // actions
    actionViewAll = new QAction("All", this);
    actionViewAll->setObjectName("actionViewAll");
    actionViewAll->setShortcut(QKeySequence("Ctrl+Alt+."));

    actionViewSelected = new QAction("Selected", this);
    actionViewSelected->setObjectName("actionViewSelected");
    actionViewSelected->setShortcut(QKeySequence("Ctrl+."));

    actionViewTop = new QAction(QIcon(":/images/view/ViewTop.png"), "Top", this);
    actionViewTop->setObjectName("actionViewTop");
    actionViewTop->setShortcut(QKeySequence("Ctrl+5"));
    actionViewSun = new QAction(QIcon(":/images/view/viewSun.png"), "Sun", this);
    actionViewSun->setObjectName("actionViewSun");
    actionViewSun->setShortcut(QKeySequence("Ctrl+0"));

    actionLookNorth = new QAction("North", this);
    actionLookNorth->setObjectName("actionLookNorth");
    actionLookNorth->setShortcut(QKeySequence("Ctrl+8"));
    actionLookEast = new QAction("East", this);
    actionLookEast->setObjectName("actionLookEast");
    actionLookEast->setShortcut(QKeySequence("Ctrl+6"));
    actionLookSouth = new QAction("South", this);
    actionLookSouth->setObjectName("actionLookSouth");
    actionLookSouth->setShortcut(QKeySequence("Ctrl+2"));
    actionLookWest = new QAction("West", this);
    actionLookWest->setObjectName("actionLookWest");
    actionLookWest->setShortcut(QKeySequence("Ctrl+4"));

    m_menu = new QMenu(this);
    QMenu* menuCamera = new QMenu("Camera", this);
    menuCamera->setIcon(QIcon(":/images/scene/nodeCamera.png"));
    menuCamera->addAction(actionViewAll);
    menuCamera->addAction(actionViewSelected);
    menuCamera->addSeparator();
    menuCamera->addAction(actionViewTop);
    menuCamera->addAction(actionViewSun);
    menuCamera->addSeparator();
    menuCamera->addAction(actionLookNorth);
    menuCamera->addAction(actionLookEast);
    menuCamera->addAction(actionLookSouth);
    menuCamera->addAction(actionLookWest);
    m_menu->addMenu(menuCamera);

    addActions(m_menu->actions()); // for shortcuts

    QMetaObject::connectSlotsByName(this);
}

GraphicView::~GraphicView()
{
    delete m_viewer;
    delete m_camera;
}

#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodekits/SoCameraKit.h>
void GraphicView::setSceneGraph(GraphicRoot* sceneGraphRoot)
{
//    m_viewer = new ExaminerViewer2(this);
    m_graphicRoot = sceneGraphRoot;
    m_viewer->setSceneGraph(m_graphicRoot->getRoot());
    SoPerspectiveCamera* camera = (SoPerspectiveCamera*) m_viewer->getCamera();
    m_camera->setCamera(camera);
//    m_cameraKit = m_graphicRoot->getCameraKit();
}

SbViewportRegion GraphicView::getViewportRegion() const
{
    return SbViewportRegion();//?
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

void GraphicView::render()
{
    m_viewer->render();
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
//        else
//            m_graphicRoot->deselectAll();
        path->unref();
    }

    //    m_sceneGraphRoot->touch();
}

#include <QMouseEvent>
#include <QDebug>
#include <QApplication>
#include "kernel/scene/TCameraKit.h"
#include <Inventor/nodes/SoTransform.h>
#include "libraries/math/gcf.h"
#include "view/OverlayNode.h"

void GraphicView::mousePressEvent(QMouseEvent* event)
{
//    qDebug() << "pressed" << event->localPos();
    m_mousePressed = event->pos();
    m_modifiersPressed = event->modifiers();
    m_camera->saveTransform();

//    if (!hasFocus()) {
        QKeyEvent ev(QEvent::KeyPress, 0, m_modifiersPressed);
        keyPressEvent(&ev);
//    }

    if (m_modifiersPressed & Qt::AltModifier)
    {
        SbVec2s vs = m_viewer->getViewportRegion().getViewportSizePixels();
        m_camera->findMoveAnchor(m_viewer, QPoint(vs[0]/2, vs[1]/2), m_graphicRoot->getScene()->getLayout());
        if (m_modifiersPressed & Qt::ShiftModifier) {
            qApp->changeOverrideCursor(m_cursors["orbitB"]);
            m_camera->findOrbitAnchor(m_viewer, m_mousePressed, m_graphicRoot->getScene()->getLayout());
        }
    }
    else if (m_modifiersPressed & Qt::ControlModifier)
    {
        if (m_modifiersPressed & Qt::ShiftModifier) {
            qApp->changeOverrideCursor(m_cursors["rotationB"]);
            m_camera->findRotationAnchor(m_viewer, m_mousePressed);
        }
    }
    else if (m_modifiersPressed & Qt::ShiftModifier)
    {
        qApp->changeOverrideCursor(m_cursors["shiftB"]);
        m_camera->findMoveAnchor(m_viewer, m_mousePressed, m_graphicRoot->getScene()->getLayout());
    }
    event->accept();
}

void GraphicView::mouseReleaseEvent(QMouseEvent* event)
{
//    qDebug() << "release" << event->pos();
    if (m_modifiersPressed & Qt::AltModifier)
    {
        if (m_modifiersPressed & Qt::ShiftModifier) {
            qApp->changeOverrideCursor(m_cursors["orbitA"]);
        }
    }
    else if (m_modifiersPressed & Qt::ControlModifier)
    {
        if (m_modifiersPressed & Qt::ShiftModifier) {
            qApp->changeOverrideCursor(m_cursors["rotationA"]);
        }
    }
    else if (m_modifiersPressed & Qt::ShiftModifier)
    {
        qApp->changeOverrideCursor(m_cursors["shiftA"]);
    }
    m_modifiersPressed = Qt::NoModifier;
    event->accept();
}

void GraphicView::mouseMoveEvent(QMouseEvent* event)
{
//    qDebug() << "move " << diff;
    if (m_modifiersPressed & Qt::AltModifier)
    {
        if (m_modifiersPressed & Qt::ShiftModifier) {
             m_camera->moveOrbitAnchor(m_viewer, event->pos());
        } else {
            QPoint diff = event->pos() - m_mousePressed;
            double s = 0.25;
            double dAz = s*diff.x();
            double dEl = -s*diff.y();
            m_camera->orbit(dAz, dEl);
        }
    }
    else if (m_modifiersPressed & Qt::ControlModifier)
    {
        if (m_modifiersPressed & Qt::ShiftModifier) {
            m_camera->moveRotationAnchor(m_viewer, event->pos());
        } else {
            QPoint diff = event->pos() - m_mousePressed;
            double s = 0.25;
            double dAz = s*diff.x();
            double dEl = -s*diff.y();
            m_camera->rotate(dAz, dEl);
        }
    }
    else if (m_modifiersPressed & Qt::ShiftModifier)
    {
       if (GetKeyState(VK_LSHIFT) < 0)
           m_camera->movePanAnchor(m_viewer, event->pos());
       else
           m_camera->moveShiftAnchor(m_viewer, event->pos());
    }

    //    m_viewer->render();
    event->accept();
}

void GraphicView::wheelEvent(QWheelEvent* event)
{
    if (!event->modifiers()) {
        double z = event->angleDelta().y()/120.; // 15 degrees * 8 units
        m_camera->saveTransform();
        m_camera->findMoveAnchor(m_viewer, event->pos(), m_graphicRoot->getScene()->getLayout());
        m_camera->moveShiftAnchor(m_viewer, event->pos(), z);
    } else if (event->modifiers() & Qt::AltModifier) {
        double z = event->angleDelta().x()/120.;
        m_camera->zoomCenter(m_viewer, m_graphicRoot->getScene()->getLayout(), z);
    } else if (event->modifiers() & Qt::ControlModifier)  {
        double z = event->angleDelta().y()/120.;
        m_camera->zoomCenter(m_viewer, m_graphicRoot->getScene()->getLayout(), z);
    } else if (event->modifiers() & Qt::ShiftModifier)  {//recenter
        double z = event->angleDelta().y()/120.;
//        m_camera->zoomCenter(m_viewer, m_graphicRoot->getScene()->getLayout(), z);
        m_camera->saveTransform();
        m_camera->moveCameraPlane(m_viewer, event->pos(), m_graphicRoot->getScene()->getLayout(),z);
        cursor().setPos(mapToGlobal(geometry().center()));
    }
    event->accept();
}

void GraphicView::keyPressEvent(QKeyEvent* event)
{
    QString nc;
    if (event->modifiers() & Qt::AltModifier) {
        nc = "orbit";
        if (event->modifiers() & Qt::ShiftModifier)
            nc += "A";
    } else if (event->modifiers() & Qt::ControlModifier) {
        nc = "rotation";
        if (event->modifiers() & Qt::ShiftModifier)
            nc += "A";
    } else if (event->modifiers() & Qt::ShiftModifier)
        nc = "shiftA";

    if (!nc.isEmpty()) {
        m_graphicRoot->enableSelection(false);
        QCursor& cursor = m_cursors[nc];
//        setCursor(); // coin overrides?
//        unsetCursor();
        if (qApp->overrideCursor())
            qApp->changeOverrideCursor(cursor);
        else
            qApp->setOverrideCursor(cursor);
        m_graphicRoot->getOverlayNode()->showAim(true);
        m_viewer->render();
    } else {
        m_graphicRoot->enableSelection(true);
        qApp->restoreOverrideCursor();
        m_graphicRoot->getOverlayNode()->showAim(false);
        m_viewer->render();
    }
    QFrame::keyPressEvent(event);
}

void GraphicView::keyReleaseEvent(QKeyEvent* event)
{
    keyPressEvent(event);
    QFrame::keyReleaseEvent(event);
}

#include <QApplication>
#include "MenuStyle.h"
#include <QDebug>
#include <QMainWindow>
#include <QMenuBar>

void GraphicView::focusInEvent(QFocusEvent* event)
{
    setProperty("inFocus", true);
    style()->unpolish(this);
    update();

//        setHook();
//    qDebug() << "disa";

    qApp->installEventFilter(m_filter);

//    MenuStyle* ms = dynamic_cast<MenuStyle*>( m_window->style());
//    if (ms)
//    {
//        qApp->installEventFilter(this);
//        QMainWindow* mw = (QMainWindow*) m_window;
////        mw->menuBar()->installEventFilter(this);
////        ms->unpolish(m_window);
////      ms->m_useAlt = false;
////      ms->polish(m_window);
////      m_window->update();

//    }

//    MenuStyle* ms = (MenuStyle*) qApp->style();
//    ms->unpolish(qApp);
//        qDebug() << ms->m_useAlt;
////    ms->m_useAlt = true;
//    ms->polish(qApp);
//    qApp->setStyle(new QProxyStyle);
//    QWidget::focusInEvent(event);
}

void GraphicView::focusOutEvent(QFocusEvent* event)
{
    setProperty("inFocus", false);
    style()->unpolish(this);
    update();
    qApp->restoreOverrideCursor();

//    qDebug() << "ena";
    qApp->removeEventFilter(m_filter);
//    MenuStyle* ms = dynamic_cast<MenuStyle*> (m_window->style());
//    if (ms)
//    {
//        ms->unpolish(m_window);
//      ms->m_useAlt = true;
//      ms->polish(m_window);
//    }
//
//        qApp->setStyle(new MenuStyle);
//    QWidget::focusOutEvent(event);
}

#include <QDesktopWidget>

void GraphicView::resizeEvent(QResizeEvent* event)
{
    QFrame::resizeEvent(event);
    int hMax = QApplication::desktop()->screenGeometry().height();
//    SbVec2s vs = m_viewer->getViewportRegion().getViewportSizePixels();
//    int h = std::min(vs[0], vs[1]);
    QSize size = event->size();
    int h = std::min(size.width(), size.height());
    m_camera->camera()->heightAngle = 30*gcf::degree*h/hMax;
    m_graphicRoot->updateSkyCamera(m_camera->camera());
}

void GraphicView::showContextMenu(QPoint pos)
{
    m_menu->popup(mapToGlobal(pos));
}

//#include <QPainter>
//void GraphicView::paintEvent(QPaintEvent* event)
//{
//    QPainter painter(this);
//    painter.drawText(0, 20 ," XVXV");
//    qDebug()<<"painter";
//}

void GraphicView::on_actionViewAll_triggered()
{
    SbViewportRegion vpr = m_viewer->getViewportRegion();
    SoCamera* camera = m_viewer->getCamera();
    camera->viewAll(m_graphicRoot->getScene()->getLayout(), vpr);

    SbVec3f pos = camera->position.getValue();
    m_camera->m_position = vec3d(pos[0], pos[1], pos[2]);

    SbRotation q = camera->orientation.getValue();
    SbVec3f d(0, 0, -1);
    q.multVec(d, d);
//    qDebug() << d.toString().getString();
    double alpha = asin(d[2])/gcf::degree;
    double gamma = atan2(d[0], d[1])/gcf::degree;
    m_camera->m_rotation = vec3d(gamma, alpha, 0);

    m_camera->updateTransform();
}

#include <Inventor/nodes/SoSelection.h>
void GraphicView::on_actionViewSelected_triggered()
{
//    QModelIndex index = ui->sceneView->currentIndex();
//    InstanceNode* instance = m_modelScene->getInstance(index);
//    SoNode* node = instance->getNode();
//    TSeparatorKit* kit = dynamic_cast<TSeparatorKit*>(node);
//    if (kit) node = kit->getPart("group", false);

//    SoSearchAction actionSearch;
//    actionSearch.setNode(node);
//    actionSearch.apply(m_document->getSceneKit()->getLayout());
//    SoPath* path = actionSearch.getPath();

    int c = m_graphicRoot->getSelection()->getNumSelected();
    if (c == 0) return;
    SoPath* path = m_graphicRoot->getSelection()->getPath(0);

    SbViewportRegion vpr;
    SoGetMatrixAction actionMatrix(vpr);
    actionMatrix.apply(path);
    SbMatrix matrix = actionMatrix.getMatrix();

    SbVec3f vL(0., 0., 0.);
    SbVec3f vG;
    matrix.multVecMatrix(vL, vG);

//    SoCamera* camera = m_viewer->getCamera();
//    camera->pointAt(vG, SbVec3f(0., 0., 1.));

//    SbVec3f pos = camera->position.getValue();
//    m_camera->m_position = vec3d(pos[0], pos[1], pos[2]);

//    SbRotation q = camera->orientation.getValue();
//    SbVec3f d(0, 0, -1);
//    q.multVec(d, d);
//    qDebug() << d.toString().getString();
//    double alpha = asin(d[2])/gcf::degree;
//    double gamma = atan2(d[0], d[1])/gcf::degree;
//    m_camera->m_rotation = vec3d(gamma, alpha, 0);

    m_camera->lookAt(tgf::makeVector3D(vG));
}

//SbVec3f getTarget(SoCamera* camera)
//{
//    SbRotation rotation = camera->orientation.getValue();
//    SbVec3f target;
//    rotation.multVec(SbVec3f(0., 0., -camera->focalDistance.getValue()), target);
//    target += camera->position.getValue();
//    return target;
//}

void GraphicView::on_actionViewTop_triggered()
{
    m_camera->setRotation(0., -90.);
}

void GraphicView::on_actionLookNorth_triggered()
{
    m_camera->setRotation(0., 0.);
}

void GraphicView::on_actionLookEast_triggered()
{
    m_camera->setRotation(90., 0.);
}

void GraphicView::on_actionLookSouth_triggered()
{
    m_camera->setRotation(180., 0.);
}

void GraphicView::on_actionLookWest_triggered()
{
    m_camera->setRotation(270., 0.);
}

void GraphicView::on_actionViewSun_triggered()
{
//    SoSceneKit* sceneKit = m_document->getSceneKit();
//    if (!sceneKit) return;
//    SunKit* lightKit = static_cast<SunKit*>(sceneKit->getPart("world.sun", false));
//    if (!lightKit) return;
//    SoTransform* transform = lightKit->m_transform;

//    SoCamera* camera = m_graphicView[m_focusView]->getCamera();
//    SbVec3f target = getTarget(camera);

//    SbVec3f shift;
//    transform->rotation.getValue().multVec(SbVec3f(0., 0., -camera->focalDistance.getValue()), shift);

//    camera->position = target + shift;
//    camera->pointAt(target, SbVec3f(0., 0., 1.));
}
