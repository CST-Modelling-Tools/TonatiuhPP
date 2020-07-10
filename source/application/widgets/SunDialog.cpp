#include "SunDialog.h"
#include "ui_SunDialog.h"

#include <QItemSelectionModel>
#include <QMessageBox>

#include "tree/SceneModel.h"
#include "kernel/run/InstanceNode.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/ShapeRT.h"
#include "kernel/sun/SunShape.h"
#include "kernel/sun/SunAperture.h"
#include "kernel/sun/SunKit.h"
#include "kernel/scene/TSceneKit.h"
#include "libraries/math/gcf.h"
#include "parameters/ParametersView.h"

/**
 * Creates a new dialog for the light definition.
 *
 * The dialog present a list given by \a sunshapeFactoryList to define light sunshape, \a shapeFactoryList for the light input
 * aperture and shows the the light parameters defined in the light \a currentLightKit.
 */

SunDialog::SunDialog(SceneModel* sceneModel,
    SunKit* sunKit,
    QMap<QString, SunFactory*> sunShapeMap,
    QWidget* parent
):
    QDialog(parent),
    ui(new Ui::SunDialog),
    m_sceneModel(sceneModel),
    m_selectionModel(0),
    m_sunKit(sunKit),
    m_sunShape(0),
    m_sunShapeMap(sunShapeMap),
    m_currentSunShapeIndex(-1)
{
    ui->setupUi(this);

//    if (sunKit) {
//        SoNode* node = sunKit->getPart("tsunshape", false);
//        if (node)
//            m_sunShape = static_cast<SunShape*>(node->copy(true));
//    }

    makeSunPositionTab();
    makeSunShapeTab();
    makeSunApertureTab();

    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
}

SunDialog::~SunDialog()
{
    delete ui;
}

/*!
 * Returns a lightkit with the parameters defined in the dialog.
 */
SunKit* SunDialog::getSunKit()
{
    SunKit* ans = new SunKit;

    ans->azimuth = ui->spinAzimuth->value()*gcf::degree;
    ans->elevation = ui->spinElevation->value()*gcf::degree;
    ans->irradiance.setValue(ui->spinIrradiance->value());

    if (m_sunShape) ans->setPart("tsunshape", m_sunShape);

    QString nodes;
    for (int n = 0; n < ui->disabledNodeList->count(); ++n)
        nodes += ui->disabledNodeList->item(n)->text() + ";";

    SunAperture* aperture = (SunAperture*) ans->getPart("icon", false);
    aperture->disabledNodes = nodes.toStdString().c_str();

    ans->updateTransform();
    return ans;
}

/*!
 * Updates the sun position tab values to the values of the current light.
 */
void SunDialog::makeSunPositionTab()
{
    if (!m_sunKit) return;
    ui->spinAzimuth->setValue(m_sunKit->azimuth.getValue()/gcf::degree);
    ui->spinElevation->setValue(m_sunKit->elevation.getValue()/gcf::degree);
    ui->spinIrradiance->setValue(m_sunKit->irradiance.getValue());
}

/*!
 * Updates the dialog values to the values of the current light.
 */
void SunDialog::makeSunShapeTab()
{
    QList<SunFactory*> sunFactories = m_sunShapeMap.values();
    for (SunFactory* f : sunFactories)
        ui->sunshapeCombo->addItem(f->icon(), f->name());

    m_currentSunShapeIndex = 0;
    if (m_sunShape) {
        QString name(m_sunShape->getTypeName());
        m_currentSunShapeIndex = ui->sunshapeCombo->findText(name);
    }

    ui->sunshapeCombo->setCurrentIndex(m_currentSunShapeIndex);
    ChangeSunshape(m_currentSunShapeIndex);

    connect(
        ui->sunshapeCombo, SIGNAL(activated(int)),
        this, SLOT(ChangeSunshape(int))
    );
    connect(
        ui->sunshapeParameters, SIGNAL(valueModified(SoNode*, QString, QString)),
        this, SLOT(SetValue(SoNode*, QString, QString))
    );
}

/*!
 * Initializes the sun size tab models and lists.
 */
void SunDialog::makeSunApertureTab()
{
    ui->modelTreeView->setModel(m_sceneModel);
    m_selectionModel = new QItemSelectionModel(m_sceneModel);
    ui->modelTreeView->setSelectionModel(m_selectionModel);
    ui->modelTreeView->setRootIndex(QModelIndex());

    if (!m_sunKit) return;
    SunAperture* aperture = (SunAperture*) m_sunKit->getPart("icon", false);
    QString nodes = aperture->disabledNodes.getValue().getString();
    for (QString s : nodes.split(";", QString::SkipEmptyParts))
        ui->disabledNodeList->addItem(s);

    connect(
        ui->addNodeButton, SIGNAL(clicked()),
        this, SLOT(addNode())
    );
    connect(
        ui->removeNodeButton, SIGNAL(clicked()),
        this, SLOT(removeNode())
    );
}

/*!
 * Changes parameters of the shunshape paraneters view to sunshape type given by \a index.
 */
void SunDialog::ChangeSunshape(int index)
{
    while (m_sunShape && m_sunShape->getRefCount() > 0)
        m_sunShape->unref();

    if (index == m_currentSunShapeIndex)
        m_sunShape = static_cast<SunShape*>(m_sunKit->getPart("tsunshape", false)->copy(true));
    else {
        SunFactory* f = m_sunShapeMap[ui->sunshapeCombo->itemText(index)];
        m_sunShape = f->create();
    }
    ui->sunshapeParameters->SetContainer(m_sunShape);
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
void SunDialog::addNode()
{
    if (!m_selectionModel->hasSelection()) return;
    if (m_selectionModel->currentIndex() == ui->modelTreeView->rootIndex()) return;

    QModelIndex index = m_selectionModel->currentIndex();
    if (!index.isValid()) return;

    InstanceNode* node = m_sceneModel->getInstance(index);
    if (!node) return;

    ui->disabledNodeList->addItem(node->getURL());
}

/*!
 * Removes the selected node in node list from disabled nodes.
 */
void SunDialog::removeNode()
{
    QListWidgetItem* currentItem = ui->disabledNodeList->currentItem();
    if (!currentItem) return;

    ui->disabledNodeList->removeItemWidget(currentItem);
    delete currentItem;
}
