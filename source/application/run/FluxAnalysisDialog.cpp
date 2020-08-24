#include "FluxAnalysisDialog.h"
#include "ui_FluxAnalysisDialog.h"

#include <cmath>

#include <QFileDialog>
#include <QFutureWatcher>
#include <QIntValidator>
#include <QMessageBox>
#include <QMutex>
#include <QPair>
#include <QProgressDialog>
#include <QtConcurrentMap>

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/nodes/SoTransform.h>

#include "FluxAnalysis.h"
#include "SelectSurfaceDialog.h"
#include "tree/SceneModel.h"
#include "kernel/air/AirTransmission.h"
#include "kernel/run/InstanceNode.h"
#include "kernel/random/Random.h"
#include "kernel/run/RayTracer.h"
#include "kernel/scene/TSceneKit.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/trf.h"
#include "kernel/shape/ShapeRT.h"
#include "kernel/sun/SunKit.h"
#include "kernel/sun/SunAperture.h"
#include "libraries/math/3D/Transform.h"
#include "libraries/math/gcf.h"



FluxAnalysisDialog::FluxAnalysisDialog(TSceneKit* sceneKit, SceneModel* sceneModel,
                                       int sunWidthDivisions, int sunHeightDivisions,
                                       Random* randomDeviate,  QWidget* parent):
    QDialog(parent),
    ui(new Ui::FluxAnalysisDialog),
    m_sceneModel(sceneModel),
    m_fluxLabel("Flux, W/m^2")
{
    ui->setupUi(this);

    m_fluxAnalysis = new FluxAnalysis(sceneKit, sceneModel, sunWidthDivisions, sunHeightDivisions, randomDeviate);

    connect(ui->surfaceButton, SIGNAL(clicked()), this, SLOT(SurfaceSelected()));
    connect(ui->surfaceEdit, SIGNAL(editingFinished()), this, SLOT(SurfaceChanged()));
    connect(ui->surfaceSideCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(SideChanged()));
    connect(ui->surfaceXSpin, SIGNAL(editingFinished()), this, SLOT(UpdateAnalysis()));
    connect(ui->surfaceYSpin, SIGNAL(editingFinished()), this, SLOT(UpdateAnalysis()));
    connect(ui->raysButton, SIGNAL(clicked()), this, SLOT(run()));

    connect(ui->exportLengthEdit, SIGNAL(editingFinished()), this, SLOT(UnitsChanged()));
    connect(ui->exportPowerEdit, SIGNAL(editingFinished()), this, SLOT(UnitsChanged()));
    connect(ui->exportFileButton, SIGNAL(clicked()), this, SLOT(SelectDir()));
    connect(ui->exportFormatCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(FormatChanged()));
    connect(ui->exportButton, SIGNAL(clicked()), this, SLOT(Export()) );

    connect(ui->spinX, SIGNAL(valueChanged(double)), this, SLOT(UpdateSectorPlotSlot()));
    connect(ui->spinY, SIGNAL(valueChanged(double)), this, SLOT(UpdateSectorPlotSlot()));

    ui->plotFxy->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom); // this will also allow rescaling the color scale by dragging/zooming
    ui->plotFxy->axisRect()->setupFullAxesBox(true);
    ui->plotFxy->plotLayout()->insertRow(0);
    ui->plotFxy->plotLayout()->addElement(0, 0, new QCPTextElement(ui->plotFxy, "Flux Distribution") );
    ui->plotFxy->xAxis->setLabel("x, m");
    ui->plotFxy->yAxis->setLabel("y, m");

    ui->plotFx->xAxis->setLabel("x, m");
    ui->plotFx->yAxis->setLabel("Flux, W/m^2");
    ui->plotFx->yAxis->setRange(0, 1.08);

    ui->plotFy->xAxis->setLabel("y, m");
    ui->plotFy->yAxis->setLabel("Flux, W/m^2");
    ui->plotFy->yAxis->setRange(0, 1.08);

    int q = fontMetrics().height();
    resize(64*q, 48*q);
}

FluxAnalysisDialog::~FluxAnalysisDialog()
{
    delete m_fluxAnalysis;
    delete ui;
}

