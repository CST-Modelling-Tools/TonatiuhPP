#include "DaysModel.h"

const int sYear = 2001;
int sDaysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

template<class T>
T clamp(T x, T a, T b)
{
    if (x <= a) return a;
    if (x >= b) return b;
    return x;
}

DaysModel::DaysModel()
{
    m_leap = false;
    m_month = 1;
    m_day = 1;
}

void DaysModel::setLeap(bool leap)
{
    m_leap = leap;
    sDaysInMonth[2] = leap ? 29 : 28;
}

void DaysModel::setMonthDay(int m, int d)
{
    if (m == m_month && d == m_day) return;

    m = clamp(m, 1, 12);
    d = clamp(d, 1, sDaysInMonth[m]);
    if (m == m_month && d == m_day) return;

    m_month = m;
    m_day = d;
}

void DaysModel::setMonth(int m)
{
    setMonthDay(m, m_day);
}

void DaysModel::setDay(int d)
{
    setMonthDay(m_month, d);
}

void DaysModel::addMonths(int m)
{
    setMonthDay(m_month + m, m_day);
}

void DaysModel::addDays(int d)
{
    fromDayNumber(dayNumber() + d);
}

int DaysModel::daysInYear()
{
    return m_leap ? 366 : 365;
}

int DaysModel::daysInMonth()
{
    return sDaysInMonth[m_month];
}

int DaysModel::dayNumber()
{
    int ans = m_day;
    for (int m = 1; m < m_month; m++)
        ans += sDaysInMonth[m];
    return ans;
}

void DaysModel::fromDayNumber(int d)
{
    d = clamp(d, 1, daysInYear());
    int m = 1;
    while (true) {
        int dMax = sDaysInMonth[m];
        if (d > dMax) {
            d -= dMax;
            m++;
        } else
            break;
    }
    setMonthDay(m, d);
}

