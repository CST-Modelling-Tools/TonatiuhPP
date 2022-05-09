#include "DaysFilter.h"
#include "ui_DaysFilter.h"

DaysFilter::DaysFilter(QWidget* parent):
    QWidget(parent),
    ui(new Ui::DaysFilter)
{
    ui->setupUi(this);

    m_year = 2001;
    m_dateA.setDate(m_year, 1, 1);
    m_dateB = m_dateA.addDays(1);

    connect(ui->widget, SIGNAL(dateChanged()), this, SLOT(updateDates()));
    connect(ui->spinBox_take, SIGNAL(valueChanged(int)), this, SLOT(updateDates()));
}

DaysFilter::~DaysFilter()
{
    delete ui;
}

void DaysFilter::setLeapYear(bool leap)
{
    if (leap) m_year = 2000;
    ui->widget->daysModel().setLeap(leap);
}

void DaysFilter::updateDates()
{
    int m = ui->widget->daysModel().month();
    int d = ui->widget->daysModel().day();
    int s = ui->spinBox_take->value();

    m_dateA.setDate(m_year, m, d);
    m_dateB = m_dateA.addDays(s);

    emit filterChanged();
}
