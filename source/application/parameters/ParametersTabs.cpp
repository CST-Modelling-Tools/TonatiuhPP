#include "ParametersTabs.h"

#include <Inventor/nodes/SoNode.h>
#include <Inventor/SbName.h>


ParametersTabs::ParametersTabs(QWidget* parent):
    QTabWidget(parent)
{  
    m_view = new ParametersView(this);
    addTab(m_view, "");

    setStyleSheet(R"(
QTabWidget::pane {
border:none;
}
    )");
}

void ParametersTabs::setNode(SoNode* node)
{
    m_view->getModel()->setNode(node);

    QString name = node->getName().getString();
    if (name.isEmpty())
        name = node->getTypeId().getName().getString();
    setTabText(0, name);
}
