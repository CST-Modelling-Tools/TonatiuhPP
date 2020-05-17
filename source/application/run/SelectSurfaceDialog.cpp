#include <QItemSelectionModel>
#include <QMessageBox>

#include "SelectSurfaceDialog.h"
#include "tree/SceneFilter.h"

#include "kernel/run/InstanceNode.h"
#include "kernel/scene/TShapeKit.h"
#include "tree/SceneModel.h"
#include "tree/SceneView.h"

/*!
 * Creates a dialog object to select a surface from the \a currentSceneModel.
 */
SelectSurfaceDialog::SelectSurfaceDialog(SceneModel& model, bool enableLight, QWidget* parent):
    QDialog(parent),
    m_isLightVisible(enableLight),
    m_model(&model)
{
    setupUi(this);

    if (!enableLight)
    {
        lightRadio->setVisible(false);
        sceneRadio->setVisible(false);
    }

    m_filter = new SceneFilter(this);
    m_filter->setSourceModel(m_model);
    sceneModelView->setModel(m_filter);

    QModelIndex viewLayoutIndex = model.IndexFromNodeUrl("//SunNode");
    sceneModelView->setSelectionBehavior(QAbstractItemView::SelectRows);
    sceneModelView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    sceneModelView->setSelectionMode(QAbstractItemView::SingleSelection);
    sceneModelView->setRootIsDecorated(true);
    sceneModelView->setRootIndex(viewLayoutIndex);
}

/*!
 * Destroys dialog object.
 */
SelectSurfaceDialog::~SelectSurfaceDialog()
{
    delete m_filter;
}


void SelectSurfaceDialog::SetShapeTypeFilters(QVector<QString> filters)
{
    m_filter->SetShapeTypeFilters(filters);
}

/*!
 * Checks if the selected node is valid node before close dialog.
 */
void SelectSurfaceDialog::accept()
{
    QItemSelectionModel* selectionModel = sceneModelView->selectionModel();
    if (sceneRadio->isChecked() && selectionModel->hasSelection() )
    {
        QModelIndex selectedIndex = selectionModel->currentIndex();
        QModelIndex currentIndex = m_filter->mapToSource(selectedIndex);

        InstanceNode* selectedNode = m_model->NodeFromIndex(currentIndex);
        if (!selectedNode->getNode()->getTypeId().isDerivedFrom(TShapeKit::getClassTypeId() ) )
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
    if (m_isLightVisible && lightRadio->isChecked())
        return "//Light";

    QItemSelectionModel* selectionModel = sceneModelView->selectionModel();

    QModelIndex selectedIndex = selectionModel->currentIndex();
    QModelIndex currentIndex = m_filter->mapToSource(selectedIndex);
    if (!currentIndex.isValid()) return QString();

    InstanceNode* currentNode = m_model->NodeFromIndex(currentIndex);
    if (!currentNode) return QString();

    return currentNode->GetNodeURL();
}
