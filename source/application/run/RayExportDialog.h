#pragma once

#include <QDialog>
#include <QVector>

#include "kernel/photons/PhotonsAbstract.h"

class SceneModel;
class PhotonsWidget;
struct PhotonsSettings;


namespace Ui {
class RayExportDialog;
}

class RayExportDialog: public QDialog
{
    Q_OBJECT

public:
    RayExportDialog(SceneModel& scene, QVector<PhotonsFactory*> factories, QWidget* parent = 0);
    ~RayExportDialog();

    PhotonsSettings GetExportPhotonMapSettings() const;

private slots:
    void storageChanged();
    void surfaceAdd();
    void surfaceDelete();

private:
    Ui::RayExportDialog* ui;
    SceneModel* m_scene;
    QStringList m_surfaces;
    QVector<PhotonsWidget*> m_widgets;
};
