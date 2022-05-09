#pragma once

#include <QStandardItem>

class ParameterDouble;

class ParameterAbstract: public QStandardItem
{
public:
    ParameterAbstract(const QString& name, const QString& text = "", bool editable = false);
    QString name() {return m_name;}

private:
    QString m_name;
};

class ParameterDouble: public ParameterAbstract
{
public:
    ParameterDouble(const QString& name, double value = 0., QString quantity = "", QString format = "");

    double value() {return m_value;}
    const QString& quantity() {return m_quantity;}
    const QString& format() {return m_format;}

    void setValue(double value);
    void setData(const QVariant& value, int role);

private:
    QString toString(double value);
    double fromString(QString text);

private:
    double m_value;
    QString m_quantity;
    QString m_format;
};


// ParameterInt, ParameterEnum, ParameterString

/*
examples
3.1415 (variant double)
"angle" (radians)
"{.4f} deg"
"180 deg"

3600
"time" (seconds)
"{h:d}h {m:02d}m"
"1h 00m"

3600
"energy" (Joule)
"{.3} kWh"
"1.000 kWh"

"group"
*/
