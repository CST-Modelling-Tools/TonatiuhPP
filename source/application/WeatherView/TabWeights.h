#pragma once

#include <QWidget>

namespace Ui {
class TabWeights;
}

class ModelTMY;
namespace sp {
    class SunSpatial;
}

class ParameterModel;


class TabWeights: public QWidget
{
    Q_OBJECT

public:
    explicit TabWeights(QWidget* parent = 0);
    ~TabWeights();

    void setModelTMY(ModelTMY* model);

private slots:
    void on_pushButton_Generate_pressed();

    void on_pushButton_Save_pressed();

    void on_pushButton_selectDir_pressed();

    void on_pushButton_openDir_pressed();

private:
    QString writeSampling();
    QString writeScript();

private:
    Ui::TabWeights* ui;
    ModelTMY* m_modelData;

    ParameterModel* m_paramsModel;
    sp::SunSpatial* m_sunSpatial;
};

