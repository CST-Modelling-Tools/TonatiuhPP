#include <QString>

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoGroup.h>

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
    clear();

    m_node = node;
    SoBaseKit* kit = dynamic_cast<SoBaseKit*>(m_node);
    if (!kit)
        AddTab(node, "");

    for (QString part : ContainerNodeParts(kit))
    {
        if (SoNode* node = kit->getPart(part.toStdString().c_str(), false))
        {
            AddTab(node, part);
        }
        else if (SoField* field = kit->getField(part.toStdString().c_str()))
        {
            if (SoSFNode* fn = dynamic_cast<SoSFNode*>(field))
                AddTab(fn->getValue(), part);
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
                if (element) AddTab(element, "");
            }
        }
    }
}

/*!
 * Updates selected node parametes views.
 */
void ParametersTabs::UpdateView()
{
    SelectionChanged(m_node);
}

/*!
 * Emits a valueModified signal with \a node as the actual node, \a paramenterName and \a newValue.
 */
void ParametersTabs::SetValue(SoNode* node, QString parameter, QString value)
{
    emit valueModified(node, parameter, value);
}

/*!
 * Adds a new tab to the view with \a coinNode \a partName parameters.
 */
void ParametersTabs::AddTab(SoNode* node, QString /*partName*/)
{
    QString name = node->getName().getString();
    if (name.length() <= 0)
        name = node->getTypeId().getName().getString();

    ParametersView* view = new ParametersView(this);
    view->SetContainer(node);
    addTab(view, name);
    connect(
        view, SIGNAL(valueModified(SoNode*, QString, QString)),
        this, SLOT(SetValue(SoNode*, QString, QString))
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
