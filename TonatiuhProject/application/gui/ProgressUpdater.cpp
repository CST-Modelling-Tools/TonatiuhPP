#include "ProgressUpdater.h"
#include <QString>

using namespace std;

ProgressUpdater::ProgressUpdater( int totalWork, const QString &title, int bar_length, QWidget *creator )
: m_totalBars( bar_length )
{
    m_progressDialog = new QProgressDialog ( title , "Cancel",  0, bar_length, creator );
    m_progressDialog->setModal( true );
    m_progressDialog->setCancelButton( NULL);
    m_progressDialog->setVisible( true );

    m_totalWork = totalWork;
    m_barsPerUnitWork = m_totalBars/m_totalWork;
}

ProgressUpdater::~ProgressUpdater()
{
    delete m_progressDialog;
}

void ProgressUpdater::Update( int work )
{
    m_progressDialog->setValue( work * m_barsPerUnitWork );
}

