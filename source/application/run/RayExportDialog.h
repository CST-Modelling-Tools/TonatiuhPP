#pragma once

#include <QDialog>
#include <QVector>

#include "kernel/photons/PhotonsAbstract.h"

#include "ui_RayExportDialog.h"

class SceneModel;
class PhotonsWidget;
struct PhotonsSettings;

class RayExportDialog: public QDialog, private Ui::RayExportDialog
{
    Q_OBJECT

public:
    RayExportDialog(SceneModel& scene, QVector<PhotonsFactory*> typeList, QWidget* parent = 0);
    ~RayExportDialog() {}

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
