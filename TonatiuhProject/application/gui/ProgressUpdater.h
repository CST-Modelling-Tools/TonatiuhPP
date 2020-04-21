#pragma once


//#include <iostream>
//#include <string>
#include <QProgressDialog>

class ProgressUpdater
{
public:
    ProgressUpdater(int totalWork, const QString &title, int barLength, QWidget* creator);
    ~ProgressUpdater();

    void Update( int num = 1 );

private:
    QProgressDialog* m_progressDialog;
    const int m_totalBars;
    double m_totalWork;
    double m_barsPerUnitWork;
};
