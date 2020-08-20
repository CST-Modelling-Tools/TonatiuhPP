#include <QString>

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoGroup.h>

#include "ParametersModel.h"
#include "ParametersView.h"
#include "ParametersTabs.h"


ParametersTabs::ParametersTabs(QWidget* parent):
    QTabWidget(parent),
    m_node(0)
{
    setStyleSheet(R"(
QTabWidget::pane {
border:none;
}
    )");

}

void ParametersTabs::setNode(SoNode* node)
{
    if (node == m_node) return;
    m_node = node;

//    QList<QWidget*> list;
//    for (int n = 0; n < count(); ++n)
//        list << widget(n);
    clear();
//    qDeleteAll(list);

//    SoBaseKit* kit = dynamic_cast<SoBaseKit*>(m_node);
//    if (!kit) {
        addTabNode(node, "");
//        return;
//    }

//    QString type = kit->getTypeId().getName().getString();
//    QStringList parts;
//    if (type == "TSeparatorKit")
//        parts << "transform";
//    else if (type == "TShapeKit")
//        parts << "shapeRT" << "profileRT" << "materialRT" << "material";
//    else if (type == "SunKit")
//        parts << "position" << "shape" << "aperture";
//    else if (type == "AirKit")
//        parts << "transmission";
//    else if (type == "TerrainKit")
//        parts << "grid";

//    for (QString part : parts)
//    {
//        if (SoNode* node = kit->getPart(part.toStdString().c_str(), false))
//        {
//            addTabNode(node, part);
//        }
//        else if (SoField* field = kit->getField(part.toStdString().c_str()))
//        {
//            if (SoSFNode* fn = dynamic_cast<SoSFNode*>(field))
//                addTabNode(fn->getValue(), part);
//        }
//        else if (part[part.size() - 1] == '*')
//        {
//            QString partX = part.left(part.size() - 1);
//            SoGroup* group = static_cast<SoGroup*>(kit->getPart(partX.toStdString().c_str(), false));
//            if (!group) continue;
//            int nMax = std::min(group->getNumChildren(), 10);
//            for (int n = 0; n < nMax; n++)
//            {
//                SoNode* node = (SoNode*) group->getChild(n);
//                if (node) addTabNode(node, "");
//            }
//        }
//    }
}

void ParametersTabs::updateNode()
{
    setNode(m_node);
}

void ParametersTabs::setValue(SoNode* node, QString field, QString value)
{
    emit valueModified(node, field, value);
}

void ParametersTabs::addTabNode(SoNode* node, QString /*partName*/)
{
    ParametersView* view = new ParametersView(this);
    ParametersModel* model = new ParametersModel(view);
    model->setNode(node);
    view->setModel(model);
    view->expandToDepth(1);

    QString name = node->getName().getString();
    if (name.isEmpty())
        name = node->getTypeId().getName().getString();
    addTab(view, name);

    connect(
        model, SIGNAL(valueModified(SoNode*, QString, QString)),
        this, SLOT(setValue(SoNode*, QString, QString))
    );
}
