#include "GridPick.h"

#include <QHeaderView>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QStandardItemModel>
#include <QVBoxLayout>

GridPick::GridPick(QStandardItemModel* model, QWidget* parent):
    QDialog(parent)
{  
    // view
    m_view = new QTableView(this);
    m_view->setModel(model);

    m_view->horizontalHeader()->setVisible(false);
    m_view->verticalHeader()->setVisible(false);
    m_view->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_view->setSelectionMode(QAbstractItemView::SingleSelection);
    m_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_view->setShowGrid(false);

    connect(m_view, SIGNAL(pressed(QModelIndex)), this, SLOT(close()));

    // dialog
    setWindowFlags(Qt::Popup);
    setLayout(new QVBoxLayout);
    layout()->setContentsMargins(0, 0, 0, 0);
    layout()->addWidget(m_view);

    setStyleSheet(R"(
        QTableView {
            outline: 0;
         }
        QTableView::item:hover {
            background-color: #eeeeee;
            color: black;
        }
        QTableView::item:selected {
            background-color: #c8dbe5;
            color: black;
        }
    )");
    m_view->setFocus();
    m_view->installEventFilter(this);
    m_direction = Qt::Horizontal;
}

void GridPick::setDirection(Qt::Orientation direction)
{
    m_direction = direction;
}

bool GridPick::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == m_view) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

             int dx = 0;
             int dy = 0;
             if (keyEvent->key() == Qt::Key_Left) dx = -1;
             else if (keyEvent->key() == Qt::Key_Right) dx = +1;
             else if (keyEvent->key() == Qt::Key_Up) dy = -1;
             else if (keyEvent->key() == Qt::Key_Down) dy = +1;

             if (dx || dy) {
                 QModelIndex index = m_view->currentIndex();
                 int r = index.row();
                 int c = index.column();
                 if (m_direction == Qt::Horizontal) {
                     int cMax = m_view->model()->columnCount();
                     int n = r*cMax + c;
                     n += dy*cMax + dx;
                     r = n/cMax;
                     c = n%cMax;
                 } else if (m_direction == Qt::Vertical) {
                     int rMax = m_view->model()->rowCount();
                     int n = c*rMax + r;
                     n += dx*rMax + dy;
                     r = n%rMax;
                     c = n/rMax;
                 }
                 index = index.sibling(r, c);
                 if (index.isValid())
                    m_view->setCurrentIndex(index);
                 return true;
             }

            if (keyEvent->key() == Qt::Key_Return) {
                close();
                return true;
            }

//        } else if (event->type() == QEvent::MouseButtonPress) {
//            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
//            if (mouseEvent->button() == Qt::LeftButton) {
//                emit pickDay();
//                return true;
//            }
        }
    }
    return QWidget::eventFilter(watched, event);
}

