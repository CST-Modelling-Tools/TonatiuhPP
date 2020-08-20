#include "ParametersItemNode.h"

#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/lists/SoFieldList.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoGroup.h>

#include "ParametersItem.h"

ParametersItemNode::ParametersItemNode(SoNode* node):
    QStandardItem(),
    m_node(node)
{
    QString name = node->getName().getString();
    if (name.isEmpty())
        name = node->getTypeId().getName().getString();
    setText(name);
    setEditable(false);

    SoFieldList fields;
    node->getFields(fields);
    for (int n = 0; n < fields.getLength(); ++n)
    {
        SoField* field = fields.get(n);
        SbName name;
        node->getFieldName(field, name);

        QStandardItem* itemName = new QStandardItem(name.getString());
        itemName->setEditable(false);
        ParametersItem* itemValue = new ParametersItem(field);

        appendRow({itemName, itemValue});

//        QModelIndex index = m_model->indexFromItem(itemValue);
//        openPersistentEditor(index);
    }
}
