#include "ParametersTabs.h"

#include <Inventor/nodes/SoNode.h>
#include <Inventor/SbName.h>

#include "ParametersModel.h"
#include "ParametersView.h"


ParametersTabs::ParametersTabs(QWidget* parent):
    QTabWidget(parent),
    m_node(0)
{  
    m_model = new ParametersModel(this);

    ParametersView* view = new ParametersView(this);
    view->setModel(m_model);

    addTab(view, "");

    connect(
        m_model, SIGNAL(valueModified(SoNode*, QString, QString)),
        this, SLOT(setValue(SoNode*, QString, QString))
    );

    setStyleSheet(R"(
QTabWidget::pane {
border:none;
}
    )");
}

void ParametersTabs::setMain(MainWindow* main)
{
    m_model->setMain(main);
}

void ParametersTabs::setNode(SoNode* node)
{
    if (node == m_node) return;
    m_node = node;
    m_model->setNode(node);

    QString name = node->getName().getString();
    if (name.isEmpty())
        name = node->getTypeId().getName().getString();

    setTabText(0, name);
}

void ParametersTabs::setValue(SoNode* node, QString field, QString value)
{
    emit valueModified(node, field, value);
}

//    QList<QWidget*> list;
//    for (int n = 0; n < count(); ++n)
//        list << widget(n);
//    clear();
//    qDeleteAll(list);
