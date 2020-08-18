#include "AirDialog.h"
#include "ui_AirDialog.h"

#include <Inventor/fields/SoField.h>

#include "kernel/air/AirTransmission.h"
#include "kernel/air/AirKit.h"
#include "tree/SceneModel.h"
#include "kernel/scene/TSceneKit.h"
#include "parameters/ParametersModel.h"


AirDialog::AirDialog(SceneModel* sceneModel, QMap<QString, AirFactory*> airMap, QWidget* parent):
    QDialog(parent),
    ui(new Ui::AirDialog),
    m_airMap(airMap)
{
    ui->setupUi(this);

    TSceneKit* sceneKit = sceneModel->getSceneKit();
    AirKit* airOld = (AirKit*) sceneKit->getPart("world.air", false);
    m_air = (AirKit*) airOld->copy();
    m_air->ref();

    for (AirFactory* f : airMap)
        ui->comboBox->addItem(f->icon(), f->name());

    AirTransmission* airT = (AirTransmission*) m_air->getPart("transmission", false);
    int index = ui->comboBox->findText(airT->getTypeName());
    ui->comboBox->setCurrentIndex(index); // activated != currentIndexChanged
    ParametersModel* model = new ParametersModel(this);
    ui->airParameters->setModel(model);
    model->setNode(airT);

    connect(
        ui->comboBox, SIGNAL(activated(int)),
        this, SLOT(setModel(int))
    );
    connect(
        model, SIGNAL(valueModified(SoNode*, QString, QString)),
        this, SLOT(setValue(SoNode*, QString, QString))
    );

    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
}

AirDialog::~AirDialog()
{
    delete ui;
    m_air->unref();
}

void AirDialog::setModel(int index)
{
    AirFactory* f = m_airMap[ui->comboBox->itemText(index)];
    AirTransmission* airT = f->create();
    ui->airParameters->getModel()->setNode(airT);
    m_air->setPart("transmission", airT);
}

void AirDialog::setValue(SoNode* node, QString field, QString value)
{
    SoField* f = node->getField(field.toStdString().c_str());
    if (f) f->set(value.toStdString().c_str());
}
