#pragma once

#include <QTabWidget>

class SoNode;
class MainWindow;
class ParametersModel;


class ParametersTabs: public QTabWidget
{
    Q_OBJECT

public:
    ParametersTabs(QWidget* parent = 0);

    void setMain(MainWindow* main);
    void setNode(SoNode* node);

signals:
    void valueModified(SoNode* node, QString field, QString value);

public slots:
    void setValue(SoNode* node, QString field, QString value);

private:
    SoNode* m_node;
    ParametersModel* m_model;
};
