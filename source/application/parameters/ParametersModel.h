#pragma once

#include <QStandardItemModel>

class ParametersItem;

class ParametersModel: public QStandardItemModel
{

public:
    ParametersModel(QObject* parent = 0);
    ~ParametersModel();

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

    Qt::ItemFlags flags(const QModelIndex& index) const;

    ParametersItem* ModelItem(const QModelIndex& index) const;
    void SetEditable(bool editable);

private:
    bool m_editable;
};
