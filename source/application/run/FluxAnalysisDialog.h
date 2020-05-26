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
    FluxAnalysisDialog(TSceneKit* currentScene, SceneModel& currentSceneModel, InstanceNode* rootSeparatorInstance,
                       int sunWidthDivisions, int sunHeightDivisions,
                       Random* randomDeviate, QWidget* parent = 0);
    ~FluxAnalysisDialog();

protected:
    void resizeEvent(QResizeEvent* event);

private slots:
    void ChangeCurrentSurface();
    void ChangeCurrentSurfaceSide();
    void ExportData();
    void Run();
    void UpdateAnalysis();
    void UpdateSectorPlotSlot();
    void SelectExportFile();
    void SelectSurface();
    void UpdateLabelsUnits();
    void SaveCoordsExport();

private:
    Ui::FluxAnalysisDialog* ui;

    void UpdateStatistics(double totalEnergy, double minimumFlux, double averageFlux, double maximumFlux,
                          double maxXCoord, double maxYCoord, double error, double uniformity, double gravityX, double gravityY);
    void UpdateFluxMapPlot(int** photonCounts, double wPhoton, int widthDivisions, int heightDivisions, double xmin, double ymin, double xmax, double ymax);
    void CreateSectorPlots(double xmin, double ymin, double xmax, double ymax);
    void UpdateSectorPlots(int** photonCounts, double wPhoton, int widthDivisions, int heightDivisions, double xmin, double ymin, double xmax, double ymax, double maximumFlux);
    void ClearCurrentAnalysis();
    void UpdateSurfaceSides(QString selectedSurfaceURL);

    FluxAnalysis* m_fluxAnalysis;
    QString m_currentSurfaceURL;

    SceneModel* m_pCurrentSceneModel;

    QIntValidator* m_pGridWidthVal;
    QIntValidator* m_pGridHeightVal;
    QIntValidator* m_pNOfRays;

    //Labels
    QString m_fluxLabelString;
};
