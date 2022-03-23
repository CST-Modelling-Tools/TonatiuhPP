#include "SceneTreeView.h"

#include <QApplication>
#include <QDrag>
#include <QLineEdit>
#include <QMimeData>
#include <QMouseEvent>

#include <Inventor/nodekits/SoBaseKit.h>

#include "kernel/run/InstanceNode.h"
#include "tree/SceneTreeModel.h"
#include "kernel/scene/TSeparatorKit.h"
#include "kernel/scene/TShapeKit.h"
#include <QHeaderView>


SceneTreeView::SceneTreeView(QWidget* parent):
    QTreeView(parent)
{
//    header()->setFont(font()); // resolves Qt bug

    setEditTriggers(QAbstractItemView::DoubleClicked);

    setItemDelegate(new SceneDelegate(this));

//    setAcceptDrops(true);
//    setAnimated(false);
}

void SceneTreeView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        m_startPos = event->pos();
    else if (event->button() == Qt::RightButton)
        emit showMenu(indexAt(event->pos()));

    QTreeView::mousePressEvent(event);
}

void SceneTreeView::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton) {
        int distance = (event->pos() - m_startPos).manhattanLength();
        if (distance >= QApplication::startDragDistance())
            startDrag(event);
    }
   // QTreeView::mouseMoveEvent(event);
}

void SceneTreeView::dragEnterEvent(QDragEnterEvent* event)
{
    event->setDropAction(Qt::MoveAction);
    event->accept();
}

void SceneTreeView::dragMoveEvent(QDragMoveEvent* event)
{
    event->setDropAction(Qt::MoveAction);
    event->accept();
}

void SceneTreeView::dropEvent(QDropEvent* event)
{
    QModelIndex indexParent = indexAt(event->position().toPoint());
    if (!indexParent.isValid()) return;
    SceneTreeModel* modelScene = (SceneTreeModel*) model();
    SoNode* nodeParent = modelScene->getInstance(indexParent)->getNode();
    if (!nodeParent->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId())) return;

    QByteArray data = event->mimeData()->data("text/objectID");
    QDataStream stream(&data, QIODevice::ReadOnly);
    QPoint position;
    stream >> position;
    QModelIndex index = indexAt(position);

    if (index == rootIndex()) return;
    if (index == indexParent) return;
    if (modelScene->parent(index) == indexParent) return;

    QModelIndex ancestor = indexParent;
    while (ancestor.parent().isValid()) {
        if (ancestor.parent() == index) return;
        ancestor = ancestor.parent();
    }

    bool isMove = true;
    if (event->modifiers() == Qt::ControlModifier)
        isMove = false;
    emit dragAndDrop(indexParent, index, isMove);

    event->acceptProposedAction();
    event->accept();
}

void SceneTreeView::closeEditor(QWidget* editor, QAbstractItemDelegate::EndEditHint hint)
{
    if (!editor) return;

    QLineEdit* textEdit = qobject_cast<QLineEdit*>(editor);
    QString name = textEdit->text();

    emit nodeRenamed(currentIndex(), name);
    QTreeView::closeEditor(editor, hint);
}

void SceneTreeView::startDrag(QMouseEvent* event)
{
    QPoint position = event->pos();
    QModelIndex index = indexAt(position);
    if (index.parent() == rootIndex()) return;
    
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << position;
    QMimeData* mimeData = new QMimeData;
    mimeData->setData("text/objectID", data);
      
    QDrag* drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->exec(Qt::MoveAction);
}

QSize SceneTreeView::sizeHint() const
{
    QSize size = QTreeView::sizeHint();
    size.setWidth(30*fontMetrics().xHeight());
    return size;
}