QStringList sFilters = {"Planar", "Parabolic", "Elliptic", "Hyperbolic", "Cylinder"};
//#include <QSet>
//QSet<QString> setFilters = {"Planar", "Parabolic", "Elliptic", "Hyperbolic", "Cylinder"};

/*!
 * Opens a dialog to select a surface from the system.
 * If the selected surface is ok
 */
void FluxAnalysisDialog::SurfaceSelected()
{
    SelectSurfaceDialog dialog(*m_sceneModel, false, this);
    dialog.SetShapeTypeFilters(sFilters);
    if (!dialog.exec()) return;

    QString fluxSurfaceURL = dialog.GetSelectedSurfaceURL();
    if (fluxSurfaceURL.isEmpty()) return;
    if (fluxSurfaceURL == m_fluxSurfaceURL) return;

    m_fluxAnalysis->clear();
    ClearAnalysis();
    ui->surfaceEdit->setText(fluxSurfaceURL);
    m_fluxSurfaceURL = fluxSurfaceURL;
}

/*!
 * Changes current surface to the surface node defined in surface line.
 */
void FluxAnalysisDialog::SurfaceChanged()
{
    QString fluxSurfaceURL = ui->surfaceEdit->text();
    if (fluxSurfaceURL.isEmpty() || fluxSurfaceURL == m_fluxSurfaceURL)
    {
        ui->surfaceEdit->setText(m_fluxSurfaceURL);
        return;
    }

    QString shapeType = m_fluxAnalysis->getShapeType(fluxSurfaceURL);
    if (sFilters.contains(shapeType))
    {
        m_fluxAnalysis->clear();
        ui->raysAppendCheck->setChecked(false);
        ui->raysAppendCheck->setEnabled(false);
        ClearAnalysis();
        ui->surfaceEdit->setText(fluxSurfaceURL);
        m_fluxSurfaceURL = fluxSurfaceURL;
    }
}

/*
 * Clear the current analysis if the surface side is modified
 */
void FluxAnalysisDialog::SideChanged()
{
    m_fluxAnalysis->clear();
    ui->raysAppendCheck->setChecked(false);
    ui->raysAppendCheck->setEnabled(false);
    ClearAnalysis();
}

/*
 * Calculate flux distribution and statistics
 */
void FluxAnalysisDialog::UpdateAnalysis()
{
//	int pos = 0;
//    QString withValue = ui->spinCellsX->text();
//    QValidator::State widthState = m_pGridWidthVal->validate(withValue, pos);

//    QString heightValue = ui->surfaceYSpin->text();
//    QValidator::State heightSate = m_pGridHeightVal->validate(heightValue, pos);

//    if ( (widthState != QValidator::Acceptable) || (heightSate != QValidator::Acceptable) )
//	{
//        QMessageBox::warning(this, "Tonatiuh",
//                             tr("The gird divisions size must be greater than 2.") );
//		return;
//	}

    if (m_fluxAnalysis->getBins().isEmpty())
        return;

    int xDivs = ui->surfaceXSpin->value();
    int yDivs = ui->surfaceYSpin->value();
    m_fluxAnalysis->setBins(xDivs, yDivs);

    const Matrix2D<int>& photonCounts = m_fluxAnalysis->getBins();

    ClearAnalysis();

    double uMin = m_fluxAnalysis->uMin();
    double uMax = m_fluxAnalysis->uMax();
    double vMin = m_fluxAnalysis->vMin();
    double vMax = m_fluxAnalysis->vMax();
    double powerPhoton = m_fluxAnalysis->powerPhoton();
    double powerTotal = m_fluxAnalysis->powerTotal();

    double uStep = (uMax - uMin)/xDivs;
    double vStep = (vMax - vMin)/yDivs;
    double areaCell = uStep*vStep;
    double fluxMin = 0.;
    double fluxAverage = powerTotal/(areaCell*xDivs*yDivs);
    double fluxMax = m_fluxAnalysis->photonsMax()*powerPhoton/areaCell;
    double fluxMaxU = uMin + (m_fluxAnalysis->photonsMaxRow() + 0.5)*uStep;
    double fluxMaxV = vMin + (m_fluxAnalysis->photonsMaxCol() + 0.5)*vStep;
    double areaE = (uMax - uMin)/(xDivs - 1)*(vMax - vMin)/(yDivs - 1);
    double maximumFluxError = m_fluxAnalysis->photonsError()*powerPhoton/areaE;
    double error = fabs(fluxMax - maximumFluxError)/fluxMax;
    double fluxMeanU = 0.;
    double fluxMeanV = 0.;
    double E = 0;

    for (int r = 0; r < xDivs; ++r)
    {
        for (int c = 0; c < yDivs; ++c)
        {
            double fluxCell = photonCounts(r, c)*powerPhoton/areaCell;
            if (fluxMin > fluxCell) fluxMin = fluxCell;

            fluxMeanU += fluxCell*(uMin + (r + 0.5)*uStep);
            fluxMeanV += fluxCell*(vMin + (c + 0.5)*vStep);
            E += gcf::pow2(fluxCell - fluxAverage);
        }
    }

    double standardDeviation = sqrt(E/(xDivs*yDivs));
    double uniformity = standardDeviation/fluxAverage;
    double fluxNorm = powerTotal/areaCell;
    fluxMeanU /= fluxNorm;
    fluxMeanV /= fluxNorm;

    UpdateStatistics(powerTotal, fluxMin, fluxAverage, fluxMax, fluxMaxU, fluxMaxV, error, uniformity, fluxMeanU, fluxMeanV);
    UpdateFluxMapPlot(photonCounts, powerPhoton, xDivs, yDivs, uMin, vMin, uMax, vMax);
    CreateSectorPlots(uMin, vMin, uMax, vMax);
    UpdateSectorPlots(photonCounts, powerPhoton, xDivs, yDivs, uMin, vMin, uMax, vMax, fluxMax);
}

