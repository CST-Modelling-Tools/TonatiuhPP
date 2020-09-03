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
    header()->setFont(font()); // resolves Qt bug

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

void SceneTreeView::dragEnterEvent(QDragEnterEvent *event)
{
    event->setDropAction(Qt::MoveAction);
    event->accept();
}

void SceneTreeView::dragMoveEvent(QDragMoveEvent *event)
{
    event->setDropAction(Qt::MoveAction);
    event->accept();
}

void SceneTreeView::dropEvent(QDropEvent* event)
{
    QModelIndex newParent = indexAt(event->pos());
    if (newParent.isValid())
    {
        SceneTreeModel* modelScene = static_cast< SceneTreeModel* >(model());
        QString type = modelScene->getInstance(newParent)->getNode()->getTypeId().getName().getString();
        
        SoNode* parentNode = modelScene->getInstance(newParent)->getNode();
        if (!parentNode->getTypeId().isDerivedFrom( SoBaseKit::getClassTypeId() ) ) return;
    
        QByteArray data = event->mimeData()->data("text/objetID");
        QDataStream stream(&data, QIODevice::ReadOnly);
        QPoint position;
        stream >> position;
            
        QModelIndex nodeIndex = indexAt(position);
        
        if (modelScene->parent(nodeIndex) == newParent) return;
        
        SoNode* childNode = modelScene->getInstance(nodeIndex)->getNode();
        
            
        if ( ( parentNode->getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) ) && 
             !( childNode->getTypeId().isDerivedFrom( SoSeparatorKit::getClassTypeId() ) ) )  return;
             
        if ( ( parentNode->getTypeId().isDerivedFrom( TShapeKit::getClassTypeId() ) ) && 
             ( childNode->getTypeId().isDerivedFrom( SoBaseKit::getClassTypeId() ) ) )  return;
             
        if ( ( parentNode->getTypeId().isDerivedFrom( TShapeKit::getClassTypeId() ) ) &&
             !( childNode->getTypeId().isDerivedFrom( SoBaseKit::getClassTypeId() ) ) ) 
        {
            
            TShapeKit* shapeKit = static_cast<TShapeKit*>(parentNode);
            if (shapeKit->getPart("shape", false)) return;
        }
            
        QModelIndex ancestor = newParent;
        while (ancestor.parent().isValid())
        {
            if (ancestor.parent() == nodeIndex)
                return;
            else
                ancestor = ancestor.parent();    
        }
            
        if (nodeIndex != newParent)
        {
            if (event->keyboardModifiers () == Qt::ControlModifier)
                 emit dragAndDropCopy(newParent, nodeIndex);
             else
                 emit dragAndDrop(newParent, nodeIndex);
             
            event->acceptProposedAction();
            event->accept();
        }
    }
}

void SceneTreeView::closeEditor(QWidget* editor, QAbstractItemDelegate::EndEditHint hint)
{
    if (!editor) return;

    QLineEdit* textEdit = qobject_cast<QLineEdit*>(editor);
    QString name = textEdit->text();

    emit nodeNameModificated(currentIndex(), name);
    QTreeView::closeEditor(editor, hint);
}

void SceneTreeView::startDrag(QMouseEvent* event)
{
    QPoint position = event->pos();
    QModelIndex index = indexAt(position);
    
    if (index.parent() == rootIndex()) return;
    
    QByteArray itemPos;
    QDataStream stream(&itemPos, QIODevice::WriteOnly);
    stream << position;
        
    QMimeData* mimeData = new QMimeData;
    mimeData->setData("text/objetID", itemPos);
      
    QDrag* drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->exec(Qt::MoveAction);
}