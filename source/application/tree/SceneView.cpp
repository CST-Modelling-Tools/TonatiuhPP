#include <QApplication>
#include <QDrag>
#include <QLineEdit>
#include <QMimeData>
#include <QMouseEvent>

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodekits/SoNodeKitListPart.h>

#include "kernel/run/InstanceNode.h"
#include "SceneView.h"
#include "tree/SceneModel.h"
#include "kernel/scene/TSeparatorKit.h"
#include "kernel/scene/TShapeKit.h"
#include <QHeaderView>

/**
 * Creates a new view for a model. This
 */
SceneView::SceneView(QWidget* parent):
    QTreeView(parent),
    m_currentIndex(),
    m_itemsDelegate(0)
{
    header()->setFont(font()); // Qt bug

//    header()->setFrameShape(QFrame::NoFrame);
//    header()->setFrameShadow(QFrame::Plain);

    m_itemsDelegate = new SceneDelegate();
    setItemDelegate(m_itemsDelegate);

    //setAcceptDrops(true);
//    setAnimated(false);

    connect( this, SIGNAL( collapsed( const QModelIndex& ) ),
            this, SLOT ( resizeViewToContents ( const QModelIndex& ) ) );
    connect( this, SIGNAL( expanded ( const QModelIndex& ) ),
            this, SLOT ( resizeViewToContents ( const QModelIndex& ) ) );
}

/*!
 * Destoryes view object.
 */
SceneView::~SceneView()
{
    delete m_itemsDelegate;
}

/**
 * Not yet docummented
 */
void SceneView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        startPos = event->pos();
    }
    else if( event->button() == Qt::RightButton)
    {
        emit showMenu( indexAt( event->pos() ) );    
    }
    QTreeView::mousePressEvent(event);
}

/**
 * Not yet docummented
 */
void SceneView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        int distance = (event->pos() - startPos).manhattanLength();
        if( distance >= QApplication::startDragDistance() )
            startDrag(event);
    }
   // QTreeView::mouseMoveEvent(event);
}

/**
 * Not yet docummented
 */
void SceneView::dragEnterEvent(QDragEnterEvent *event)
{
    event->setDropAction(Qt::MoveAction);
    event->accept();
}

/**
 * Not yet docummented
 */
void SceneView::dragMoveEvent(QDragMoveEvent *event)
{
    event->setDropAction(Qt::MoveAction);
    event->accept();
}

/**
 * Not yet docummented
 */
void SceneView::dropEvent(QDropEvent *event)
{
    QModelIndex newParent= indexAt(event->pos());
    if ( newParent.isValid() )
    {
        SceneModel* modelScene = static_cast< SceneModel* >(model());
        QString type = modelScene->NodeFromIndex( newParent )->GetNode()->getTypeId().getName().getString();
        
        SoNode* parentNode = modelScene->NodeFromIndex( newParent )->GetNode();
        if (! parentNode->getTypeId().isDerivedFrom( SoBaseKit::getClassTypeId() ) ) return;
    
        QByteArray data = event->mimeData()->data("text/objetID");
        QDataStream stream(&data, QIODevice::ReadOnly);
        QPoint position;
        stream >> position;
            
        QModelIndex nodeIndex = indexAt( position );
        
        if ( modelScene->parent( nodeIndex ) == newParent ) return;
        
        SoNode* childNode = modelScene->NodeFromIndex( nodeIndex )->GetNode();
        
            
        if ( ( parentNode->getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) ) && 
             !( childNode->getTypeId().isDerivedFrom( SoSeparatorKit::getClassTypeId() ) ) )  return;
             
        if ( ( parentNode->getTypeId().isDerivedFrom( TShapeKit::getClassTypeId() ) ) && 
             ( childNode->getTypeId().isDerivedFrom( SoBaseKit::getClassTypeId() ) ) )  return;
             
        if ( ( parentNode->getTypeId().isDerivedFrom( TShapeKit::getClassTypeId() ) ) &&
             !( childNode->getTypeId().isDerivedFrom( SoBaseKit::getClassTypeId() ) ) ) 
        {
            
            TShapeKit* shapeKit = static_cast< TShapeKit* > ( parentNode );
            if( shapeKit->getPart("shape", false) ) return;
        }
            
        QModelIndex ancestor = newParent;
        while(ancestor.parent().isValid())
        {
            if(ancestor.parent() == nodeIndex)
                return;
            else
                ancestor = ancestor.parent();    
        }
            
        if ( nodeIndex != newParent )
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

/*!
 * Sets \a current as the view current element index.
 */
void SceneView::currentChanged(const QModelIndex& current, const QModelIndex& previous )
{
    m_currentIndex = current;
    QTreeView::currentChanged(current, previous);
}

void SceneView::closeEditor( QWidget* editor, QAbstractItemDelegate::EndEditHint hint )
{
    QLineEdit* textEdit = qobject_cast<QLineEdit *>(editor);
    QString newValue = textEdit->text();

     emit nodeNameModificated( m_currentIndex, newValue );
     QTreeView::closeEditor( editor, hint );
}

/**
 * Resizes the view to the size of its contents.
 */
void SceneView::resizeViewToContents(const QModelIndex& index)
{
    resizeColumnToContents( index.column() );
}

/**
 * Not yet docummented
 */
void SceneView::startDrag(QMouseEvent* event)
{
    QPoint position = event->pos();
    QModelIndex index = indexAt(position);
    
    if( index.parent() == rootIndex() ) return;
    
    QByteArray itemPos;
    QDataStream stream (&itemPos, QIODevice::WriteOnly);
    stream << position;
        
    QMimeData* mimeData = new QMimeData;
    mimeData->setData("text/objetID", itemPos);
      
    QDrag* drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->start(Qt::MoveAction);
}
