#pragma once

#include <QDialog>
#include <QVector>

#include "kernel/photons/PhotonExportFactory.h"

#include "ui_ExportPhotonsDialog.h"

class SceneModel;
class PhotonExportWidget;
struct PhotonExportSettings;

class ExportPhotonsDialog: public QDialog, private Ui::ExportPhotonsDialog
{
    Q_OBJECT

public:
    ExportPhotonsDialog(SceneModel& scene, QVector<PhotonExportFactory*> typeList, QWidget* parent = 0);
    ~ExportPhotonsDialog() {}

    PhotonExportSettings GetExportPhotonMapSettings() const;

private slots:
    void ChangeCurrentStoreTypeParameters();
    void AddSurface();
    void DeleteSurface();

private:
    SceneModel* m_scene;
    QStringList m_surfaces;
    QVector<PhotonExportWidget*> m_parameters;
};
