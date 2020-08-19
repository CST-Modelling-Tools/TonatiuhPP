#include "ParametersModel.h"

#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/lists/SoFieldList.h>
#include <Inventor/SbString.h>

#include "ParametersItem.h"


ParametersModel::ParametersModel(QObject* parent):
    QStandardItemModel(parent)
{
//    connect(this, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(onItemChanged(QStandardItem*)));
}

void ParametersModel::setNode(SoNode* node)
{
    beginResetModel();
    clear();

    setHorizontalHeaderLabels({"Parameter", "Value"});

    if (!node) return;
    m_node = node;

    SoFieldList fields;
    node->getFields(fields);
    for (int n = 0; n < fields.getLength(); ++n)
    {
        SoField* field = fields.get(n);
        SbName name;
        if (!node->getFieldName(field, name)) continue;

//        QStandardItem* itemName = new QStandardItem(name.getString());
        ParametersItem* itemName = new ParametersItem(name.getString());
        setItem(n, 0, itemName);
        ParametersItem* itemValue = new ParametersItem(field);
        setItem(n, 1, itemValue);

//        QModelIndex index = m_model->indexFromItem(itemValue);
//        openPersistentEditor(index);
    }
    endResetModel();
}

QVariant ParametersModel::data(const QModelIndex& index, int role) const
{
    ParametersItem* parameter = static_cast<ParametersItem*>(itemFromIndex(index));
    return parameter->data(role);
}

bool ParametersModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    ParametersItem* item = static_cast<ParametersItem*>(itemFromIndex(index));
    item->setData(value, role);

    SbName name;
    m_node->getFieldName(item->getField(), name);

    emit valueModified(m_node, name.getString(), value.toString());
    return true;
}

void ParametersModel::onItemChanged(QStandardItem* item)
{
    ParametersItem* pitem = (ParametersItem*) item;
    SbName name;
    m_node->getFieldName(pitem->getField(), name);

    emit valueModified(m_node, name.getString(), pitem->getText());


//    emit valueModified(node, field, value);

//    if (item->isCheckable()) {
//        ParametersItem* pitem = (ParametersItem*) item;
//        SoSFBool* f = dynamic_cast<SoSFBool*>(pitem->getField());
//        if (!f) return;
//        bool on = item->checkState() == Qt::Checked;
//        if (f->getValue() == on) return;

//        SbName name;
//        m_node->getFieldName(f, name);
//        emit valueModified(m_node, name.getString(), on ? "TRUE" : "FALSE");
//    }
}
