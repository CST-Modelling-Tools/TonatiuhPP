#include "ParametersItemField.h"

#include <Inventor/fields/SoField.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "ParametersItemNode.h"
#include "kernel/scene/TNode.h"


void ParametersItemField::updateItem(void* data, SoSensor*)
{
    ParametersItemField* itemField = (ParametersItemField*) data;
    if (SoSFNode* f = dynamic_cast<SoSFNode*>(itemField->m_field))
    {
        QStandardItem* item = itemField->parent()->child(itemField->row(), 0);
        ParametersItemNode* itemNode = (ParametersItemNode*) item;
        itemNode->setNode(f->getValue());// this line changes current
    }
    itemField->emitDataChanged();
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
        if (SoSFNode* f = dynamic_cast<SoSFNode*>(m_field))
        {
            if (TNode* tn = dynamic_cast<TNode*>(f->getValue()))
                return tn->getTypeName();
//            else
//                return f->getValue()->getTypeId().getName().getString();
        }
        else if (SoSFBool* f = dynamic_cast<SoSFBool*>(m_field))
            return f->getValue() ? "true" : "false";
        else {
            SbString v;
            m_field->get(v);
            QString txt = v.getString();

            // discard multilines
            if (txt.indexOf('\n') >= 0)
                return QString("...");

            // correct precision
            QStringList list = txt.split(" ");
            QString ans;
            for (int n = 0; n < list.size(); ++n) {
                if (n > 0) ans += " ";
                QString q = list[n];
                bool ok;
                double d = q.toDouble(&ok);
                if (ok)
                    ans += QString::number(d);
                else
                    return txt;
            }
            return ans;
        }
    }
    else if (role == Qt::DecorationRole)
    {
        if (SoSFNode* f = dynamic_cast<SoSFNode*>(m_field))
            if (TNode* tn = dynamic_cast<TNode*>(f->getValue()))
                return QIcon(tn->getTypeIcon());
    }
    else if (role == Qt::EditRole)
    {
        SbString v;
        m_field->get(v);
        return v.getString();
    }

    return QStandardItem::data(role);
}

void ParametersItemField::setData(const QVariant& value, int role)
{
    QStandardItem::setData(value, role);
}
