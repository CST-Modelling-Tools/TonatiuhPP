#include "ParametersItem.h"

#include <QString>

#include <Inventor/SbString.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/lists/SoFieldList.h>
#include <Inventor/sensors/SoFieldSensor.h>


void updateItem(void* data, SoSensor*)
{
    ParametersItem* item = (ParametersItem*) data;
    item->setData(item->data(Qt::DisplayRole), Qt::DisplayRole);
}

ParametersItem::ParametersItem(QString text, bool editable, SoField* field):
    QStandardItem(text),
    m_text(text),
    m_field(field),
    m_sensor(0)
{
     setEditable(editable);
     m_sensor = new SoFieldSensor(updateItem, this);
     m_sensor->attach(m_field);
}

ParametersItem::~ParametersItem()
{
    delete m_sensor;
}

QVariant ParametersItem::data(int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (column() == 0) return m_text;

        SbString fieldValue = "null";
        m_field->get(fieldValue);
        return QString(fieldValue.getString());
    }
    else
        return QStandardItem::data(role);
}

void ParametersItem::setData(const QVariant& value, int role)
{
    if (role == Qt::EditRole) {
        if (column() == 1)
            m_text = value.toString();
    }
    else if (role == Qt::UserRole)
    {
        if (m_field)
            m_field->set(value.toString().toStdString().c_str());
    }
    else
        return QStandardItem::setData(value, role);
}
