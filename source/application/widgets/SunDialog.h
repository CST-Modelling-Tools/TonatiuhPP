#pragma once

#include <QDialog>
#include <QMap>

class SoNode;
class QComboBox;
class QFrame;
class QGroupBox;
class QItemSelectionModel;
class QTabWidget;
class SceneModel;
class SunKit;
class SunShape;
class SunFactory;

namespace Ui {
class SunDialog;
}

//!  LightDialog class is the dialog to define the light parameters.
/*!
   LightDialog sets light parameters. The parameters that user can define are light suunshape, light input aperture shape and light position.
 */
class SunDialog: public QDialog
{
    Q_OBJECT

public:
    SunDialog(SceneModel* sceneModel, SunKit* sunKit, QMap<QString, SunFactory*> sunShapeMap, QWidget* parent = 0);
    ~SunDialog();

    SunKit* getSunKit();

public slots:
    void SetValue(SoNode* node, QString parameter, QString value);

protected slots:
    void ChangeSunshape(int index);
    void addNode();
    void removeNode();

private:
    Ui::SunDialog* ui;

    void makeSunPositionTab();
    void makeSunShapeTab();
    void makeSunApertureTab();

    SceneModel* m_sceneModel;
    QItemSelectionModel* m_selectionModel;
    SunKit* m_sunKit;
    SunShape* m_sunShape;
    QMap<QString, SunFactory*> m_sunShapeMap;
    int m_currentSunShapeIndex;
};
