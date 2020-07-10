#pragma once

#include <QDialog>
#include <QVector>

class RandomFactory;
class SceneModel;
class PhotonsFactory;
struct PhotonsSettings;

namespace Ui {
class RayTracingDialog;
}


class RayTracingDialog: public QDialog
{
    Q_OBJECT

public:
    RayTracingDialog(QWidget* parent = 0);
    ~RayTracingDialog();

    void setParameters(
        int raysNumber, int raysScreen,
        QVector<RandomFactory*> randomFactories, int raysRandomFactory = 0,
        int raysGridWidth = 200, int raysGridHeight = 200,
        int photonBufferSize = 1'000'000, bool photonBufferAppend = false);

    int raysNumber() const;
    int raysScreen() const;
    int raysRandomFactory() const;
    int raysGridWidth() const;
    int raysGridHeight() const;

    int photonBufferSize() const;
    bool photonBufferAppend() const;

    void setPhotonSettings(SceneModel* scene, QVector<PhotonsFactory*> factories, PhotonsSettings* ps);
    PhotonsSettings getPhotonSettings() const;

private slots:
    void outputChanged();
    void surfaceAdd();
    void surfaceDelete();

private:
    Ui::RayTracingDialog* ui;
    SceneModel* m_scene;
    QStringList m_surfaces;
};
