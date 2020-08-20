#include <QString>

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoGroup.h>

#include "ParametersModel.h"
#include "ParametersView.h"
#include "ParametersTabs.h"


ParametersTabs::ParametersTabs(QWidget* parent):
    QTabWidget(parent)
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

    ParametersView* view = new ParametersView(this);
    ParametersModel* model = new ParametersModel(view);
    model->setNode(node);
    view->setModel(model);

    QString name = node->getName().getString();
    if (name.isEmpty())
        name = node->getTypeId().getName().getString();
    addTab(view, name);

    connect(
        model, SIGNAL(valueModified(SoNode*, QString, QString)),
        this, SLOT(setValue(SoNode*, QString, QString))
    );
}

void ParametersTabs::updateNode()
{
    setNode(m_node);
}

void ParametersTabs::setValue(SoNode* node, QString field, QString value)
{
    emit valueModified(node, field, value);
}
