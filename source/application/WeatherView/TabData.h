#pragma once

#include <QWidget>

namespace Ui {
class TabData;
}

class ModelTMY;
class Plotter;

class TabData: public QWidget
{
    Q_OBJECT

public:
    explicit TabData(QWidget* parent = 0);
    ~TabData();

    void setModelTMY(ModelTMY* model);

public slots:
    void onOpened();

private slots:
    void on_comboBox_DNI_textActivated(const QString& arg);
    void filterChanged();
    void on_pushButton_Reset_clicked();

    void mouseWheelCP(QWheelEvent*);
    void mouseMoveCP(QMouseEvent*);
    void mousePressCP(QMouseEvent*);
    void mouseReleaseCP(QMouseEvent*);

    void horzScrollBarChanged(int v);
    void vertScrollBarChanged(int value);

    void on_checkBox_stateChanged(int arg1);

private:
    void setRange(double tMin, double tMax);


private:
    Ui::TabData* ui;
    ModelTMY* m_modelData;
    Plotter* m_plotter;

    bool m_moveStart;
    double m_xpressed;
};

