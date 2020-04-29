#include "SunDialog.h"

#include <QItemSelectionModel>
#include <QMessageBox>

#include "gui/SceneModel.h"
#include "kernel/run/InstanceNode.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/ShapeAbstract.h"
#include "kernel/shape/ShapeFactory.h"
#include "kernel/sun/SunFactory.h"
#include "kernel/sun/TLightKit.h"
#include "libraries/geometry/gc.h"
#include "parameters/FieldContainerWidget.h"

/**
 * Creates a new dialog for the light definition.
 *
 * The dialog present a list given by \a sunshapeFactoryList to define light sunshape, \a shapeFactoryList for the light input
 * aperture and shows the the light parameters defined in the light \a currentLightKit.
 */

SunDialog::SunDialog(
    SceneModel& sceneModel,
    TLightKit* lightKitOld,
    QMap<QString, SunFactory*> sunShapeMap,
    QWidget* parent
):
    QDialog(parent),
    m_sceneModel(&sceneModel),
    m_selectionModel(0),
    m_lightKitOld(lightKitOld),
    m_sunNew(0),
    m_sunShapeMap(sunShapeMap),
    m_currentSunShapeIndex(-1)
{
    setupUi(this);

    if (lightKitOld) {
        SoNode* node = lightKitOld->getPart("tsunshape", false);
        if (node)
            m_sunNew = static_cast<SunAbstract*>(node->copy(true));
    }

    makeSunShapeTab();
    makeSunApertureTab();
    makeSunPositionTab();
}

/*!
 * Destroys the LightDialog object.
 */
SunDialog::~SunDialog()
{

}

/*!
 * Returns a lightkit with the parameters defined in the dialog.
 */
TLightKit* SunDialog::getLightKit()
{
    TLightKit* lightKit = new TLightKit;
    lightKit->setName("Light");

    if (m_sunNew) lightKit->setPart("tsunshape", m_sunNew);

    lightKit->ChangePosition(
        azimuthSpin->value()*gc::Degree,
        (90 - elevationSpin->value())*gc::Degree
    );

    QString nodes("");
    for (int n = 0; n < disabledNodeList->count(); n++) {
        QString node = disabledNodeList->item(n)->text();
        nodes += QString("%1;").arg(node);
    }
    lightKit->disabledNodes.setValue( nodes.toStdString().c_str() );

    return lightKit;
}

/*!
 * Updates the dialog values to the values of the current light.
 */
void SunDialog::makeSunShapeTab()
{
    connect(
        sunshapeParameters, SIGNAL(valueModificated(SoNode*, QString, QString)),
        this, SLOT(SetValue(SoNode*, QString, QString))
    );
    connect(
        sunshapeCombo, SIGNAL(activated(int)),
        this, SLOT(ChangeSunshape(int))
    );

    QList<SunFactory*> sunFactories = m_sunShapeMap.values();
//    sunshapeCombo->addItem("---");
    for (SunFactory* f : sunFactories)
        sunshapeCombo->addItem(f->icon(), f->name());

    m_currentSunShapeIndex = 0;
    if (m_sunNew) {
        QString name(m_sunNew->getTypeName());
        m_currentSunShapeIndex = sunshapeCombo->findText(name);
    }

    sunshapeCombo->setCurrentIndex(m_currentSunShapeIndex);
    ChangeSunshape(m_currentSunShapeIndex);
}

/*!
 * Updates the sun position tab values to the values of the current light.
 */
void SunDialog::makeSunPositionTab()
{
    if (!m_lightKitOld) return;
    azimuthSpin->setValue(m_lightKitOld->azimuth.getValue()/gc::Degree);
    elevationSpin->setValue(90. - m_lightKitOld->zenith.getValue()/gc::Degree);
}

/*!
 * Initializes the sun size tab models and lists.
 */
void SunDialog::makeSunApertureTab()
{
    modelTreeView->setModel(m_sceneModel);
    m_selectionModel = new QItemSelectionModel(m_sceneModel);
    modelTreeView->setSelectionModel(m_selectionModel);
    modelTreeView->setRootIndex(m_sceneModel->IndexFromNodeUrl("//SunNode"));

    if (!m_lightKitOld) return;

    QString nodeDisabled = QString(m_lightKitOld->disabledNodes.getValue().getString());
    for (QString s : nodeDisabled.split(";", QString::SkipEmptyParts))
        disabledNodeList->addItem(s);

    connect(
        addNodeButton, SIGNAL(clicked()),
        this, SLOT(AddNodeToDisabledNodeList())
    );
    connect(
        removeNodeButton, SIGNAL(clicked()),
        this, SLOT(RemoveNodeFromDisabledNodeList())
    );
}

/*!
 * Changes parameters of the shunshape paraneters view to sunshape type given by \a index.
 */
void SunDialog::ChangeSunshape(int index)
{
    while (m_sunNew && m_sunNew->getRefCount() > 0)
        m_sunNew->unref();

    if (index == m_currentSunShapeIndex)
        m_sunNew = static_cast<SunAbstract*>(m_lightKitOld->getPart("tsunshape", false)->copy(true));
    else {
        SunFactory* f = m_sunShapeMap[sunshapeCombo->itemText(index)];
        m_sunNew = f->create();
    }
    sunshapeParameters->SetContainer(m_sunNew, QString());
}

void SunDialog::SetValue(SoNode* node, QString parameter, QString value)
{
    if (parameter == "irradiance")
        if (value.toDouble() < 0.) return;

    SoField* field = node->getField(SbName(parameter.toStdString().c_str()));
    if (field)
        field->set(value.toStdString().c_str());
}

/*!
 * Adds in the tree view selected node to disabled node list.
 */
void SunDialog::AddNodeToDisabledNodeList()
{
    if ( !m_selectionModel->hasSelection() ) return;
    if ( m_selectionModel->currentIndex() == modelTreeView->rootIndex() ) return;

    QModelIndex index = m_selectionModel->currentIndex();
    if (!index.isValid()) return;

    InstanceNode* node = m_sceneModel->NodeFromIndex(index);
    if (!node) return;

    disabledNodeList->addItem(node->GetNodeURL());
}

/*!
 * Removes the selected node in node list from disabled nodes.
 */
void SunDialog::RemoveNodeFromDisabledNodeList()
{
    QListWidgetItem* currentItem = disabledNodeList->currentItem();
    if (!currentItem) return;

    disabledNodeList->removeItemWidget(currentItem);
    delete currentItem;
}
