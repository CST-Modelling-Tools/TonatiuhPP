#include "ParametersItemField.h"

#include <Inventor/fields/SoField.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/sensors/SoFieldSensor.h>


void ParametersItemField::updateItem(void* data, SoSensor*)
{
    ParametersItemField* item = (ParametersItemField*) data;
    SbString v;
    item->m_field->get(v);
    item->setData(v.getString(), Qt::EditRole);
}


ParametersItemField::ParametersItemField(SoField* field):
    QStandardItem(),
    m_field(field)
{
     m_sensor = new SoFieldSensor(updateItem, this);
     m_sensor->attach(m_field);
}

ParametersItemField::~ParametersItemField()
{
    delete m_sensor;
}

QVariant ParametersItemField::data(int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (m_field) {
            if (SoSFBool* f = dynamic_cast<SoSFBool*>(m_field))
                return f->getValue() ? "true" : "false";
            else {
                SbString v;
                m_field->get(v);
                return v.getString();
            }
        }
    }
//        QString text = QStandardItem::data(role).toString();

//        // discard multilines
//        if (text.indexOf('\n') >= 0)
//            return QString("...");

//        // correct precision
//        QStringList list = text.split(" ");
//        QString ans;
//        for (int n = 0; n < list.size(); ++n) {
//            if (n > 0) ans += " ";
//            QString q = list[n];
//            bool ok;
//            double d = q.toDouble(&ok);
//            if (ok) ans += QString::number(d);
//            else
//                return text;
//        }
//        return ans;

//    {
//        if (column() == 0) return text();

//        SbString value = "null";
//        m_field->get(value);
//        return QString(value.getString());
//    }
    return QStandardItem::data(role);
}

void ParametersItemField::setData(const QVariant& value, int role)
{
      QStandardItem::setData(value, role);
}
