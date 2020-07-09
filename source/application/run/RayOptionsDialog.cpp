#include "RayOptionsDialog.h"
#include <ui_RayOptionsDialog.h>

#include "kernel/random/Random.h"


RayOptionsDialog::RayOptionsDialog(
    int raysNumber,
    QVector<RandomFactory*> randomFactories, int raysRandomFactory,
    int raysPlaneWidth, int raysPlaneHeight,
    bool drawRays, bool drawPhotons,
    int photonBufferSize, bool photonBufferAppend,
    QWidget* parent, Qt::WindowFlags f
):
    QDialog(parent, f),
    ui(new Ui::RayOptionsDialog)
{
    ui->setupUi(this);

    ui->raysNumberSpin->setValue(raysNumber);
    for (RandomFactory* f : randomFactories)
        ui->raysRandomFactoryCombo->addItem(f->icon(), f->name());
    ui->raysRandomFactoryCombo->setCurrentIndex(raysRandomFactory);
    ui->raysPlaneWidthSpin->setValue(raysPlaneWidth);
    ui->raysPlaneHeightSpin->setValue(raysPlaneHeight);

    ui->drawRaysCheck->setChecked(drawRays);
    ui->drawPhotonsCheck->setChecked(drawPhotons);

    ui->photonBufferSizeSpin->setValue(photonBufferSize);
    ui->photonBufferAppendRadio->setChecked(photonBufferAppend);
}

RayOptionsDialog::~RayOptionsDialog()
{
    delete ui;
}

int RayOptionsDialog::raysNumber() const {return ui->raysNumberSpin->value();}
int RayOptionsDialog::raysRandomFactory() const {return ui->raysRandomFactoryCombo->currentIndex();}
int RayOptionsDialog::rayPlaneWidth() const {return ui->raysPlaneWidthSpin->value();}
int RayOptionsDialog::rayPlaneHeight() const {return ui->raysPlaneHeightSpin->value();}

bool RayOptionsDialog::drawRays() const {return ui->drawRaysCheck->isChecked();}
bool RayOptionsDialog::drawPhotons() const {return ui->drawPhotonsCheck->isChecked();}

int RayOptionsDialog::photonBufferSize() const {return ui->photonBufferSizeSpin->value();}
bool RayOptionsDialog::photonBufferAppend() const {return ui->photonBufferAppendRadio->isChecked();}
