#include "SunCalculatorDialog.h"
#include "ui_SunCalculatorDialog.h"

#include <QCloseEvent>
#include <QDateTime>
#include <QSettings>

#include "libraries/math/gcf.h"


SunCalculatorDialog::SunCalculatorDialog(QWidget* parent):
    QDialog(parent),
    ui(new Ui::SunCalculatorDialog)
{
    ui->setupUi(this);

    setWindowFlag(Qt::WindowContextHelpButtonHint, false);

    readSettings();

    connect(ui->longitudeSpin, SIGNAL(valueChanged(double)), ui->worldMap, SLOT(LongitudeChanged(double)) );
    connect(ui->latitudeSpin, SIGNAL(valueChanged(double)), this, SLOT(calculateSunPosition()) );

    connect(ui->longitudeSpin, SIGNAL(valueChanged(double)), this, SLOT(calculateSunPosition()) );
    connect(ui->latitudeSpin, SIGNAL(valueChanged(double)), ui->worldMap, SLOT(LatitudeChanged(double)) );

    connect(ui->calendarWidget, SIGNAL(selectionChanged()), this, SLOT(calculateSunPosition()) );
    connect(ui->utTime, SIGNAL(timeChanged(QTime)), this, SLOT(calculateSunPosition()) );
    connect(ui->ctTime, SIGNAL(timeChanged(QTime)), this, SLOT(calculateSunPosition()) );
    connect(ui->zoneSpin, SIGNAL(valueChanged(int)), this, SLOT(calculateSunPosition()) );

    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(updatePosition(QAbstractButton*)) );

    int q = fontMetrics().height();
    resize(64*q, 36*q);

     ui->worldMap->LongitudeChanged(ui->longitudeSpin->value());
     ui->worldMap->LatitudeChanged(ui->latitudeSpin->value());
    calculateSunPosition();
}

SunCalculatorDialog::~SunCalculatorDialog()
{
    delete ui;
}

void SunCalculatorDialog::closeEvent(QCloseEvent* event)
{
    writeSettings();
    event->accept();
}

void SunCalculatorDialog::updatePosition(QAbstractButton* button)
{
    if (ui->buttonBox->buttonRole(button) == QDialogButtonBox::ApplyRole)
    {
        cSunCoordinates results = getPosition();
        emit changeSunLight(results.dAzimuth, 90. - results.dZenithAngle);
    }
}

void SunCalculatorDialog::calculateSunPosition()
{
    cSunCoordinates results = getPosition();
    ui->celestialWidget->CoordinatesChanged(results);
    ui->horizontalWidget->CoordinatesChanged(results);
    ui->worldMap->SunChanged(results);
}

QDateTime SunCalculatorDialog::getTime()
{
    QDate date = ui->calendarWidget->selectedDate();

    QTime time;
    if (ui->utRadio->isChecked())
        time = ui->utTime->time();
    else
        time = ui->ctTime->time().addSecs(-ui->zoneSpin->value()*3600);

    return QDateTime(date, time);
}

cSunCoordinates SunCalculatorDialog::getPosition()
{
    QDateTime time = getTime();
    int year = time.date().year();
    int month = time.date().month();
    int day = time.date().day();
    double hours = time.time().hour();
    double minutes = time.time().minute();
    double seconds = time.time().second();
    cTime myTime = {year, month, day, hours, minutes, seconds};

    double longitude = ui->longitudeSpin->value();
    double latitude = ui->latitudeSpin->value();
    cLocation myLocation = {longitude, latitude};

    cSunCoordinates results;
    sunpos(myTime, myLocation, &results);
    return results;
}

void SunCalculatorDialog::readSettings()
{
    QSettings settings("Tonatiuh", "Cyprus");

    ui->latitudeSpin->setValue(settings.value("SunCalculator.latitude", 0.).toDouble());
    ui->longitudeSpin->setValue(settings.value("SunCalculator.longitude", 0.).toDouble());

    ui->calendarWidget->setSelectedDate(settings.value("SunCalculator.date").toDate());
    ui->utRadio->setChecked(settings.value("SunCalculator.utTimeSelected", true).toBool());
    ui->ctRadio->setChecked(settings.value("SunCalculator.ctTimeSelected", false).toBool());
    ui->utTime->setTime(settings.value("SunCalculator.utTime").toTime());
    ui->ctTime->setTime(settings.value("SunCalculator.ctTime").toTime());
    ui->zoneSpin->setValue(settings.value("SunCalculator.zone", 0).toInt());
}

void SunCalculatorDialog::writeSettings()
{
    QSettings settings("Tonatiuh", "Cyprus");

    settings.setValue("SunCalculator.latitude", ui->latitudeSpin->value());
    settings.setValue("SunCalculator.longitude", ui->longitudeSpin->value());

    settings.setValue("SunCalculator.date", ui->calendarWidget->selectedDate());
    settings.setValue("SunCalculator.utTimeSelected", ui->utRadio->isChecked());
    settings.setValue("SunCalculator.ctTimeSelected", ui->ctRadio->isChecked());
    settings.setValue("SunCalculator.utTime", ui->utTime->time());
    settings.setValue("SunCalculator.ctTime", ui->ctTime->time());
    settings.setValue("SunCalculator.zone", ui->zoneSpin->value());
}
