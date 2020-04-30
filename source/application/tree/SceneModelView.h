#pragma once

#include <QTreeView>

#include "NodeNameDelegate.h"

class SceneModelView: public QTreeView
{
    Q_OBJECT

public:
    SceneModelView(QWidget* parent = 0);
    ~SceneModelView();

signals:
    void dragAndDrop(const QModelIndex& newParent, const QModelIndex& item);
    void dragAndDropCopy(const QModelIndex& newParent, const QModelIndex& item);
    void nodeNameModificated(const QModelIndex& index, const QString& newName);
    void showMenu(const QModelIndex& index);

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dropEvent(QDropEvent* event);

protected slots:
    void currentChanged (const QModelIndex& current, const QModelIndex& previous);
    void closeEditor (QWidget* editor, QAbstractItemDelegate::EndEditHint hint);
    void resizeViewToContents(const QModelIndex& index);

protected slots:

private:
    void startDrag(QMouseEvent* event);

    QModelIndex m_currentIndex;
    NodeNameDelegate* m_itemsDelegate;
    QPoint startPos;
};
