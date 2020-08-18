#include <QString>

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoGroup.h>

#include "ParametersModel.h"
#include "ParametersView.h"
#include "ParametersTabs.h"

/**
 * Creates a new ParametersView with parent \a parent.
 */
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

/*!
 * Changes the parameters view to show \a coinNode \a parts parameters.
 */
void ParametersTabs::SelectionChanged(SoNode* node)
{
    if (node == m_node) return;
//    QList<QWidget*> list;
//    for (int n = 0; n < count(); ++n)
//        list << widget(n);
    clear();
//    qDeleteAll(list);

    m_node = node;
    SoBaseKit* kit = dynamic_cast<SoBaseKit*>(m_node);
    if (!kit)
        addTabNode(node, "");

    for (QString part : ContainerNodeParts(kit))
    {
        if (SoNode* node = kit->getPart(part.toStdString().c_str(), false))
        {
            addTabNode(node, part);
        }
        else if (SoField* field = kit->getField(part.toStdString().c_str()))
        {
            if (SoSFNode* fn = dynamic_cast<SoSFNode*>(field))
                addTabNode(fn->getValue(), part);
        }
        else if (part[part.size() - 1] == '*')
        {
            QString partX = part.left(part.size() - 1);
            SoGroup* parentGroup = static_cast<SoGroup*>(kit->getPart(partX.toStdString().c_str(), false));
            if (!parentGroup) continue;
            int nMax = std::min(parentGroup->getNumChildren(), 10);
            for (int n = 0; n < nMax; n++)
            {
                SoNode* element = (SoNode*) parentGroup->getChild(n);
                if (element) addTabNode(element, "");
            }
        }
    }
}

void ParametersTabs::UpdateView()
{
    SelectionChanged(m_node);
}

/*!
 * Emits a signal with \a node as the actual node, \a paramenterName and \a newValue.
 */
void ParametersTabs::setValue(SoNode* node, QString field, QString value)
{
    emit valueModified(node, field, value);
}

/*!
 * Adds a new tab to the view with \a coinNode \a partName parameters.
 */
void ParametersTabs::addTabNode(SoNode* node, QString /*partName*/)
{
    QString name = node->getName().getString();
    if (name.length() <= 0)
        name = node->getTypeId().getName().getString();

    ParametersView* view = new ParametersView(this);
    ParametersModel* model = new ParametersModel(view);
    model->setNode(node);
    view->setModel(model);
    addTab(view, name);
    connect(
        model, SIGNAL(valueModified(SoNode*, QString, QString)),
        this, SLOT(setValue(SoNode*, QString, QString))
    );
}

/*!
 * Returns the names of the parts of the \a coinNode that the view shows.
 *
 * If the \a coinNode is not a container node, return a empty list.
 */
QStringList ParametersTabs::ContainerNodeParts(SoBaseKit* kit)
{
    if (!kit) return {};

    QString type = kit->getTypeId().getName().getString();

    if (type == "TSeparatorKit")
        return {"transform"};
    else if (type == "TShapeKit")
        return {"shapeRT", "profileRT", "materialRT", "material"};
    else if (type == "SunKit")
        return {"position", "shape", "aperture"};
    else if (type == "AirKit")
        return {"transmission"};
    else if (type == "TerrainKit")
        return {"grid"};
    else
        return {};
}
