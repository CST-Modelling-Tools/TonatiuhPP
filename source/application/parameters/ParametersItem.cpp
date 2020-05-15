#include "ParametersItem.h"

#include <QString>

#include <Inventor/SbString.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/lists/SoFieldList.h>
#include <Inventor/sensors/SoFieldSensor.h>


void ParametersItem::updateItem(void* data, SoSensor*)
{
    ParametersItem* item = (ParametersItem*) data;
    SbString v;
    item->m_field->get(v);
    item->setData(v.getString(), Qt::DisplayRole);
}

ParametersItem::ParametersItem(QString text, bool editable, SoField* field):
    QStandardItem(text),
    m_field(field)
{
     setEditable(editable);
     if (editable) {
        m_sensor = new SoFieldSensor(updateItem, this);
        m_sensor->attach(m_field);
     } else
         m_sensor = 0;
}

ParametersItem::~ParametersItem()
{
    delete m_sensor;
}

QVariant ParametersItem::data(int role) const
{
    if (role == Qt::DisplayRole)
    {
        bool ok;
        double d = QStandardItem::data(role).toDouble(&ok);
        if (ok) return QString::number(d);
    }
//    {
//        if (column() == 0) return text();

//        SbString value = "null";
//        m_field->get(value);
//        return QString(value.getString());
//    }
    return QStandardItem::data(role);
}

void ParametersItem::setData(const QVariant& value, int role)
{
//    if (role == Qt::EditRole)
//    {
//        if (column() == 1)
//            setText(value.toString());
//    }
//    else if (role == Qt::UserRole)
//    {
//        if (m_field)
//            m_field->set(value.toString().toStdString().c_str());
//    }
//    else
        return QStandardItem::setData(value, role);
}
