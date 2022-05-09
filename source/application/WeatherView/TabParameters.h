#pragma once

#include <QWidget>
#include <QStandardItem>

namespace Ui {
class TabParameters;
}

class ModelTMY;
class ParameterModel;

class TabParameters: public QWidget
{
    Q_OBJECT

public:
    explicit TabParameters(QWidget* parent = 0);
    ~TabParameters();

    void setModelTMY(ModelTMY* model);

signals:
    void fileOpened();

private slots:
    void on_pushButton_Open_clicked();
    void on_lineEdit_File_editingFinished();
    void on_active(QStandardItem*);

private:
    Ui::TabParameters* ui;

    ModelTMY* m_modelData; // all
    ParameterModel* m_paramsOriginal;
    ParameterModel* m_paramsEffective;
};

