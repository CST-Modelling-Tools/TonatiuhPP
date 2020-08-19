#include "ParametersView.h"

#include <QMouseEvent>

#include "ParametersDelegate.h"


ParametersView::ParametersView(QWidget* parent):
    QTreeView(parent)
{
    ParametersDelegate* delegate = new ParametersDelegate(this);
    setItemDelegate(delegate);
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    setStyleSheet(R"(
QLineEdit {
padding-left: 1;
selection-background-color: #c8dbe5;
selection-color: black;
}
     )");
    connect(this, SIGNAL(pressed(QModelIndex)),
            this, SLOT(onPressed(QModelIndex))
            );
}

ParametersModel* ParametersView::getModel()
{
    return (ParametersModel*) model();
}

void ParametersView::reset()
{
    QTreeView::reset();
    double w = 2.5*fontMetrics().horizontalAdvance("Parameter");
    setColumnWidth(0, w);
}

void ParametersView::onPressed(const QModelIndex& index)
{
    if (currentIndex() != index) return;
    if (model()->flags(index) & Qt::ItemIsEditable)
        edit(index);
}

//#include <QDebug>
//void ParametersView::mousePressEvent(QMouseEvent* event)
//{
//    qDebug() << "press";
//    if (event->button() == Qt::LeftButton) {
//        QModelIndex index = indexAt(event->pos());
//        if (currentIndex() != index) {}
//        else if (model()->flags(index) & Qt::ItemIsEditable) {
//            edit(index);
//            return;
//        }
//    }
//    QTreeView::mousePressEvent(event);
//}

//void ParametersView::mouseDoubleClickEvent(QMouseEvent* event)
//{
//        qDebug() << "doublepress";
////    if (event->button() == Qt::LeftButton) {
////        QModelIndex index = indexAt(event->pos());
////       /* if (currentIndex() == index) {}
////        else*/ if (model()->flags(index) & Qt::ItemIsEditable) {
////            edit(index);
////            return;
////        }
////    }
//    QTreeView::mouseDoubleClickEvent(event);
//}
