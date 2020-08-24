#pragma once

#include <QStyledItemDelegate>


class SceneDelegate: public QStyledItemDelegate
{
    Q_OBJECT

public:
    SceneDelegate(QObject* parent = 0);

    QWidget* createEditor(
        QWidget* parent,
        const QStyleOptionViewItem& option,
        const QModelIndex& index
    ) const;

    void setEditorData(QWidget* editor, const QModelIndex& index) const;
};
