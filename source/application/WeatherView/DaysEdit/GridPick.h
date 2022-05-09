#pragma once

#include <QDialog>
#include <QTableView>
#include <QStandardItemModel>
#include <QModelIndex>

#include "DaysModel.h"

class GridPick: public QDialog
{
    Q_OBJECT

public:
    GridPick(QStandardItemModel* model, QWidget* parent = 0);

    QTableView* view() {return m_view;}

    void setDirection(Qt::Orientation direction);
    bool eventFilter(QObject* watched, QEvent* event);

private:
    QTableView* m_view;
    Qt::Orientation m_direction;
};

