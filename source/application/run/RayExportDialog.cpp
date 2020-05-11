#include "RayExportDialog.h"

#include <iostream>
#include <QMessageBox>

#include "kernel/photons/PhotonsWidget.h"
#include "kernel/photons/PhotonsSettings.h"
#include "SelectSurfaceDialog.h"

/*!
 * Creates a new dialog object to define the export settings. The available export mode types are listed into \a typeList.
 */
RayExportDialog::RayExportDialog(
    SceneModel& scene,
    QVector<PhotonsFactory*> typeList,
    QWidget* parent):
    QDialog(parent),
    m_scene(&scene)
{
    setupUi(this);

    for (int index = 0; index < typeList.size(); index++)
    {
        storeTypeCombo->addItem(typeList[index]->icon(), typeList[index]->name() );
        PhotonsWidget* widget = typeList[index]->createWidget();
        if (widget)
        {
            m_parameters << widget;
            parametersWidget->addWidget(widget);
        }
        else
        {
            m_parameters << 0;
            parametersWidget->addWidget(new QWidget);
        }
    }
    ChangeCurrentStoreTypeParameters();

    connect(storeTypeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeCurrentStoreTypeParameters()) );
    connect(addSurfaceButton, SIGNAL(clicked()), this, SLOT(AddSurface()) );
    connect(deleteSurfaceButton, SIGNAL(clicked()), this, SLOT(DeleteSurface()) );
}

/*!
 *    Returns defined settings into settings class object.
 */
PhotonsSettings RayExportDialog::GetExportPhotonMapSettings() const
{
    PhotonsSettings settings;
    settings.modeTypeName = storeTypeCombo->currentText();
    //settings.exportAllPhotonMap = ( exportAllPhotonsRadio->isEnabled() && exportAllPhotonsRadio->isChecked() );
    settings.exportCoordinates = (coordCheck->isEnabled() && coordCheck->isChecked() );
    settings.exportIntersectionSurfaceSide = (sideBox->isEnabled() && sideBox->isChecked() );
    settings.exportInGlobalCoordinates = (globalCoordinatesRadio->isEnabled() && globalCoordinatesRadio->isChecked() );
    settings.exportPreviousNextPhotonID = (nextPreviousCheck->isEnabled() && nextPreviousCheck->isChecked() );
    settings.exportSurfaceID = (surfaceIdenfierCheck->isEnabled() && surfaceIdenfierCheck->isChecked() );
    if (exportAllSurfacesRadio->isEnabled() && exportAllSurfacesRadio->isChecked() )
        settings.exportSurfaceNodeList.clear();
    else
        settings.exportSurfaceNodeList = m_surfaces;

    PhotonsWidget* exportTypeWidget = m_parameters[storeTypeCombo->currentIndex()];
    if (!exportTypeWidget) return settings;

    QStringList exportTypeParametersName = exportTypeWidget->GetParameterNames();
    for (int p = 0; p < exportTypeParametersName.size(); p++) {
        QString parameterName = exportTypeParametersName[p];
        settings.AddParameter(parameterName, exportTypeWidget->GetParameterValue(parameterName) );
    }
    return settings;
}

/*!
 * Changes the widget of the store type settings to current selected store widget.
 */
void RayExportDialog::ChangeCurrentStoreTypeParameters()
{
    parametersWidget->setCurrentIndex(storeTypeCombo->currentIndex() );
}

/*!
 * Adds a surface to export surface list.
 */
void RayExportDialog::AddSurface()
{
    SelectSurfaceDialog selectSurfaceDialog(*m_scene, true);
    if (!selectSurfaceDialog.exec() ) return;

    QString surface = selectSurfaceDialog.GetSelectedSurfaceURL();
    if (surface.isEmpty() ) return;

    if (m_surfaces.contains(surface) ) {
        QMessageBox::information(this, "Tonatiuh", tr("Selected node has already been added."), 1);
        return;
    }

    m_surfaces << surface;
    surfacesListWidget->addItem(surface);
}

/*!
 * Deletes current selected surface from export surface list.
 */
void RayExportDialog::DeleteSurface()
{
    if (!surfacesListWidget->currentItem()) return;
    int n = m_surfaces.indexOf(surfacesListWidget->currentItem()->text());
    if (n < 0) return;
    m_surfaces.removeAt(n);
    delete surfacesListWidget->item(n);
}
