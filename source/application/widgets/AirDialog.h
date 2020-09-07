#pragma once

#include <QDialog>

class TSceneKit;
class AirKit;
class SoNode;

namespace Ui {
class AirDialog;
}


class AirDialog: public QDialog
{
    Q_OBJECT

public:
    AirDialog(TSceneKit* sceneKit, QWidget* parent = 0);
    ~AirDialog();

    AirKit* getAir() {return m_air;}

private slots:
    void setFieldText(SoNode* node, QString field, QString value);
    void setFieldNode(SoNode* node, QString field, SoNode* value);

private:
    void initCustomPlot();
    void updateCustomPlot();

private:
    Ui::AirDialog* ui;
    AirKit* m_air;
};
