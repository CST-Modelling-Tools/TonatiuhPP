#pragma once

#include <QTreeView>

class ParametersModel;
class SoNode;


class ParametersView: public QTreeView
{
    Q_OBJECT

public:
    ParametersView(QWidget* parent);

    ParametersModel* getModel();

public slots:
    void reset();

protected:
    void mousePressEvent(QMouseEvent* event);
};
