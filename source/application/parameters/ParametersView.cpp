#include "ParametersView.h"

#include "ParametersDelegate.h"
#include <QApplication>


ParametersView::ParametersView(QWidget* parent):
    QTreeView(parent)
{
    ParametersModel* model = new ParametersModel(this);
    setModel(model);

    setEditTriggers(QAbstractItemView::NoEditTriggers);

//    setItemDelegate(new ParametersDelegate(this));

    connect(
        this, SIGNAL(pressed(QModelIndex)),
        this, SLOT(onPressed(QModelIndex))
    );

    setStyleSheet(R"(
QLineEdit {
    border: 1px solid #c8dbe5;
    selection-background-color: #c8dbe5;
    selection-color: black;
}

QComboBox QAbstractItemView::item {
  padding-bottom: 0.2ex;
}

    )");
// border: 2px solid darkgray;
// selection-background-color: lightgray;
//    QComboBox QAbstractItemView::item {
//      padding-bottom: 0.2ex;
//    }

//    QComboBox QAbstractItemView::separator {
//      padding-bottom: 0.2ex;
//    }

//    QApplication::setEffectEnabled(Qt::UI_AnimateCombo, false);
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

    if (ParametersModel* m = getModel()) {
        QStandardItem* root = m->invisibleRootItem();
        root = root->child(0);
        if (root) setRootIndex(root->index());
    }

//    expandToDepth(1);
    expandAll();
}

void ParametersView::onPressed(const QModelIndex& index)
{
    if (model()->flags(index) & Qt::ItemIsEditable) {
//        setSelectionBehavior(QAbstractItemView::SelectItems);
//        selectionModel()->select(index, QItemSelectionModel::Clear);
        edit(index);
//        selectionModel()->select(index, QItemSelectionModel::Select);
//        setSelectionBehavior(QAbstractItemView::SelectRows);
    }
}