/*!
 * Runs flux analysis for current defined surface.
 */
void FluxAnalysisDialog::run()
{
    QElapsedTimer timer;
    timer.start();

    if (m_fluxSurfaceURL.isEmpty())
    {
        QMessageBox::warning(this, "Tonatiuh", "Select a valid surface.");
        return;
    }
    QString surfaceSide = ui->surfaceSideCombo->currentText();
    bool increasePhotonMap = ui->raysAppendCheck->isEnabled() && ui->raysAppendCheck->isChecked();

    m_fluxAnalysis->run(m_fluxSurfaceURL, surfaceSide, ui->raysSpin->value(), increasePhotonMap, ui->surfaceXSpin->value(), ui->surfaceYSpin->value());
    UpdateAnalysis();
    ui->raysAppendCheck->setEnabled(true);

    std::cout << "Elapsed time: " << timer.elapsed() << std::endl;
}

/*
 * Updates the labels of the plots
 */
void FluxAnalysisDialog::UnitsChanged()
{
    QString lengthUnit = ui->exportLengthEdit->text();
    if (lengthUnit.isEmpty()) lengthUnit = "m";

    QString powerUnit = ui->exportPowerEdit->text();
    if (powerUnit.isEmpty()) powerUnit = "W";

    ui->plotFxy->xAxis->setLabel(QString("x, %1").arg(lengthUnit));
    ui->plotFxy->yAxis->setLabel(QString("y, %1").arg(lengthUnit));
    m_fluxLabel = QString("Flux, %1/%2^2").arg(powerUnit, lengthUnit);
    QCPColorMap* colorMapPlot = qobject_cast<QCPColorMap*>(ui->plotFxy->plottable());
    for (int i = 0; i < ui->plotFxy->plotLayout()->elementCount(); i++)
    {
        if (qobject_cast<QCPColorScale*>(ui->plotFxy->plotLayout()->elementAt(i) ) )
            colorMapPlot->colorScale()->axis()->setLabel(m_fluxLabel);
    }
    ui->plotFxy->replot();

    ui->plotFx->xAxis->setLabel(QString("x, %1").arg(lengthUnit));
    ui->plotFx->yAxis->setLabel(QString("Flux, %1/%2^2").arg(powerUnit, lengthUnit));
    ui->plotFx->replot();

    ui->plotFy->xAxis->setLabel(QString("y, %1").arg(lengthUnit));
    ui->plotFy->yAxis->setLabel(QString("Flux, %1/%2^2").arg(powerUnit, lengthUnit));
    ui->plotFy->replot();
}

/*
 * Set the coords check box according to the store type
 */
