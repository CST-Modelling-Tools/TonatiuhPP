#pragma once

#include <QStyledItemDelegate>

class StyledItemDelegate: public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit StyledItemDelegate(QObject* parent = 0);

protected:
    void initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const;
};

