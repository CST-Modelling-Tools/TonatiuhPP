#pragma once

#include <QDialog>

namespace Ui {
class FluxAnalysisDialog;
}

#include "kernel/photons/Photons.h"
#include "libraries/math/2D/Matrix2D.h"

class InstanceNode;
class SceneModel;
class QIntValidator;
class Random;
class TSceneKit;
class FluxAnalysis;


class FluxAnalysisDialog: public QDialog
{
    Q_OBJECT

public:
    FluxAnalysisDialog(TSceneKit* sceneKit, SceneModel& sceneModel, InstanceNode* rootInstance,
                       int sunWidthDivisions, int sunHeightDivisions,
                       Random* randomDeviate, QWidget* parent = 0);
    ~FluxAnalysisDialog();

private slots:
    void SurfaceSelected();
    void SurfaceChanged();
    void SideChanged();
    void UpdateAnalysis();
    void run();

    void UnitsChanged();
    void FormatChanged();
    void SelectDir();
    void Export();

    void UpdateSectorPlotSlot();

private:
    void ClearAnalysis();

    void UpdateStatistics(double powerTotal, double fluxMin, double fluxAverage, double fluxMax,
                          double fluxMaxU, double fluxMaxV, double error, double uniformity, double gravityX, double gravityY);
    void UpdateFluxMapPlot(const Matrix2D<int>& photonCounts, double powerPhoton, int widthDivisions, int heightDivisions, double xmin, double ymin, double xmax, double ymax);
    void CreateSectorPlots(double xMin, double yMin, double xMax, double yMax);
    void UpdateSectorPlots(const Matrix2D<int>& photonCounts, double wPhoton, int widthDivisions, int heightDivisions, double xmin, double ymin, double xmax, double ymax, double maximumFlux);

private:
    Ui::FluxAnalysisDialog* ui;
    FluxAnalysis* m_fluxAnalysis;
    SceneModel* m_sceneModel;
    QString m_fluxSurfaceURL;
    QString m_fluxLabel;
};
