#include "SunDialog.h"
#include "ui_sundialog.h"

#include <QItemSelectionModel>
#include <QMessageBox>

#include "tree/SceneModel.h"
#include "kernel/run/InstanceNode.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/ShapeAbstract.h"
#include "kernel/shape/ShapeFactory.h"
#include "kernel/sun/SunFactory.h"
#include "kernel/sun/TLightKit.h"
#include "kernel/scene/TSceneKit.h"
#include "libraries/geometry/gcf.h"
#include "parameters/ParametersView.h"

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
    ui(new Ui::SunDialog),
    m_sceneModel(&sceneModel),
    m_selectionModel(0),
    m_lightKitOld(lightKitOld),
    m_sunNew(0),
    m_sunShapeMap(sunShapeMap),
    m_currentSunShapeIndex(-1)
{
    ui->setupUi(this);

    if (lightKitOld) {
        SoNode* node = lightKitOld->getPart("tsunshape", false);
        if (node)
            m_sunNew = static_cast<SunAbstract*>(node->copy(true));
    }

    makeSunPositionTab();
    makeSunShapeTab();
    makeSunApertureTab();

    Qt::WindowFlags flags = windowFlags();
    flags ^= Qt::WindowContextHelpButtonHint;
    setWindowFlags(flags);
}

SunDialog::~SunDialog()
{
    delete ui;
}

/*!
 * Returns a lightkit with the parameters defined in the dialog.
 */
TLightKit* SunDialog::getLightKit()
{
    TLightKit* lightKit = new TLightKit;


    if (m_sunNew) lightKit->setPart("tsunshape", m_sunNew);

//    lightKit->ChangePosition(
//        ui->azimuthSpin->value()*gc::Degree,
//        (90. - ui->elevationSpin->value())*gc::Degree
//    );

    QString nodes("");
    for (int n = 0; n < ui->disabledNodeList->count(); n++) {
        QString node = ui->disabledNodeList->item(n)->text();
        nodes += QString("%1;").arg(node);
    }
    lightKit->disabledNodes.setValue( nodes.toStdString().c_str() );

    return lightKit;
}

double SunDialog::getAzimuth()
{
    return ui->azimuthSpin->value()*gcf::degree;
}

double SunDialog::getElevation()
{
    return ui->elevationSpin->value()*gcf::degree;
}

/*!
 * Updates the dialog values to the values of the current light.
 */
void SunDialog::makeSunShapeTab()
{
    connect(
        ui->sunshapeParameters, SIGNAL(valueModified(SoNode*, QString, QString)),
        this, SLOT(SetValue(SoNode*, QString, QString))
    );
    connect(
        ui->sunshapeCombo, SIGNAL(activated(int)),
        this, SLOT(ChangeSunshape(int))
    );

    QList<SunFactory*> sunFactories = m_sunShapeMap.values();
//    ui->sunshapeCombo->addItem("---");
    for (SunFactory* f : sunFactories)
        ui->sunshapeCombo->addItem(f->icon(), f->name());

    m_currentSunShapeIndex = 0;
    if (m_sunNew) {
        QString name(m_sunNew->getTypeName());
        m_currentSunShapeIndex = ui->sunshapeCombo->findText(name);
    }

    ui->sunshapeCombo->setCurrentIndex(m_currentSunShapeIndex);
    ChangeSunshape(m_currentSunShapeIndex);
}

/*!
 * Updates the sun position tab values to the values of the current light.
 */
void SunDialog::makeSunPositionTab()
{
    if (!m_lightKitOld) return;

    TSceneKit* scene = m_sceneModel->getSceneKit();
    ui->azimuthSpin->setValue(scene->azimuth.getValue()/gcf::degree);
    ui->elevationSpin->setValue(scene->elevation.getValue()/gcf::degree);
}

/*!
 * Initializes the sun size tab models and lists.
 */
void SunDialog::makeSunApertureTab()
{
    ui->modelTreeView->setModel(m_sceneModel);
    m_selectionModel = new QItemSelectionModel(m_sceneModel);
    ui->modelTreeView->setSelectionModel(m_selectionModel);
    ui->modelTreeView->setRootIndex(m_sceneModel->IndexFromNodeUrl("//SunNode"));

    if (!m_lightKitOld) return;

    QString nodeDisabled = QString(m_lightKitOld->disabledNodes.getValue().getString());
    for (QString s : nodeDisabled.split(";", QString::SkipEmptyParts))
        ui->disabledNodeList->addItem(s);

    connect(
        ui->addNodeButton, SIGNAL(clicked()),
        this, SLOT(AddNodeToDisabledNodeList())
    );
    connect(
        ui->removeNodeButton, SIGNAL(clicked()),
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
        SunFactory* f = m_sunShapeMap[ui->sunshapeCombo->itemText(index)];
        m_sunNew = f->create();
    }
    ui->sunshapeParameters->SetContainer(m_sunNew);
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
    if ( m_selectionModel->currentIndex() == ui->modelTreeView->rootIndex() ) return;

    QModelIndex index = m_selectionModel->currentIndex();
    if (!index.isValid()) return;

    InstanceNode* node = m_sceneModel->getInstance(index);
    if (!node) return;

    ui->disabledNodeList->addItem(node->GetNodeURL());
}

/*!
 * Removes the selected node in node list from disabled nodes.
 */
void SunDialog::RemoveNodeFromDisabledNodeList()
{
    QListWidgetItem* currentItem = ui->disabledNodeList->currentItem();
    if (!currentItem) return;

    ui->disabledNodeList->removeItemWidget(currentItem);
    delete currentItem;
}
