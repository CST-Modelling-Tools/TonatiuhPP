#include "ComboBoxDelegate.h"

#include <QPainter>

void ComboBoxDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QString type = index.data(Qt::AccessibleDescriptionRole).toString();
    if (type == "separator")
    {
        painter->setPen(Qt::lightGray);
        painter->drawLine(option.rect.left(), option.rect.center().y(), option.rect.right(), option.rect.center().y());
    }
    else
        QStyledItemDelegate::paint(painter, option, index);
}

QSize ComboBoxDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QString type = index.data(Qt::AccessibleDescriptionRole).toString();
    if (type == "separator")
        return QSize(0, 1);
    else
        return QStyledItemDelegate::sizeHint(option, index);
}
