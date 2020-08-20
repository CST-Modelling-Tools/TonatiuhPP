#include "ParametersView.h"

#include "ParametersDelegate.h"


ParametersView::ParametersView(QWidget* parent):
    QTreeView(parent)
{
    ParametersDelegate* delegate = new ParametersDelegate(this);
    setItemDelegate(delegate);
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(
        this, SIGNAL(pressed(QModelIndex)),
        this, SLOT(onPressed(QModelIndex))
    );

    setStyleSheet(R"(
QLineEdit {
padding-left: 1;
selection-background-color: #c8dbe5;
selection-color: black;
}
     )");
}

ParametersModel* ParametersView::getModel()
{
    return (ParametersModel*) model();
}

void ParametersView::reset()
{
    QTreeView::reset(); // keep one model to avoid reset?
    double w = 3*fontMetrics().horizontalAdvance("Parameter");
    setColumnWidth(0, w);
}

void ParametersView::onPressed(const QModelIndex& index)
{
    if (model()->flags(index) & Qt::ItemIsEditable)
        edit(index);
}

