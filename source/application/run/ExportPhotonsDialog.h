#pragma once

#include <QDialog>
#include <QVector>

#include "kernel/photons/PhotonsFactory.h"

#include "ui_ExportPhotonsDialog.h"

class SceneModel;
class PhotonsWidget;
struct PhotonsSettings;

class ExportPhotonsDialog: public QDialog, private Ui::ExportPhotonsDialog
{
    Q_OBJECT

public:
    ExportPhotonsDialog(SceneModel& scene, QVector<PhotonsFactory*> typeList, QWidget* parent = 0);
    ~ExportPhotonsDialog() {}

    PhotonsSettings GetExportPhotonMapSettings() const;

private slots:
    void ChangeCurrentStoreTypeParameters();
    void AddSurface();
    void DeleteSurface();

private:
    SceneModel* m_scene;
    QStringList m_surfaces;
    QVector<PhotonsWidget*> m_parameters;
};
