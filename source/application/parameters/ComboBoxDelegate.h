#pragma once

#include <QStyledItemDelegate>


class ComboBoxDelegate: public QStyledItemDelegate
{
    Q_OBJECT
public:
    ComboBoxDelegate(QObject* parent = 0): QStyledItemDelegate(parent) {}

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

