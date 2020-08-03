#pragma once

#include <QStyledItemDelegate>


//!  NodeNameDelegate class to facility editing the name of a node.
/*!
NodeNameDelegate can be used to display and edit the name of a selected node. When a node is selected to edit
this class shows an editor with a validator
to enable only names with a valid regular expresion.
*/

// editor crated on double click
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
