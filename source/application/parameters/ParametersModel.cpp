#include "ParametersModel.h"
#include "ParametersItem.h"


ParametersModel::ParametersModel(QObject* parent):
    QStandardItemModel(parent)
{

}

ParametersItem* ParametersModel::getData(const QModelIndex& index) const
{
    ParametersItem* item = static_cast<ParametersItem*>(itemFromIndex(index));
    return item;
}

QVariant ParametersModel::data(const QModelIndex& index, int role) const
{
    ParametersItem* parameter = static_cast<ParametersItem*>(itemFromIndex(index));
    return parameter->data(role);
}

bool ParametersModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    ParametersItem* parameter = static_cast<ParametersItem*>(itemFromIndex(index));
    if (!parameter) return false;
    parameter->setData(value, role);
    emit itemChanged(parameter);
    return true;
}

Qt::ItemFlags ParametersModel::flags(const QModelIndex& index) const
{
    if (m_editable)
        return QStandardItemModel::flags(index);
    else
        return QStandardItemModel::flags(index) & ~Qt::ItemIsEditable;
}