void FluxAnalysisDialog::FormatChanged()
{
    if (ui->exportFormatCombo->currentText() == "png" || ui->exportFormatCombo->currentText() == "jpg")
    {
        ui->exportGridCheck->setChecked(false);
        ui->exportGridCheck->setDisabled(true);
    }
    else
        ui->exportGridCheck->setEnabled(true);
}

/*
 * Select directory
 */
void FluxAnalysisDialog::SelectDir()
{
    QString path = QFileDialog::getExistingDirectory(this, "Choose a directory to save");
    ui->exportDirEdit->setText(path);
}

/*
 * Exports the flux distribution
 */
void FluxAnalysisDialog::Export()
{
    const Matrix2D<int>& photonCounts = m_fluxAnalysis->getBins();
    if (photonCounts.data().isEmpty())
    {
        QString message = QString("Nothing available to export, first run the simulation");
        QMessageBox::warning(this, "Tonatiuh", message);
        return;
    }

    if (ui->exportDirEdit->text().isEmpty())
    {
        QString message("Directory not valid");
        QMessageBox::warning(this, "Tonatiuh", message);
        return;
    }

    if (ui->exportFileEdit->text().isEmpty())
    {
        QString message("File name not valid");
        QMessageBox::warning(this, "Tonatiuh", message);
        return;
    }

    QString exportDirectory(ui->exportDirEdit->text());
    QString exportFormat = ui->exportFormatCombo->currentText();
    QString exportFileName = ui->exportFileEdit->text();

    QFileInfo exportFileInfo(exportFileName);
    if (exportFormat == "txt")
    {
        m_fluxAnalysis->write(exportDirectory, exportFileName, ui->exportGridCheck->isChecked());
    }
    else if (exportFormat == "jpg")
    {
        if (exportFileInfo.completeSuffix().compare("jpg") )
            exportFileName.append(".jpg");

        QFile exportFile(exportDirectory + "/" + exportFileName);
        exportFile.open(QIODevice::WriteOnly);
        ui->plotFxy->saveJpg(exportFile.fileName());
        exportFile.close();
    }
    else if (exportFormat == "png")
    {
        if (exportFileInfo.completeSuffix().compare("png") )
            exportFileName.append(".png");

        QFile exportFile(exportDirectory + "/" + exportFileName);
        exportFile.open(QIODevice::WriteOnly);
        ui->plotFxy->savePng(exportFile.fileName());
        exportFile.close();
    }

    QString message("Export done successfully");
    QMessageBox::information(this, "Tonatiuh", message);
}

/*
 * Update plots when changing x or y coord
 */
void FluxAnalysisDialog::UpdateSectorPlotSlot()
{
    const Matrix2D<int>& photonCounts = m_fluxAnalysis->getBins();
//    if (!photonCounts || photonCounts == 0) return;

    double xmin = m_fluxAnalysis->uMin();
    double ymin = m_fluxAnalysis->vMin();
    double xmax = m_fluxAnalysis->uMax();
    double ymax = m_fluxAnalysis->vMax();
    double wPhoton = m_fluxAnalysis->powerPhoton();
    int widthDivisions = ui->surfaceXSpin->value();
    int heightDivisions = ui->surfaceYSpin->value();
    double widthCell = (xmax - xmin)/widthDivisions;
    double heightCell = (ymax - ymin)/heightDivisions;
    double areaCell = widthCell*heightCell;
    double maximumFlux = (m_fluxAnalysis->photonsMax()*wPhoton)/areaCell;
    UpdateSectorPlots(photonCounts, wPhoton, widthDivisions, heightDivisions, xmin, ymin, xmax, ymax, maximumFlux);
}

/*!
 * Clear current results and simulation.
 */
