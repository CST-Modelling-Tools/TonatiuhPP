#include <iostream>
#include <QVariant>
#include <QVBoxLayout>

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
    QVBoxLayout* layout = new QVBoxLayout;
    setLayout(layout);

    m_viewer = new SoQtExaminerViewer(this);
    SoBoxHighlightRenderAction* highlighter = new SoBoxHighlightRenderAction();
    highlighter->setColor(SbColor(100/255., 180/255., 120/255.));
    highlighter->setLineWidth(1.);
    m_viewer->setGLRenderAction(highlighter);
    m_viewer->setViewing(false);
    m_viewer->setTransparencyType(SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_BLEND);

//    SoPerspectiveCamera* camera = dynamic_cast<SoPerspectiveCamera*>(getCamera());
//    camera->scaleHeight(-1.); // left-handed to right-handed
//    camera->viewportMapping = SoCamera::LEAVE_ALONE;

    showAxes(true);
    m_viewer->setHeadlight(false);
    m_viewer->setDecoration(false);
    m_viewer->setAntialiasing(true, 1); // disable if slow
    m_camera = new TPerspectiveCamera;

    // do not propogate key and mouse events
    m_viewer->getGLWidget()->setFocusPolicy(Qt::NoFocus);
    setFocusPolicy(Qt::StrongFocus);
    m_filter = new KeyFilter(this);

    QPixmap pixmap;
    pixmap.load(":/images/cursors/cursorShift.png");
    pixmap = pixmap.scaledToWidth(48, Qt::SmoothTransformation);
    m_cursors["shift"] = QCursor(pixmap);
    pixmap.load(":/images/cursors/cursorRotation.png");
    pixmap = pixmap.scaledToWidth(48, Qt::SmoothTransformation);
    m_cursors["rotation"] = QCursor(pixmap);
    pixmap.load(":/images/cursors/cursorOrbit.png");
    pixmap = pixmap.scaledToWidth(48, Qt::SmoothTransformation);
    m_cursors["orbit"] = QCursor(pixmap);

    m_modifiersKeys = Qt::NoModifier;
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
    return SbViewportRegion();
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

#include <QMouseEvent>
#include <QDebug>
#include <QApplication>
#include "kernel/scene/TCameraKit.h"
#include <Inventor/nodes/SoTransform.h>
#include "libraries/math/gcf.h"

void GraphicView::mousePressEvent(QMouseEvent* event)
{
//    qDebug() << "pressed" << event->localPos();
    m_mousePressed = event->pos();
    m_modifiersPressed = event->modifiers();
    m_camera->saveTransform();
    event->accept();

    if (m_modifiersPressed & Qt::AltModifier)
    {

    }
    else if (m_modifiersPressed & Qt::ControlModifier)
    {
        if (m_modifiersPressed & Qt::ShiftModifier)
           m_camera->findRotationAnchor(m_viewer, m_mousePressed);
    }
    else if (m_modifiersPressed & Qt::ShiftModifier)
    {
        m_camera->findShiftAnchor(m_viewer, m_mousePressed, m_graphicRoot->getScene()->getLayout());
    }
}

void GraphicView::mouseReleaseEvent(QMouseEvent* event)
{
    qDebug() << "release" << event->pos();
    event->accept();
    m_modifiersPressed = Qt::NoModifier;
}

void GraphicView::mouseMoveEvent(QMouseEvent* event)
{
//    qDebug() << "move " << diff;
    if (m_modifiersPressed & Qt::AltModifier)
    {

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
        m_camera->moveShiftAnchor(m_viewer, event->pos());
    }

    //    m_viewer->render();
    event->accept();
}


void GraphicView::wheelEvent(QWheelEvent* event)
{
    if (!event->modifiers())
    {
        double z = event->angleDelta().y()/120.; // 15 degrees * 8 units
        m_camera->saveTransform();
        m_camera->findShiftAnchor(m_viewer, event->pos(), m_graphicRoot->getScene()->getLayout());
        m_camera->moveShiftAnchor(m_viewer, event->pos(), z);
    }
    event->accept();
}

void GraphicView::keyPressEvent(QKeyEvent* event)
{
//    switch (event.nativeScanCode()) {
//    case 50: // left shift
//    case 62: // right shift
//    }
//    qDebug()<< "ns " <<event->nativeScanCode() ;
//    qDebug()<< "nm " << event->nativeModifiers() ;
//    qDebug()<< "qk " << event->key() ;
//    qDebug()<< "qm " << event->modifiers() ;


//    if (event->key() == Qt::AltModifier) {
//        m_modifiersKeys |= Qt::AltModifier;
//    } else if (event->modifiers() == Qt::ControlModifier)
//           m_modifiersKeys |= Qt::ControlModifier;
//    else if (event->modifiers() == Qt::ShiftModifier)
//              m_modifiersKeys |= Qt::ShiftModifier;

    QCursor* cursor = 0;
    if (event->modifiers() & Qt::AltModifier)
        cursor = &m_cursors["orbit"];
    else if (event->modifiers() & Qt::ControlModifier)
        cursor = &m_cursors["rotation"];
    else if (event->modifiers() & Qt::ShiftModifier)
        cursor = &m_cursors["shift"];

    //        setCursor();// coin overrides?
    //    unsetCursor();
    //    QApplication::restoreOverrideCursor();
    if (cursor) {
        if (qApp->overrideCursor())
            qApp->changeOverrideCursor(*cursor);
        else
            qApp->setOverrideCursor(*cursor);
    } else
        qApp->restoreOverrideCursor();

//    if (event->key() == Qt::Key_Shift)
//        QApplication::setOverrideCursor(m_cursors["shift"]);
}

void GraphicView::keyReleaseEvent(QKeyEvent* event)
{
    keyPressEvent(event);
//    if (event->modifiers())
}

#include <QApplication>
#include "MenuStyle.h"
#include <QDebug>
#include <QMainWindow>
#include <QMenuBar>


void GraphicView::focusInEvent(QFocusEvent* event)
{
//        setHook();
    qDebug() << "disa";

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
    qDebug() << "ena";
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
    QWidget::resizeEvent(event);
    int hMax = QApplication::desktop()->screenGeometry().height();
    SbVec2s vs = m_viewer->getViewportRegion().getViewportSizePixels();
    int h = std::min(vs[0], vs[1]);
    m_camera->camera()->heightAngle = 30*gcf::degree*h/hMax;
    m_graphicRoot->updateSkyCamera(m_camera->camera());
}
