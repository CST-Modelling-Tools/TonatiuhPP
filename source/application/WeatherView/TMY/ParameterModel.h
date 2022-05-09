#pragma once

#include <QStandardItemModel>

class ParameterAbstract;
class ParameterDouble;

class ParameterModel: public QStandardItemModel
{
public:
    explicit ParameterModel(QObject* parent = 0);

    void cleanRows();

    // parameters tree ("param.subparam")
    QStandardItem* findParameter(QString name) const;
    ParameterAbstract* parameterAbstract(QString name) const;
    ParameterDouble* parameterDouble(QString name) const;

    QString parameter(QString name) const;
    void setParameter(QString name, QString value);

    QStandardItem* appendParameter(QStandardItem* parent, ParameterAbstract* item);
};

