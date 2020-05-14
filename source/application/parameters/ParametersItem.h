#pragma once

#include <QStandardItem>
#include <QList>
#include <QString>

class SoField;
class SoSensor;
class SoFieldSensor;


class ParametersItem: public QStandardItem
{
public:
    ParametersItem(QString text, bool editable, SoField* field);
    ~ParametersItem();

    QString getText() const {return text();}
    SoField* getField() const {return m_field;}
    
    QVariant data(int role = Qt::UserRole + 1) const;
    void setData(const QVariant& value, int role = Qt::UserRole + 1);
    
    static void updateItem(void* data, SoSensor*);

private:
    SoField* m_field;
    SoFieldSensor* m_sensor;
};
