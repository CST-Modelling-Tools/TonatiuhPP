#pragma once

#include <QTreeView>

#include "ParametersModel.h"


class ParametersView: public QTreeView
{
    Q_OBJECT

public:
    ParametersView(QWidget* parent);

    ParametersModel* getModel();

    void reset();

private slots:
    void onPressed(const QModelIndex &index);
};
