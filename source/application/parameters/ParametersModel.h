#pragma once

#include <QStandardItemModel>

class ParametersItem;

class ParametersModel: public QStandardItemModel
{

public:
    ParametersModel(QObject* parent = 0);

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

    ParametersItem* getData(const QModelIndex& index) const;
};
