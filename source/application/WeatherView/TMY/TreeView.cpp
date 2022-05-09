#include "TreeView.h"
#include "TMY/StyledItemDelegate.h"

#include <QStandardItemModel>

TreeView::TreeView(QWidget* parent):
    QTreeView(parent)
{
    setItemDelegate(new StyledItemDelegate);

    setEditTriggers(QAbstractItemView::NoEditTriggers);
    //setEditTriggers(QAbstractItemView::AllEditTriggers); // faster via slot

    connect(this, SIGNAL(pressed(QModelIndex)),
            this, SLOT(onPressed(QModelIndex)));
}

void TreeView::onPressed(QModelIndex index)
{
    QStandardItemModel* m = dynamic_cast<QStandardItemModel*>(model());
    if (!m) return;
    QStandardItem* item = m->itemFromIndex(index);
    if (!item->isEditable()) return;
    edit(index);
}
