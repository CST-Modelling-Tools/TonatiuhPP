#include "ParametersItemNode.h"

#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodekits/SoShapeKit.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/lists/SoFieldList.h>
#include <Inventor/SbName.h>

#include "ParametersItemField.h"


ParametersItemNode::ParametersItemNode(QString part, SoNode* node):
    QStandardItem(part),
    m_node(node)
{
    if (!node) return;
    setEditable(false);

    SoFieldList fields;
    node->getFields(fields);
    for (int n = 0; n < fields.getLength(); ++n)
    {
        SoField* field = fields.get(n);
        SbName name;
        node->getFieldName(field, name);

        if (SoSFNode* fn = dynamic_cast<SoSFNode*>(field)) {
            SoNode* nodeSub = fn->getValue();
            if (!nodeSub) continue;
            if (nodeSub->getTypeId().isDerivedFrom(SoGroup::getClassTypeId())) continue;
            if (nodeSub->getTypeId().isDerivedFrom(SoShapeKit::getClassTypeId())) continue;
            QString nameType;
            nameType = nodeSub->getTypeId().getName().getString();
            QStandardItem* item = new QStandardItem(nameType);
            item->setEditable(false);
            appendRow({new ParametersItemNode(name.getString(), nodeSub), item});
        } else {
            QStandardItem* itemName = new QStandardItem(name.getString());
            itemName->setEditable(false);
            ParametersItemField* itemValue = new ParametersItemField(field);

            appendRow({itemName, itemValue});

        }
    }
}

//        QModelIndex index = m_model->indexFromItem(itemValue);
//        openPersistentEditor(index);
