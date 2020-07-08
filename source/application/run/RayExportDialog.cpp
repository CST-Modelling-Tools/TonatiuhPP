#include "RayExportDialog.h"
#include "ui_RayExportDialog.h"

#include <iostream>
#include <QMessageBox>

#include "kernel/photons/PhotonsWidget.h"
#include "kernel/photons/PhotonsSettings.h"
#include "SelectSurfaceDialog.h"

/*!
 * Creates a new dialog object to define the export settings. The available export mode types are listed into \a typeList.
 */
RayExportDialog::RayExportDialog(SceneModel& scene, QVector<PhotonsFactory*> factories, QWidget* parent):
    QDialog(parent),
    ui(new Ui::RayExportDialog),
    m_scene(&scene)
{
    ui->setupUi(this);

    for (PhotonsFactory* f : factories)
    {
        ui->storeTypeCombo->addItem(f->icon(), f->name());
        PhotonsWidget* widget = f->createWidget();
        m_widgets << widget;
        if (!widget) widget = new PhotonsWidget;
        ui->parametersWidget->addWidget(widget);
    }
    storageChanged();

    connect(ui->storeTypeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(storageChanged()) );
    connect(ui->surfaceAddButton, SIGNAL(clicked()), this, SLOT(surfaceAdd()) );
    connect(ui->surfaceDeleteButton, SIGNAL(clicked()), this, SLOT(surfaceDelete()) );
}

RayExportDialog::~RayExportDialog()
{
    delete ui;
}

/*!
 *    Returns defined settings into settings class object.
 */
PhotonsSettings RayExportDialog::GetExportPhotonMapSettings() const
{
    PhotonsSettings settings;
    settings.name = ui->storeTypeCombo->currentText();

    settings.saveCoordinates = ui->saveCoordinatesCheck->isEnabled() && ui->saveCoordinatesCheck->isChecked();
    settings.saveCoordinatesGlobal = ui->saveGlobalRadio->isEnabled() && ui->saveGlobalRadio->isChecked();
    settings.saveSurfaceID = ui->saveSurfaceCheck->isEnabled() && ui->saveSurfaceCheck->isChecked();
    settings.saveSurfaceSide = ui->saveSideCheck->isEnabled() && ui->saveSideCheck->isChecked();
    settings.savePhotonsID = ui->savePhotonsCheck->isEnabled() && ui->savePhotonsCheck->isChecked();

    if (ui->surfaceAllRadio->isEnabled() && ui->surfaceAllRadio->isChecked())
        settings.surfaces.clear();
    else
        settings.surfaces = m_surfaces;

    PhotonsWidget* widget = m_widgets[ui->storeTypeCombo->currentIndex()];
    if (widget) {
        for (QString name : widget->getParameterNames())
            settings.parameters.insert(name, widget->getParameterValue(name));
    }

    return settings;
}

/*!
 * Changes the widget of the store type settings to current selected store widget.
 */
void RayExportDialog::storageChanged()
{
    ui->parametersWidget->setCurrentIndex(ui->storeTypeCombo->currentIndex());
}

/*!
 * Adds a surface to export surface list.
 */
void RayExportDialog::surfaceAdd()
{
    SelectSurfaceDialog selectSurfaceDialog(*m_scene, true);
    if (!selectSurfaceDialog.exec()) return;

    QString surface = selectSurfaceDialog.GetSelectedSurfaceURL();
    if (surface.isEmpty()) return;

    if (m_surfaces.contains(surface) ) {
        QMessageBox::information(this, "Tonatiuh", "Selected node has already been added", 1);
        return;
    }

    m_surfaces << surface;
    ui->surfacesListWidget->addItem(surface);
}

/*!
 * Deletes current selected surface from export surface list.
 */
void RayExportDialog::surfaceDelete()
{
    if (!ui->surfacesListWidget->currentItem()) return;
    int n = m_surfaces.indexOf(ui->surfacesListWidget->currentItem()->text());
    if (n < 0) return;
    m_surfaces.removeAt(n);
    delete ui->surfacesListWidget->item(n);
}
