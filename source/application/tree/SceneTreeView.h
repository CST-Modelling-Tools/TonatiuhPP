#pragma once

#include <QTreeView>

#include "SceneDelegate.h"


class SceneTreeView: public QTreeView
{
    Q_OBJECT

public:
    SceneTreeView(QWidget* parent = 0);

signals:
    void nodeRenamed(const QModelIndex& index, const QString& newName);
    void dragAndDrop(const QModelIndex& indexParent, const QModelIndex& index, bool isMove);
    void showMenu(const QModelIndex& index);

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dropEvent(QDropEvent* event);

protected slots:
    void closeEditor(QWidget* editor, QAbstractItemDelegate::EndEditHint hint);

private:
    void startDrag(QMouseEvent* event);

    QPoint m_startPos;
};
