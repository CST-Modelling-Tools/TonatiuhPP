#pragma once

#include <QDialog>
#include <QMap>

class SceneModel;
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
    SunDialog(SceneModel* sceneModel, QMap<QString, SunFactory*> sunShapeMap, QWidget* parent = 0);
    ~SunDialog();

    SunKit* getSun() {return m_sun;}

private:
    void makeSunPositionTab();
    void makeSunShapeTab();
    void makeSunApertureTab();

protected slots:
    void on_buttonSunCalc_clicked();
    void setShape(int index);
    void setValue(SoNode* node, QString parameter, QString value);
    void addNode();
    void removeNode();
    void onAccept();

private:
    Ui::SunDialog* ui;
    SceneModel* m_sceneModel;
    QMap<QString, SunFactory*> m_sunShapeMap;
    SunKit* m_sun;
};
