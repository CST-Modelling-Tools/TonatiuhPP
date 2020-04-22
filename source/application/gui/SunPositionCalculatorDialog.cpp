#ifndef NO_MARBLE

#include <QCloseEvent>
#include <QDateTime>
#include <QSettings>
#include <QTime>

#include "libraries/geometry/gc.h"

#include "SunPositionCalculatorDialog.h"

SunPositionCalculatorDialog::SunPositionCalculatorDialog( QWidget* parent )
: QDialog( parent )
{
    setupUi(this);


    connect( buttonBox, SIGNAL( clicked( QAbstractButton* ) ), this, SLOT( UpdatePosition( QAbstractButton* ) ) );


    connect( calendarWidget, SIGNAL( selectionChanged( ) ), this, SLOT( ChangeDate( ) ) );

    connect( utTime, SIGNAL( timeChanged( QTime ) ), this, SLOT( ChangeSunTime( QTime ) ) );
    connect( ctTime, SIGNAL( timeChanged( QTime ) ), this, SLOT( ChangeSunTime( QTime ) ) );
    connect( zoneSpin, SIGNAL( valueChanged( int ) ), this, SLOT( ChangeSunTimeZone( int ) ) );

    connect( longitudeSpin, SIGNAL( valueChanged(  double ) ), this, SLOT( ChangeLongitude( double ) ) );
    connect( longitudeSpin, SIGNAL( valueChanged(  double ) ), worldMap, SLOT( LocalLongitudeChanged( double ) ) );
    connect( latitudeSpin, SIGNAL( valueChanged( double) ), this, SLOT( ChangeLatitude( double ) ) );
    connect( latitudeSpin, SIGNAL( valueChanged(  double ) ), worldMap, SLOT( LocalLatitudeChanged( double ) ) );

    connect( this, SIGNAL( changeRepresentation( cSunCoordinates ) ), worldMap, SLOT( SunChanged( cSunCoordinates ) ) );

    ReadSettings();
}

SunPositionCalculatorDialog::~SunPositionCalculatorDialog()
{
    WriteSettings();
}

void SunPositionCalculatorDialog::closeEvent( QCloseEvent* event )
{
    WriteSettings();
    event->accept();

}

void SunPositionCalculatorDialog::UpdatePosition( QAbstractButton* button )
{
    if( buttonBox->buttonRole( button ) == QDialogButtonBox::ApplyRole )
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

        emit changeSunLight( results.dAzimuth ,90-results.dZenithAngle );
    }

}

void SunPositionCalculatorDialog::ChangeDate()
{
    CalculateSunPosition();
}

void SunPositionCalculatorDialog::ChangeLongitude( double /*longitude*/ )
{
    CalculateSunPosition();
}

void SunPositionCalculatorDialog::ChangeLatitude( double /*latitude*/ )
{
    CalculateSunPosition();
}

void SunPositionCalculatorDialog::ChangeSunTime( QTime /*time*/ )
{
    CalculateSunPosition();
}

void SunPositionCalculatorDialog::ChangeSunTimeZone( int /*timeZone*/ )
{
    CalculateSunPosition();
}


void SunPositionCalculatorDialog::CalculateSunPosition()
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

QDateTime SunPositionCalculatorDialog::GetTime()
{
    QTime time = utRadio->isChecked() ? utTime->time() : ctTime->time().addSecs( - zoneSpin->value() * 3600 );

    QDateTime dateTime( calendarWidget->selectedDate(), time );
    return dateTime;

}


void SunPositionCalculatorDialog::ReadSettings()
{
    QSettings settings( "NREL UTB CENER", "Tonatiuh" );
    calendarWidget->setSelectedDate( settings.value( "sunpositioncalculator.date" ).toDate() );


    utRadio->setChecked( settings.value( "sunpositioncalculator.utTimeSelected", true ).toBool() );
    ctRadio->setChecked( settings.value( "sunpositioncalculator.ctTimeSelected", false  ).toBool() );
    utTime->setTime( settings.value( "sunpositioncalculator.utTime" ).toTime() );
    ctTime->setTime( settings.value( "sunpositioncalculator.ctTime" ).toTime() );
    zoneSpin->setValue( settings.value( "sunpositioncalculator.zone", 0 ).toInt() );
    latitudeSpin->setValue( settings.value( "sunpositioncalculator.latitude", 0 ).toDouble() );
    longitudeSpin->setValue( settings.value( "sunpositioncalculator.longitude", 0 ).toDouble() );
}

void SunPositionCalculatorDialog::WriteSettings()
{
    QSettings settings( "NREL UTB CENER", "Tonatiuh" );
    settings.setValue( "sunpositioncalculator.date", calendarWidget->selectedDate() );

    settings.setValue( "sunpositioncalculator.utTimeSelected", utRadio->isChecked() );
    settings.setValue( "sunpositioncalculator.ctTimeSelected", ctRadio->isChecked() );
    settings.setValue( "sunpositioncalculator.utTime", utTime->time() );
    settings.setValue( "sunpositioncalculator.ctTime", ctTime->time() );
    settings.setValue( "sunpositioncalculator.zone", zoneSpin->value() );
    settings.setValue( "sunpositioncalculator.latitude", latitudeSpin->value() );
    settings.setValue( "sunpositioncalculator.longitude", longitudeSpin->value() );

}
#endif /* NO_MARBLE*/
