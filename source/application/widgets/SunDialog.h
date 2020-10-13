#pragma once

#include <QDialog>
#include <QMap>

class SceneTreeModel;
class SunFactory;
class SunKit;
class SoNode;

namespace Ui {
class SunDialog;
}


class SunDialog: public QDialog
{
    Q_OBJECT

public:
    SunDialog(SceneTreeModel* sceneModel, QWidget* parent = 0);
    ~SunDialog();

    SunKit* getSun() {return m_sun;}

private slots:
    void on_buttonSunCalc_clicked();

    void setFieldText(SoNode* node, QString field, QString value);
    void setFieldNode(SoNode* node, QString field, SoNode* value);
    void setAzEl(double az, double el);
    void onPressed(const QModelIndex& index);

    void addNode();
    void removeNode();

private:
    void makeSunPositionTab();
    void makeSunShapeTab();
    void makeSunApertureTab();
    void updateAperture();

    void initCustomPlot();
    void updateCustomPlot();

private:
    Ui::SunDialog* ui;
    SceneTreeModel* m_sceneModel;
    SunKit* m_sun;
};
