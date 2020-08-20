#include "ParametersModel.h"

#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/lists/SoFieldList.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoGroup.h>


#include "ParametersItem.h"
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
    QStandardItem* parent = invisibleRootItem();

    SoBaseKit* kit = dynamic_cast<SoBaseKit*>(node);
    if (!kit) {
        parent->appendRow(new ParametersItemNode(node));
    } else {
        QString type = kit->getTypeId().getName().getString();
        QStringList parts;
        if (type == "TSeparatorKit")
            parts << "transform";
        else if (type == "TShapeKit")
            parts << "shapeRT" << "profileRT" << "materialRT" << "material";
        else if (type == "SunKit")
            parts << "position" << "shape" << "aperture";
        else if (type == "AirKit")
            parts << "transmission";
        else if (type == "TerrainKit")
            parts << "grid";

        for (QString part : parts)
        {
            if (SoNode* node = kit->getPart(part.toStdString().c_str(), false))
            {
                parent->appendRow(new ParametersItemNode(node));
            }
            else if (SoField* field = kit->getField(part.toStdString().c_str()))
            {
                if (SoSFNode* fn = dynamic_cast<SoSFNode*>(field))
                    parent->appendRow(new ParametersItemNode(fn->getValue()));
            }
            else if (part[part.size() - 1] == '*')
            {
                QString partX = part.left(part.size() - 1);
                SoGroup* group = static_cast<SoGroup*>(kit->getPart(partX.toStdString().c_str(), false));
                if (!group) continue;
                int nMax = std::min(group->getNumChildren(), 10);
                for (int n = 0; n < nMax; n++)
                {
                    SoNode* node = (SoNode*) group->getChild(n);
                    if (node)
                        parent->appendRow(new ParametersItemNode(node));
                }
            }
        }
    }

    endResetModel();
}

bool ParametersModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    ParametersItem* item = dynamic_cast<ParametersItem*>(itemFromIndex(index));
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

//    if (item->isCheckable()) {
//        ParametersItem* pitem = (ParametersItem*) item;
//        SoSFBool* f = dynamic_cast<SoSFBool*>(pitem->getField());
//        if (!f) return;
//        bool on = item->checkState() == Qt::Checked;
//        if (f->getValue() == on) return;
//    }