void FluxAnalysisDialog::ClearAnalysis()
{
    ui->plotFy->clearPlottables();
    ui->plotFy->yAxis->setRange(0, 1.05);
    ui->plotFy->replot();

    ui->plotFx->clearPlottables();
    ui->plotFx->yAxis->setRange(0, 1.05);
    ui->plotFx->replot();

    ui->plotFxy->clearPlottables();
    for (int i = 0; i < ui->plotFxy->plotLayout()->elementCount(); i++)
    {
        //test to see if any of the layout elements are of QCPColorScale type
        if (qobject_cast<QCPColorScale*>(ui->plotFxy->plotLayout()->elementAt(i)))
            ui->plotFxy->plotLayout()->removeAt(i);
        //collapse the empty elements
        ui->plotFxy->plotLayout()->simplify();
    }
    ui->plotFxy->replot();

    ui->spinX->setValue(0.);
    ui->spinX->setMinimum(0.);
    ui->spinX->setMaximum(0.);
    ui->spinY->setValue(0.);
    ui->spinY->setMinimum(0.);
    ui->spinY->setMaximum(0.);

    ui->powerTotal->setText(QString::number(0.));
    ui->fluxMin->setText(QString::number(0.));
    ui->fluxAverage->setText(QString::number(0.));
    ui->fluxMax->setText(QString::number(0.));

    ui->fluxMaxUV->setText(" ; ");
    ui->errorValue->setText(QString::number(0.));
    ui->uniformityValue->setText(QString::number(0.));
    ui->centroidValue->setText(" ; ");
}

/*
 * Updates the statistics
 */
void FluxAnalysisDialog::UpdateStatistics(double powerTotal, double fluxMin, double fluxAverage, double fluxMax,
                                          double fluxMaxU, double fluxMaxV, double error, double uniformity, double gravityX, double gravityY)
{
    ui->powerTotal->setText(QString::number(powerTotal));
    ui->fluxMin->setText(QString::number(fluxMin));
    ui->fluxAverage->setText(QString::number(fluxAverage));
    ui->fluxMax->setText(QString::number(fluxMax));
    ui->fluxMaxUV->setText(QString("%1 ; %2").arg(QString::number(fluxMaxU), QString::number(fluxMaxV)));
    ui->errorValue->setText(QString::number(error) );
    ui->uniformityValue->setText(QString::number(uniformity) );
    ui->centroidValue->setText(QString("%1 ; %2").arg(QString::number(gravityX), QString::number(gravityY)));
}

/*
 * Updates the flux map plot
 */
void FluxAnalysisDialog::UpdateFluxMapPlot(const Matrix2D<int>& photonCounts, double powerPhoton, int xDivs, int yDivs, double xMin, double yMin, double xMax, double yMax)
{
	//Delete previous colormap, scale
    ui->plotFxy->clearPlottables();
    ui->plotFxy->clearItems();

    for (int i = 0; i < ui->plotFxy->plotLayout()->elementCount(); i++)
	{
        if (qobject_cast<QCPColorScale*>(ui->plotFxy->plotLayout()->elementAt(i) ) )
            ui->plotFxy->plotLayout()->removeAt(i);
        //collapse the empty elements
        ui->plotFxy->plotLayout()->simplify();
	}

	// Create a QCPColorMap object to draw flux distribution
    QCPColorMap* colorMap = new QCPColorMap(ui->plotFxy->xAxis, ui->plotFxy->yAxis);
//    ui->plotFxy->addPlottable(colorMap);

    colorMap->data()->setSize(xDivs, yDivs);   // we want the color map to have widthDivisions * heightDivisions data points
    colorMap->data()->setRange(QCPRange(xMin, xMax), QCPRange(yMin, yMax));      // and span the coordinate range -4..4 in both key (x) and value (y) dimensions

	//Assign flux data
    double xStep = (xMax - xMin)/xDivs;
    double yStep = (yMax - yMin)/yDivs;
    double areaCell = xStep*yStep;
    for (int x = 0; x < xDivs; ++x)
        for (int y = 0; y < yDivs; ++y)
            colorMap->data()->setCell(x, y, photonCounts(x, y)*powerPhoton/areaCell);

	// add a color scale:
    QCPColorScale* colorScale = new QCPColorScale(ui->plotFxy);
    ui->plotFxy->plotLayout()->addElement(1, 1, colorScale); // add it to the right of the main axis rect
    colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
    colorScale->axis()->setLabel(m_fluxLabel);
    colorScale->setRangeZoom(false); // forbid to drag and zoom the colorscale
    colorScale->setRangeDrag(false);

    colorMap->setColorScale(colorScale);   // associate the color map with the color scale
    colorMap->setGradient(QCPColorGradient::gpThermal); // set the contour plot color
    colorMap->rescaleDataRange(); // rescale the data dimension (color) such that all data points lie in the span visualized by the color gradient:

	// make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
    QCPMarginGroup* marginGroup = new QCPMarginGroup(ui->plotFxy);
    ui->plotFxy->axisRect()->setMarginGroup(QCP::msBottom | QCP::msTop, marginGroup);
    colorScale->setMarginGroup(QCP::msBottom | QCP::msTop, marginGroup);

	// rescale axes and update plot view
    ui->plotFxy->rescaleAxes();
    ui->plotFxy->replot();
}

