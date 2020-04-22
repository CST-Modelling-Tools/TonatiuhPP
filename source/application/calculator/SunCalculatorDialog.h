#pragma once

#include <QDialog>

#include "ui_suncalculatordialog.h"
#include "kernel/auxiliary/sunpos.h"

class QDateTime;
class QTime;

class SunCalculatorDialog: public QDialog, private Ui::SunPositionCalculatorDialog
{
    Q_OBJECT

public:
    SunCalculatorDialog(QWidget* parent = 0);
    ~SunCalculatorDialog();

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
