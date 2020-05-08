#include <QItemSelectionModel>
#include <QMessageBox>

#include "SelectSurfaceDialog.h"
#include "NodesFilterModel.h"

#include "kernel/run/InstanceNode.h"
#include "kernel/scene/TShapeKit.h"
#include "tree/SceneModel.h"
#include "tree/SceneView.h"

/*!
 * Creates a dialog object to select a surface from the \a currentSceneModel.
 */
SelectSurfaceDialog::SelectSurfaceDialog(SceneModel& currentSceneModel, bool enableLight, QWidget* parent)
    : QDialog(parent),
    m_isLightVisible(enableLight),
    m_pCurrentSceneModel(&currentSceneModel),
    m_pNodeFilterModel(0)
{
    setupUi(this);

    if (!enableLight)
    {
        lightRadio->setVisible(false);
        sceneRadio->setVisible(false);
    }

    m_pNodeFilterModel = new NodesFilterModel(this);
    m_pNodeFilterModel->setSourceModel(m_pCurrentSceneModel);
    sceneModelView->setModel(m_pNodeFilterModel);

    QModelIndex viewRootNodeIndex = currentSceneModel.IndexFromNodeUrl("//SunNode");
    sceneModelView->setSelectionBehavior(QAbstractItemView::SelectRows);
    sceneModelView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    sceneModelView->setSelectionMode(QAbstractItemView::SingleSelection);
    sceneModelView->setRootIsDecorated(true);
    sceneModelView->setRootIndex(viewRootNodeIndex);
}

/*!
 * Destroys dialog object.
 */
SelectSurfaceDialog::~SelectSurfaceDialog()
{
    delete m_pNodeFilterModel;
}


void SelectSurfaceDialog::SetShapeTypeFilters(QVector<QString>  shapeTypeFilters)
{
    m_pNodeFilterModel->SetShapeTypeFilters(shapeTypeFilters);
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
        QModelIndex currentIndex = m_pNodeFilterModel->mapToSource(selectedIndex);

        InstanceNode* selectedNode = m_pCurrentSceneModel->NodeFromIndex(currentIndex);
        if (!selectedNode->GetNode()->getTypeId().isDerivedFrom(TShapeKit::getClassTypeId() ) )
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
    if (m_isLightVisible  && lightRadio->isChecked() )
        return "//Light";

    QItemSelectionModel* selectionModel = sceneModelView->selectionModel();

    QModelIndex selectedIndex = selectionModel->currentIndex();
    QModelIndex currentIndex = m_pNodeFilterModel->mapToSource(selectedIndex);
    if (!currentIndex.isValid() ) return QString();

    InstanceNode* currentNode = m_pCurrentSceneModel->NodeFromIndex(currentIndex);
    if (!currentNode) return QString();

    return (currentNode->GetNodeURL() );
}
