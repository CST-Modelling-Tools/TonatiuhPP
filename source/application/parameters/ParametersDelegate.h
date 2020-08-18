#pragma once

#include <QStyledItemDelegate>

class SoNode;


class ParametersDelegate: public QStyledItemDelegate
{
    Q_OBJECT

public:
    ParametersDelegate(QObject* parent = 0);
    
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

public slots:
    void onCloseEditor();
    void commitAndCloseEditor();

signals:
    void closeEditor(QWidget*);
};
