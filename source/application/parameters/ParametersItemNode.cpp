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
    QStandardItem(part)
{
    setEditable(false);
    setNode(node);
}

void ParametersItemNode::setNode(SoNode* node)
{
    m_node = node;

    removeRows(0, rowCount());
    if (!m_node) return;

    SoFieldList fields;
    m_node->getFields(fields);
    for (int n = 0; n < fields.getLength(); ++n)
    {
        SoField* field = fields.get(n);
        SbName sname;
        m_node->getFieldName(field, sname);
        QString name = sname.getString();

        if (SoSFNode* fn = dynamic_cast<SoSFNode*>(field)) {
            SoNode* nodeSub = fn->getValue();
            if (!nodeSub) continue;
            if (nodeSub->getTypeId().isDerivedFrom(SoGroup::getClassTypeId())) continue;
            ParametersItemNode* itemNode = new ParametersItemNode(name, nodeSub);
            ParametersItemField* itemType = new ParametersItemField(field);
            appendRow({itemNode, itemType});
        } else {
            QStandardItem* itemName = new QStandardItem(name);
            itemName->setEditable(false);
            ParametersItemField* itemValue = new ParametersItemField(field);
            appendRow({itemName, itemValue});

        }
    }
}

//        QModelIndex index = m_model->indexFromItem(itemValue);
//        openPersistentEditor(index);
