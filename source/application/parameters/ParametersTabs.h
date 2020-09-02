#pragma once

#include <QTabWidget>

#include "ParametersView.h"

class SoNode;
class MainWindow;


class ParametersTabs: public QTabWidget
{
    Q_OBJECT

public:
    ParametersTabs(QWidget* parent = 0);

    ParametersView* getView() {return m_view;}
    void setNode(SoNode* node);

private:
    ParametersView* m_view;
};
