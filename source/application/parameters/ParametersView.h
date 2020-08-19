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

    void reset();

public slots:
    void onPressed(const QModelIndex &index);

//protected:
//    void mousePressEvent(QMouseEvent* event);
//    void mouseDoubleClickEvent(QMouseEvent* event);
};
