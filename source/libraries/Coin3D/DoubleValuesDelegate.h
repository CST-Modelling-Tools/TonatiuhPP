#pragma once

#include "libraries/TonatiuhLibraries.h"

#include <QItemDelegate>


class TONATIUH_LIBRARIES DoubleValuesDelegate: public QItemDelegate
{
    Q_OBJECT

public:
    DoubleValuesDelegate(QObject* parent = 0);

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};
