#pragma once

#include <QDialog>
#include <QItemSelectionModel>

#include "gui/SceneModel.h"
#include "ui_exportdialog.h"

//!  ExportDialog class is the dialog to define the photon map export mode.
/*!
   ExportDialog sets the photons to export, the coordinates system and the file to save selected information.
 */
class ExportDialog: public QDialog, private Ui::ExportDialog
{
    Q_OBJECT

public:
    ExportDialog(SceneModel& sceneModel, QString previousSurfaceUrl = 0, bool previusInGlobal = true, QString previousFile = 0, QWidget* parent = 0);
    ExportDialog(SceneModel& sceneModel, QWidget* parent);
    ~ExportDialog();

    bool ExportAllPhotonMap() const;
    bool ExportPhotonsInGlobal() const;
    QString GetExportFileName() const;
    QString GetSelectedSurface() const;

public slots:
    void accept();

private slots:
    void SetExportAllPhotons(bool allPhotos);
    void SetExportSurfacePhotons(bool surfacePhotos);
    void SelectFile();

private:
    SceneModel* m_exportSceneModel;
    QItemSelectionModel*  m_exportSelectionModel;
};
