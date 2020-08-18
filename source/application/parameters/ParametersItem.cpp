#include "ParametersItem.h"

#include <QString>

#include <Inventor/SbString.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/lists/SoFieldList.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/fields/SoSFBool.h>


void ParametersItem::updateItem(void* data, SoSensor*)
{
    ParametersItem* item = (ParametersItem*) data;
    SbString v;
    item->m_field->get(v);
    if (item->data() == v.getString()) return;
    item->setData(v.getString(), Qt::DisplayRole);
}

ParametersItem::ParametersItem(QString text, bool editable, SoField* field):
    QStandardItem(text),
    m_field(field),
    m_sensor(0)
{
     setEditable(editable);
     if (editable) {
        m_sensor = new SoFieldSensor(updateItem, this);
        m_sensor->attach(m_field);

//        if (SoSFBool* f = dynamic_cast<SoSFBool*>(field)) {
//            setCheckable(true);
//            setCheckState(f->getValue() ? Qt::Checked : Qt::Unchecked);
//        }
     }
}

ParametersItem::~ParametersItem()
{
    delete m_sensor;
}

QVariant ParametersItem::data(int role) const
{
    if (role == Qt::DisplayRole)
    {
//        if (isEditable() && dynamic_cast<SoSFBool*>(m_field)) return "";
        QString text = QStandardItem::data(role).toString();

        // discard multilines
        if (text.indexOf('\n') >= 0)
            return QString("...");

        // correct precision
        QStringList list = text.split(" ");
        QString ans;
        for (int n = 0; n < list.size(); ++n) {
            if (n > 0) ans += " ";
            QString q = list[n];
            bool ok;
            double d = q.toDouble(&ok);
            if (ok) ans += QString::number(d);
            else
                return text;
        }
        return ans;
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
      QStandardItem::setData(value, role);
}
