#pragma once

#include <QStandardItemModel>

class ParametersItem;


class ParametersModel: public QStandardItemModel
{

public:
    ParametersModel(QObject* parent = 0);

    ParametersItem* getData(const QModelIndex& index) const;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex& index) const;

    void SetEditable(bool editable) {m_editable = editable;}

private:
    bool m_editable;
};
