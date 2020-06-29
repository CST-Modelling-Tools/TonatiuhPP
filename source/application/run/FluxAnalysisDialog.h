#pragma once

#include <QDialog>

namespace Ui {
class FluxAnalysisDialog;
}

#include "kernel/photons/Photons.h"

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
    void SelectSurface();
    void ChangeCurrentSurface();
    void ChangeCurrentSurfaceSide();
    void ExportData();
    void Run();
    void UpdateAnalysis();
    void UpdateSectorPlotSlot();
    void SelectExportFile();
    void UpdateLabelsUnits();
    void SaveCoordsExport();

private:
    void ClearCurrentAnalysis();
    void UpdateSurfaceSides(QString selectedSurfaceURL);

    void UpdateStatistics(double totalEnergy, double minimumFlux, double averageFlux, double maximumFlux,
                          double maxXCoord, double maxYCoord, double error, double uniformity, double gravityX, double gravityY);
    void UpdateFluxMapPlot(int** photonCounts, double wPhoton, int widthDivisions, int heightDivisions, double xmin, double ymin, double xmax, double ymax);
    void CreateSectorPlots(double xmin, double ymin, double xmax, double ymax);
    void UpdateSectorPlots(int** photonCounts, double wPhoton, int widthDivisions, int heightDivisions, double xmin, double ymin, double xmax, double ymax, double maximumFlux);

private:
    Ui::FluxAnalysisDialog* ui;
    FluxAnalysis* m_fluxAnalysis;
    SceneModel* m_sceneModel;
    QString m_currentSurfaceURL;
    QString m_fluxLabelString;
};
