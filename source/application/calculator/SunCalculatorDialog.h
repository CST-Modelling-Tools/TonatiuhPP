#pragma once

#include <QDialog>

class QAbstractButton;

#include "libraries/sun/sunpos.h"

namespace Ui {
class SunCalculatorDialog;
}


class SunCalculatorDialog: public QDialog
{
    Q_OBJECT

public:
    SunCalculatorDialog(QWidget* parent = 0);
    ~SunCalculatorDialog();

    QDateTime getTime();
    cSunCoordinates getPosition();

signals:
    void changeSunLight(double azimuth, double zenith);

protected:
    void closeEvent(QCloseEvent* event);

protected slots:
    void updatePosition(QAbstractButton* button);
    void calculateSunPosition();

private:
    void readSettings();
    void writeSettings();

private:
    Ui::SunCalculatorDialog* ui;
};
