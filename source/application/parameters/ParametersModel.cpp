#include "ParametersModel.h"

#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/lists/SoFieldList.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoGroup.h>

#include "ParametersItemField.h"
#include "ParametersItemNode.h"


ParametersModel::ParametersModel(QObject* parent):
    QStandardItemModel(parent)
{

}

void ParametersModel::setNode(SoNode* node)
{
    beginResetModel();
    clear();
    setHorizontalHeaderLabels({"Parameter", "Value"});

    QString name = node->getName().getString();
    ParametersItemNode* itemNode = new ParametersItemNode(name, node);

    QString nameType = node->getTypeId().getName().getString();
    QStandardItem* itemType = new QStandardItem(nameType);
    itemType->setEditable(false);

    QStandardItem* parent = invisibleRootItem();
    parent->appendRow({itemNode, itemType});

    endResetModel();
}

bool ParametersModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    ParametersItemField* item = dynamic_cast<ParametersItemField*>(itemFromIndex(index));
    if (!item)
        return QStandardItemModel::setData(index, value, role);
    else {
        item->setData(value, role);

        ParametersItemNode* itemNode = (ParametersItemNode*) itemFromIndex(index.parent());
        SoNode* node = itemNode->node();
        SbName name;
        node->getFieldName(item->field(), name);
        emit valueModified(node, name.getString(), value.toString());

        return true;
    }
}



//            else if (part[part.size() - 1] == '*')
//            {
//                QString partX = part.left(part.size() - 1);
//                SoGroup* group = static_cast<SoGroup*>(kit->getPart(partX.toStdString().c_str(), false));
//                if (!group) continue;
//                int nMax = std::min(group->getNumChildren(), 10);
//                for (int n = 0; n < nMax; n++)
//                {
//                    SoNode* node = (SoNode*) group->getChild(n);
//                    if (node)
//                        parent->appendRow(new ParametersItemNode(node));
//                }
//            }

//    if (item->isCheckable()) {
//        ParametersItem* pitem = (ParametersItem*) item;
//        SoSFBool* f = dynamic_cast<SoSFBool*>(pitem->getField());
//        if (!f) return;
//        bool on = item->checkState() == Qt::Checked;
//        if (f->getValue() == on) return;
//    }
