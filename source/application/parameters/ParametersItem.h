#pragma once

#include <QStandardItem>
#include <QList>
#include <QString>

class SoField;
class SoFieldSensor;


class ParametersItem: public QStandardItem
{
public:

    ParametersItem(QString text, bool editable, SoField* field);
    ~ParametersItem();
    SoField* GetField() const {return m_pField;}
    
    QVariant data (int role = Qt::UserRole + 1) const;
    void setData (const QVariant & value, int role = Qt::UserRole + 1);
    QString text () const; 
    
private:
    SoField* m_pField;
    SoFieldSensor* m_pFieldSensor;
    QString m_text;
};
