#include "SelectSurfaceDialog.h"
#include "ui_SelectSurfaceDialog.h"

#include <QItemSelectionModel>
#include <QMessageBox>

#include "kernel/run/InstanceNode.h"
#include "kernel/scene/TShapeKit.h"
#include "tree/SceneFilter.h"
#include "tree/SceneModel.h"
#include "tree/SceneView.h"

/*!
 * Creates a dialog object to select a surface from the \a currentSceneModel.
 */
SelectSurfaceDialog::SelectSurfaceDialog(SceneModel& model, bool enableLight, QWidget* parent):
    QDialog(parent),
    ui(new Ui::SelectSurfaceDialog),
    m_isLightVisible(enableLight),
    m_model(&model)
{
    ui->setupUi(this);

    if (!enableLight)
    {
        ui->lightRadio->setVisible(false);
        ui->sceneRadio->setVisible(false);
    }

    m_filter = new SceneFilter(this);
    m_filter->setSourceModel(m_model);
    ui->sceneModelView->setModel(m_filter);

    QModelIndex viewLayoutIndex = model.IndexFromUrl("//");
    ui->sceneModelView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->sceneModelView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->sceneModelView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->sceneModelView->setRootIsDecorated(true);
    ui->sceneModelView->setRootIndex(viewLayoutIndex);
    ui->sceneModelView->expandToDepth(1);

    setWindowFlag(Qt::WindowContextHelpButtonHint, false);

    int q = fontMetrics().height();
    resize(36*q, 24*q);
}

/*!
 * Destroys dialog object.
 */
SelectSurfaceDialog::~SelectSurfaceDialog()
{
    delete ui;
    delete m_filter;
}

void SelectSurfaceDialog::SetShapeTypeFilters(QStringList filters)
{
    m_filter->SetShapeTypeFilters(filters);
    m_filter->invalidate();
}

/*!
 * Checks if the selected node is valid node before close dialog.
 */
void SelectSurfaceDialog::accept()
{
    QItemSelectionModel* selectionModel = ui->sceneModelView->selectionModel();
    if (ui->sceneRadio->isChecked() && selectionModel->hasSelection() )
    {
        QModelIndex selectedIndex = selectionModel->currentIndex();
        QModelIndex currentIndex = m_filter->mapToSource(selectedIndex);

        InstanceNode* instance = m_model->getInstance(currentIndex);
        if (!instance->getNode()->getTypeId().isDerivedFrom(TShapeKit::getClassTypeId() ) )
        {
            QMessageBox::information(this, "Tonatiuh", tr("Selected node is not surface node"), 1);
            return;
        }
    }

    QDialog::accept();
}

/*!
 * Returns selected surface url.
 * If the selected node is not a surface node return an empty url.
 */
QString SelectSurfaceDialog::GetSelectedSurfaceURL() const
{
    if (m_isLightVisible && ui->lightRadio->isChecked())
        return "//Light";

    QItemSelectionModel* selectionModel = ui->sceneModelView->selectionModel();

    QModelIndex selectedIndex = selectionModel->currentIndex();
    QModelIndex currentIndex = m_filter->mapToSource(selectedIndex);
    if (!currentIndex.isValid()) return QString();

    InstanceNode* instance = m_model->getInstance(currentIndex);
    if (!instance) return QString();
    return instance->getURL();
}
