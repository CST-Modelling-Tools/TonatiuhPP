#pragma once

#include <QDialog>
#include <QMap>


#include "ui_lightdialog.h"

class QComboBox;
class QFrame;
class QGroupBox;
class QItemSelectionModel;
class QTabWidget;
class SceneModel;
class TLightKit;
class ShapeAbstract;
class ShapeFactory;
class SunAbstract;
class SunFactory;

//!  LightDialog class is the dialog to define the light parameters.
/*!
   LightDialog sets light parameters. The parameters that user can define are light suunshape, light input aperture shape and light position.
 */
class LightDialog: public QDialog, private Ui::LightDialog
{
    Q_OBJECT

public:
    LightDialog(SceneModel& sceneModel, TLightKit* currentLightKit, QMap<QString, SunFactory*> sunShapeMap, QWidget* parent = 0);
    ~LightDialog();

    TLightKit* getLightKit();

public slots:
    void SetValue(SoNode* node, QString paramenterName, QString newValue);

protected slots:
    void ChangeSunshape(int index);
    void AddNodeToDisabledNodeList();
    void RemoveNodeFromDisabledNodeList();

private:
    //void GenerateNodeTree( QModelIndex parentIndex, QTreeWidgetItem* parentIntem );
    void makeSunApertureTab();
    void makeSunPositionTab();
    void makeSunShapeTab();

    SceneModel* m_sceneModel;
    QItemSelectionModel* m_selectionModel;
    TLightKit* m_lightKitOld;
    SunAbstract* m_sunNew;
    QMap<QString, SunFactory*> m_sunShapeMap;
    int m_currentSunShapeIndex;

};
