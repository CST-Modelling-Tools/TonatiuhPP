#pragma once

#include <QDialog>
#include <QVector>

#include "kernel/gui/PhotonMapExportFactory.h"

#include "ui_ExportPhotonsDialog.h"

class SceneModel;
class PhotonMapExportParametersWidget;
struct PhotonMapExportSettings;

class ExportPhotonsDialog: public QDialog, private Ui::ExportPhotonsDialog
{
    Q_OBJECT

public:
    ExportPhotonsDialog(SceneModel& scene, QVector<PhotonMapExportFactory*> typeList, QWidget* parent = 0);
    ~ExportPhotonsDialog() {}

    PhotonMapExportSettings GetExportPhotonMapSettings() const;

private slots:
    void ChangeCurrentStoreTypeParameters();
    void AddSurface();
    void DeleteSurface();

private:
    void SetupTriggers();

    SceneModel* m_scene;
    QStringList m_surfaces;
    QVector<PhotonMapExportParametersWidget*> m_parameters;
};
