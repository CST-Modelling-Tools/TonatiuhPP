#pragma once
#ifndef NO_MARBLE


#include <QDialog>

#include "ui_sunpositioncalculatordialog.h"
#include "kernel/auxiliary/sunpos.h"

class QDateTime;
class QTime;

class SunPositionCalculatorDialog: public QDialog, private Ui::SunPositionCalculatorDialog
{
    Q_OBJECT

public:
    SunPositionCalculatorDialog(QWidget* parent = 0);
    ~SunPositionCalculatorDialog();

protected:
    void closeEvent(QCloseEvent* event);

private slots:
    void UpdatePosition(QAbstractButton* button);
    void ChangeDate();
    void ChangeLongitude(double longitude);
    void ChangeLatitude(double latitude);
    void ChangeSunTime(QTime time);
    void ChangeSunTimeZone(int timeZone);

signals:
    void changeRepresentation(cSunCoordinates results);
    void changeSunLight(double azimuth, double zenith);

private:
    void CalculateSunPosition();
    QDateTime GetTime();
    void ReadSettings();
    void WriteSettings();

};
#endif /* NO_MARBLE */
