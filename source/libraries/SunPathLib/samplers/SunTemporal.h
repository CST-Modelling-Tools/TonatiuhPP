#pragma once

#include <QSharedPointer>

#include "SunPathLib/samplers/TimeSampler.h"
#include "SunPathLib/data/SunFunctor.h"

namespace sp {


class InterpolatorLinear;
class InterpolatorCubicMono;


class SUNPATHLIB SunTemporal
{
public:
    SunTemporal(SunCalculator& calculator);
    SunCalculator* calculator() {return m_calculator.get();}

    void setTimeStamps(const QVector<TimeStamp>& timeStamps);
    const QVector<TimeStamp>& timeStamps() const {return m_timeStamps;}

    void setData(const QVector<double>& data);
    void setData(const SunFunctor& sf);
    const QVector<double>& data() const {return m_data;}

    double interpolate(QDateTime t) const;

    double integrate() const;
    double integrate(QDateTime tA, QDateTime tB) const;

    double average() const;
    double average(QDateTime tA, QDateTime tB) const;

    double integrateWeighted(const SunFunctor& sf) const;

protected:
    QSharedPointer<SunCalculator> m_calculator;
    QVector<TimeStamp> m_timeStamps;
    double m_timeStepH;
    QVector<double> m_data;
    QSharedPointer<InterpolatorLinear> m_interpolator;
};


} // namespace sp
