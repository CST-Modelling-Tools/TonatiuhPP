#pragma once

#include <QTabWidget>

class SoBaseKit;
class SoNode;
class MainWindow;


class ParametersTabs: public QTabWidget
{
    Q_OBJECT

public:
    ParametersTabs(QWidget* parent = 0);

    void setNode(SoNode* node);
    void updateNode();

    void setMain(MainWindow* main) {m_main = main;}
    MainWindow* getMain() const {return m_main;}

signals:
    void valueModified(SoNode* node, QString field, QString value);

public slots:
    void setValue(SoNode* node, QString field, QString value);

private:
    SoNode* m_node;
    MainWindow* m_main;
};
