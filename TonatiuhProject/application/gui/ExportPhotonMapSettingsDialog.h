#pragma once

#include <QDialog>
#include <QVector>

#include "kernel/gui/PhotonMapExportFactory.h"

#include "ui_exportphotonmapsettingsdialog.h"

class PhotonMapExportParametersWidget;
class SceneModel;

struct PhotonMapExportSettings;

class ExportPhotonMapSettingsDialog: public QDialog, private Ui::ExportPhotonMapSettingsDialog
{
    Q_OBJECT

public:
    ExportPhotonMapSettingsDialog(SceneModel& currentSceneModel, QVector< PhotonMapExportFactory* > typeList, QWidget* parent = 0);
    ~ExportPhotonMapSettingsDialog();

    PhotonMapExportSettings GetExportPhotonMapSettings() const;

private slots:
    void AddSurfaceToExportList();
    void ChangeCurrentStoreTypeParameters();
    void DeleteSurfaceFromExportList();

private:
    void SetupTriggers();

    SceneModel* m_pCurrentSceneModel;
    QStringList m_exportSurfaceNodeList;
    QVector< PhotonMapExportParametersWidget* > parametersWidgetList;
};
