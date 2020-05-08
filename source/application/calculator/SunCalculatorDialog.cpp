#include <QCloseEvent>
#include <QDateTime>
#include <QSettings>
#include <QTime>

#include "libraries/geometry/gc.h"

#include "SunCalculatorDialog.h"

SunCalculatorDialog::SunCalculatorDialog(QWidget* parent)
: QDialog(parent)
{
    setupUi(this);


    connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(UpdatePosition(QAbstractButton*)) );


    connect(calendarWidget, SIGNAL(selectionChanged()), this, SLOT(ChangeDate()) );

    connect(utTime, SIGNAL(timeChanged(QTime)), this, SLOT(ChangeSunTime(QTime)) );
    connect(ctTime, SIGNAL(timeChanged(QTime)), this, SLOT(ChangeSunTime(QTime)) );
    connect(zoneSpin, SIGNAL(valueChanged(int)), this, SLOT(ChangeSunTimeZone(int)) );

    connect(longitudeSpin, SIGNAL(valueChanged(double)), this, SLOT(ChangeLongitude(double)) );
    connect(longitudeSpin, SIGNAL(valueChanged(double)), worldMap, SLOT(LocalLongitudeChanged(double)) );
    connect(latitudeSpin, SIGNAL(valueChanged(double)), this, SLOT(ChangeLatitude(double)) );
    connect(latitudeSpin, SIGNAL(valueChanged(double)), worldMap, SLOT(LocalLatitudeChanged(double)) );

    connect(this, SIGNAL(changeRepresentation(cSunCoordinates)), worldMap, SLOT(SunChanged(cSunCoordinates)) );

    ReadSettings();
}

SunCalculatorDialog::~SunCalculatorDialog()
{
    WriteSettings();
}

void SunCalculatorDialog::closeEvent( QCloseEvent* event )
{
    WriteSettings();
    event->accept();
}

void SunCalculatorDialog::UpdatePosition( QAbstractButton* button )
{
    if (buttonBox->buttonRole(button) == QDialogButtonBox::ApplyRole)
    {
        //Fecha y Hora
        QDateTime time = GetTime();

        int year = time.date().year();
        int month = time.date().month();
        int day = time.date().day();

        double hours = time.time().hour ();
        double minutes = time.time().minute ();
        double seconds = time.time().second ();
        cTime myTime = { year, month, day, hours, minutes, seconds };

        //Localization
        double longitude = longitudeSpin->value();
        double latitude = latitudeSpin->value();
        cLocation myLocation = {longitude, latitude };

        //Calculate sun position
        cSunCoordinates results;
        sunpos(myTime, myLocation, &results);

        emit changeSunLight(results.dAzimuth,90 - results.dZenithAngle);
    }
}

void SunCalculatorDialog::ChangeDate()
{
    CalculateSunPosition();
}

void SunCalculatorDialog::ChangeLongitude(double /*longitude*/)
{
    CalculateSunPosition();
}

void SunCalculatorDialog::ChangeLatitude(double /*latitude*/)
{
    CalculateSunPosition();
}

void SunCalculatorDialog::ChangeSunTime(QTime /*time*/)
{
    CalculateSunPosition();
}

void SunCalculatorDialog::ChangeSunTimeZone(int /*timeZone*/)
{
    CalculateSunPosition();
}

void SunCalculatorDialog::CalculateSunPosition()
{

    //Fecha y Hora
    QDateTime time = GetTime();

    int year = time.date().year();
    int month = time.date().month();
    int day = time.date().day();

    double hours = time.time().hour ();
    double minutes = time.time().minute ();
    double seconds = time.time().second ();
    cTime myTime = { year, month, day, hours, minutes, seconds };

    //Localization
    double longitude = longitudeSpin->value();
    double latitude = latitudeSpin->value();
    cLocation myLocation = {longitude , latitude };

    //Calculate sun position
    cSunCoordinates results;
    sunpos( myTime, myLocation, &results );
    //update celestial and horizontal coordinates
    celestialWidget->CoordinatesChanged(results);
    horizontalWidget->CoordinatesChanged(results);
    emit changeRepresentation(results);
}

QDateTime SunCalculatorDialog::GetTime()
{
    QTime time = utRadio->isChecked() ? utTime->time() : ctTime->time().addSecs( - zoneSpin->value() * 3600 );
    QDateTime dateTime( calendarWidget->selectedDate(), time );
    return dateTime;
}

void SunCalculatorDialog::ReadSettings()
{
    QSettings settings("CyI", "Tonatiuh");
    calendarWidget->setSelectedDate(settings.value("SunCalculator.date").toDate() );
    utRadio->setChecked(settings.value("SunCalculator.utTimeSelected", true).toBool() );
    ctRadio->setChecked(settings.value("SunCalculator.ctTimeSelected", false).toBool() );
    utTime->setTime(settings.value("SunCalculator.utTime").toTime() );
    ctTime->setTime(settings.value("SunCalculator.ctTime").toTime() );
    zoneSpin->setValue(settings.value("SunCalculator.zone", 0).toInt() );
    latitudeSpin->setValue(settings.value("SunCalculator.latitude", 0).toDouble() );
    longitudeSpin->setValue(settings.value("SunCalculator.longitude", 0).toDouble() );
}

void SunCalculatorDialog::WriteSettings()
{
    QSettings settings("CyI", "Tonatiuh");
    settings.setValue("SunCalculator.date", calendarWidget->selectedDate() );
    settings.setValue("SunCalculator.utTimeSelected", utRadio->isChecked() );
    settings.setValue("SunCalculator.ctTimeSelected", ctRadio->isChecked() );
    settings.setValue("SunCalculator.utTime", utTime->time() );
    settings.setValue("SunCalculator.ctTime", ctTime->time() );
    settings.setValue("SunCalculator.zone", zoneSpin->value() );
    settings.setValue("SunCalculator.latitude", latitudeSpin->value() );
    settings.setValue("SunCalculator.longitude", longitudeSpin->value() );
}
