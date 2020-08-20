#pragma once

#include <QStandardItemModel>

class ParametersItem;
class SoNode;


class ParametersModel: public QStandardItemModel
{
    Q_OBJECT

public:
    ParametersModel(QObject* parent = 0);

    void setNode(SoNode* node);

//    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

signals:
    void valueModified(SoNode* node, QString field, QString value);

private:
    SoNode* m_node;
};
