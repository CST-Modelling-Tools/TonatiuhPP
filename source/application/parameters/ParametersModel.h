#pragma once

#include <QStandardItemModel>

class SoNode;


class ParametersModel: public QStandardItemModel
{
    Q_OBJECT

public:
    ParametersModel(QObject* parent = 0);

    void setNode(SoNode* node);

    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

signals:
    void valueModified(SoNode* node, QString field, QString value);
};
