#pragma once

#include <QMainWindow>

namespace Ui {
    class WeatherDialog;
}

class ModelTMY;
class TreeModel;

class TabParameters;
class TabData;
class TabWeights;

class WeatherDialog: public QMainWindow
{
    Q_OBJECT

public:
    WeatherDialog(QWidget* parent = 0);
    ~WeatherDialog();

private:
    Ui::WeatherDialog* ui;
    ModelTMY* m_modelData; // hour table

    TabParameters* m_tabParameters;
    TabData* m_tabData;
    TabWeights* m_tabWeights;
};
