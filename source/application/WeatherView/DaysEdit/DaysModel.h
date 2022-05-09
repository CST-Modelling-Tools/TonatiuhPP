#pragma once

#include <QDate>


// Date with month and day number

class DaysModel
{
public:
    DaysModel();

    bool leap() const {return m_leap;}
    int month() const {return m_month;}
    int day() const {return m_day;}

    void setLeap(bool leap);
    void setMonthDay(int m, int d);
    void setMonth(int m);
    void setDay(int d);

    void addMonths(int m);
    void addDays(int d);

    int daysInYear();
    int daysInMonth();
    int dayNumber();
    void fromDayNumber(int d);

private:
    int m_month;
    int m_day;
    bool m_leap;
};