/*
 * Create sector plots
 */
void FluxAnalysisDialog::CreateSectorPlots(double xMin, double yMin, double xMax, double yMax)
{
    QPen pen(QColor(137, 140, 140), 1);

    // Create a vertical and horizontal line for sectors
    ui->spinX->setMinimum(xMin);
    ui->spinX->setMaximum(xMax);
    ui->spinX->setSingleStep((xMax - xMin)/10);

    QCPItemLine* lineV = new QCPItemLine(ui->plotFxy);
//    ui->plotFxy->addItem(tickVLine);
    lineV->start->setCoords(0, yMin - 1);
    lineV->end->setCoords(0, yMax + 1);
    lineV->setPen(pen);

    ui->spinY->setMinimum(yMin);
    ui->spinY->setMaximum(yMax);
    ui->spinY->setSingleStep((yMax - yMin)/10);

    QCPItemLine* lineH = new QCPItemLine(ui->plotFxy);
//    ui->plotFxy->addItem(tickHLine);
    lineH->start->setCoords(xMin - 1,  0);
    lineH->end->setCoords(xMax + 1, 0);
    lineH->setPen(pen);
}

/*
 * Updates the sector plots
 */
void FluxAnalysisDialog::UpdateSectorPlots(const Matrix2D<int>& bins, double wPhoton, int xDivs, int yDivs, double xMin, double yMin, double xMax, double yMax, double fluxMax)
{
    double x = ui->spinX->value();
    double y = ui->spinY->value();

    QCPItemLine* lineV = (QCPItemLine*) ui->plotFxy->item(0);
    lineV->start->setCoords(x, lineV->start->coords().y());
    lineV->end->setCoords(x, lineV->end->coords().y());

    QCPItemLine* lineH = (QCPItemLine*) ui->plotFxy->item(1);
    lineH->start->setCoords(lineH->start->coords().x(), y);
    lineH->end->setCoords(lineH->end->coords().x(), y);

    ui->plotFxy->replot();
    ui->plotFx->clearPlottables();
    ui->plotFy->clearPlottables();


    double xSize = (xMax - xMin)/xDivs;
    double ySize = (yMax - yMin)/yDivs;
    double areaCell = xSize*ySize;

    int xBin = floor( (x - xMin) / (xMax - xMin) * xDivs);
    if (xBin >= xDivs) xBin = xDivs - 1;
    int yBin = floor( (y - yMin) / (yMax - yMin) * yDivs);
    if (yBin >= yDivs) yBin = yDivs - 1;


    QVector<double> dataY(yDivs), dataFy(yDivs);
    for (int ny = 0; ny < yDivs; ++ny)
	{
        dataY[ny] = yMin + (ny + 0.5)*ySize;
        dataFy[ny] = bins(xBin, ny) * wPhoton / areaCell;
	}

    QVector<double> dataX(xDivs), dataFx(xDivs);
    for (int nx = 0; nx < xDivs; ++nx)
	{
        dataX[nx] = xMin + (nx + 0.5)*xSize;
        dataFx[nx] = bins(nx, yBin) * wPhoton / areaCell;
	}

    ui->plotFy->addGraph();
    ui->plotFy->graph(0)->setData(dataY, dataFy);
    ui->plotFy->xAxis->setRange(yMin, yMax);
    ui->plotFy->yAxis->setRange(0, 1.2*fluxMax);
    ui->plotFy->replot();

    ui->plotFx->addGraph();
    ui->plotFx->graph(0)->setData(dataX, dataFx);
    ui->plotFx->xAxis->setRange(xMin, xMax);
    ui->plotFx->yAxis->setRange(0, 1.2*fluxMax);
    ui->plotFx->replot();
}
