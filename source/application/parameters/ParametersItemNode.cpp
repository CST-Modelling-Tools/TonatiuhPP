#include "ParametersItemNode.h"

#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/lists/SoFieldList.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoGroup.h>

#include "ParametersItemField.h"


ParametersItemNode::ParametersItemNode(QString part, SoNode* node):
    QStandardItem(part),
    m_node(node)
{
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
        ParametersItemField* itemValue = new ParametersItemField(field);

        appendRow({itemName, itemValue});

//        QModelIndex index = m_model->indexFromItem(itemValue);
//        openPersistentEditor(index);
    }
}
