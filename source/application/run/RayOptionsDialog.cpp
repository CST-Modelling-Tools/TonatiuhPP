#include "RayOptionsDialog.h"
#include <ui_RayOptionsDialog.h>

#include "kernel/random/Random.h"


RayOptionsDialog::RayOptionsDialog(int raysNumber, int raysScreen,
    QVector<RandomFactory*> randomFactories, int raysRandomFactory,
    int raysGridWidth, int raysGridHeight,
    int photonBufferSize, bool photonBufferAppend,
    QWidget* parent, Qt::WindowFlags f
):
    QDialog(parent, f),
    ui(new Ui::RayOptionsDialog)
{
    ui->setupUi(this);

    ui->raysNumberSpin->setValue(raysNumber);
    ui->raysScreenSpin->setValue(raysScreen);
    for (RandomFactory* f : randomFactories)
        ui->raysRandomFactoryCombo->addItem(f->icon(), f->name());
    ui->raysRandomFactoryCombo->setCurrentIndex(raysRandomFactory);
    ui->raysPlaneWidthSpin->setValue(raysGridWidth);
    ui->raysPlaneHeightSpin->setValue(raysGridHeight);

    ui->photonBufferSizeSpin->setValue(photonBufferSize);
    ui->photonBufferAppendRadio->setChecked(photonBufferAppend);
}

RayOptionsDialog::~RayOptionsDialog()
{
    delete ui;
}

int RayOptionsDialog::raysNumber() const {return ui->raysNumberSpin->value();}
int RayOptionsDialog::raysScreen() const {return ui->raysScreenSpin->value();}
int RayOptionsDialog::raysRandomFactory() const {return ui->raysRandomFactoryCombo->currentIndex();}
int RayOptionsDialog::raysGridWidth() const {return ui->raysPlaneWidthSpin->value();}
int RayOptionsDialog::raysGridHeight() const {return ui->raysPlaneHeightSpin->value();}

int RayOptionsDialog::photonBufferSize() const {return ui->photonBufferSizeSpin->value();}
bool RayOptionsDialog::photonBufferAppend() const {return ui->photonBufferAppendRadio->isChecked();}
