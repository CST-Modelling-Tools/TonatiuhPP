#include "SunDialog.h"
#include "ui_SunDialog.h"

#include "tree/SceneModel.h"
#include "kernel/run/InstanceNode.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/ShapeRT.h"
#include "kernel/sun/SunShape.h"
#include "kernel/sun/SunPosition.h"
#include "kernel/sun/SunAperture.h"
#include "kernel/sun/SunKitW.h"
#include "kernel/scene/TSceneKit.h"
#include "calculator/SunCalculatorDialog.h"


SunDialog::SunDialog(SceneModel* sceneModel,
    QMap<QString, SunFactory*> sunShapeMap,
    QWidget* parent
):
    QDialog(parent),
    ui(new Ui::SunDialog),
    m_sceneModel(sceneModel),
    m_sunShapeMap(sunShapeMap)
{
    ui->setupUi(this);

    TSceneKit* sceneKit = sceneModel->getSceneKit();
    SunKitW* sunOld = (SunKitW*) sceneKit->getPart("world.sun", false);
    m_sun = (SunKitW*) sunOld->copy();
    m_sun->ref();

    makeSunPositionTab();
    makeSunShapeTab();
    makeSunApertureTab();

    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    connect(this, SIGNAL(accepted()), this, SLOT(onAccept()));
}

SunDialog::~SunDialog()
{
    delete ui;
    m_sun->unref();
}

void SunDialog::onAccept()
{
    SunPosition* sp = (SunPosition*) m_sun->getPart("position", false);
    sp->azimuth = ui->spinAzimuth->value();
    sp->elevation = ui->spinElevation->value();
    sp->irradiance = ui->spinIrradiance->value();

    SunAperture* aperture = (SunAperture*) m_sun->getPart("aperture", false);
    QStringList nodes;
    for (int n = 0; n < ui->disabledNodes->count(); ++n)
        nodes << ui->disabledNodes->item(n)->text();
    aperture->disabledNodes = nodes.join(';').toStdString().c_str();

    m_sun->updateTransform();
}

void SunDialog::makeSunPositionTab()
{
    SunPosition* sp = (SunPosition*) m_sun->getPart("position", false);

    ui->spinAzimuth->setValue(sp->azimuth.getValue());
    ui->spinElevation->setValue(sp->elevation.getValue());
    ui->spinIrradiance->setValue(sp->irradiance.getValue());
}

void SunDialog::makeSunShapeTab()
{
    for (SunFactory* f : m_sunShapeMap.values())
        ui->sunshapeCombo->addItem(f->icon(), f->name());

    SunShape* sunShape = (SunShape*) m_sun->getPart("shape", false);
    int index = ui->sunshapeCombo->findText(sunShape->getTypeName());
    ui->sunshapeCombo->setCurrentIndex(index);

    connect(
        ui->sunshapeCombo, SIGNAL(activated(int)),
        this, SLOT(setShape(int))
    );
    connect(
        ui->sunshapeParameters, SIGNAL(valueModified(SoNode*, QString, QString)),
        this, SLOT(setValue(SoNode*, QString, QString))
    );
}

void SunDialog::setShape(int index)
{
    SunFactory* f = m_sunShapeMap[ui->sunshapeCombo->itemText(index)];
    SunShape* sunShape = f->create();
    m_sun->setPart("shape", sunShape);
    ui->sunshapeParameters->SetContainer(sunShape);
}

void SunDialog::setValue(SoNode* node, QString parameter, QString value)
{
    if (parameter == "irradiance")
        if (value.toDouble() < 0.) return;

    SoField* field = node->getField(SbName(parameter.toStdString().c_str()));
    if (field)
        field->set(value.toStdString().c_str());
}

void SunDialog::makeSunApertureTab()
{
    ui->modelTreeView->setModel(m_sceneModel);
    QItemSelectionModel* selectionModel = new QItemSelectionModel(m_sceneModel);
    ui->modelTreeView->setSelectionModel(selectionModel);
    ui->modelTreeView->setRootIndex(QModelIndex());

    SunAperture* aperture = (SunAperture*) m_sun->getPart("aperture", false);
    QString nodes = aperture->disabledNodes.getValue().getString();
    for (QString s : nodes.split(';', QString::SkipEmptyParts))
        ui->disabledNodes->addItem(s);

    connect(
        ui->addNodeButton, SIGNAL(clicked()),
        this, SLOT(addNode())
    );
    connect(
        ui->removeNodeButton, SIGNAL(clicked()),
        this, SLOT(removeNode())
    );
}

void SunDialog::addNode()
{
    QItemSelectionModel* selectionModel = ui->modelTreeView->selectionModel();
    if (!selectionModel->hasSelection()) return;
    QModelIndex index = selectionModel->currentIndex();
    if (index == ui->modelTreeView->rootIndex()) return;
    if (!index.isValid()) return;
    InstanceNode* node = m_sceneModel->getInstance(index);
    if (!node) return;

    ui->disabledNodes->addItem(node->getURL());
}

void SunDialog::removeNode()
{
    QListWidgetItem* item = ui->disabledNodes->currentItem();
    if (!item) return;

    ui->disabledNodes->removeItemWidget(item);
    delete item;
}

void SunDialog::on_buttonSunCalc_clicked()
{
    //#ifndef NO_MARBLE

    //    SoSceneKit* coinScene = m_document->getSceneKit();
    //    if (!coinScene->getPart("lightList[0]", false) ) return;

        SunCalculatorDialog sunposDialog(this);
    //    connect(&sunposDialog, SIGNAL(changeSunLight(double,double)), this, SLOT(ChangeSunPosition(double,double)) );

        sunposDialog.exec();

    //#endif /* NO_MARBLE*/
}
