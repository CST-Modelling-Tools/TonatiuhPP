#include "ParametersModel.h"
#include "ParametersItem.h"


ParametersModel::ParametersModel(QObject* parent):
    QStandardItemModel(parent)
{

}

QVariant ParametersModel::data(const QModelIndex& index, int role) const
{
    ParametersItem* parameter = static_cast<ParametersItem*>(itemFromIndex(index));
    return parameter->data(role);
}

bool ParametersModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    ParametersItem* parameter = static_cast<ParametersItem*>(itemFromIndex(index));
    parameter->setData(value, role);
    return true;
}

ParametersItem* ParametersModel::getData(const QModelIndex& index) const
{
    ParametersItem* item = static_cast<ParametersItem*>(itemFromIndex(index));
    return item;
}
