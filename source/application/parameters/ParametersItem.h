#pragma once

#include <QStandardItem>

class SoField;
class SoSensor;
class SoFieldSensor;


class ParametersItem: public QStandardItem
{
public:
    ParametersItem(SoField* field);
    ~ParametersItem();
    
    QVariant data(int role = Qt::UserRole + 1) const;
    void setData(const QVariant& value, int role = Qt::UserRole + 1);
    
    SoField* field() const {return m_field;}

private:
    static void updateItem(void* data, SoSensor*);

private:
    SoField* m_field;
    SoFieldSensor* m_sensor;
};
