#pragma once

#include <QStandardItem>
#include <QList>
#include <QString>

class SoField;
class SoFieldSensor;


class ParametersItem: public QStandardItem
{
public:
    ParametersItem(QString getText, bool editable, SoField* field);
    ~ParametersItem();

    QString getText() const {return m_text;}
    SoField* getField() const {return m_field;}
    
    QVariant data(int role = Qt::UserRole + 1) const;
    void setData(const QVariant& value, int role = Qt::UserRole + 1);
    
private:
    QString m_text;
    SoField* m_field;
    SoFieldSensor* m_sensor;
};
