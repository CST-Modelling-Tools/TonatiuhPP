#pragma once

#include <QTreeView>

class TreeView: public QTreeView
{
    Q_OBJECT

public:
    TreeView(QWidget* parent = 0);

private slots:
    void onPressed(QModelIndex index);
};

