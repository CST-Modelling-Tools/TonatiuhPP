#pragma once

#include <QWidget>
#include <QDate>

namespace Ui {
    class DaysFilter;
}

class DaysFilter: public QWidget
{
    Q_OBJECT

public:
    explicit DaysFilter(QWidget* parent = 0);
    ~DaysFilter();

    void setLeapYear(bool leap);
    QDate dateA() const {return m_dateA;}
    QDate dateB() const {return m_dateB;}

signals:
    void filterChanged();

private slots:
    void updateDates();

private:
    Ui::DaysFilter* ui;
    int m_year;
    QDate m_dateA;
    QDate m_dateB;
};

