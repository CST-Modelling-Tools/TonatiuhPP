#pragma once

#include <QTreeView>

class ParametersModel;


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
