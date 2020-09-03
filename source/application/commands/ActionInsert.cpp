#include "ActionInsert.h"

#include "kernel/component/ComponentFactory.h"


ActionInsert::ActionInsert(TFactory* factory, QObject* parent): // todo remove
    QAction(parent),
    m_factory(factory)
{
    setText(factory->name());
    setIcon(factory->icon());

    connect(
        this, SIGNAL(triggered()),
        this, SLOT(onTriggered())
    );
}

void ActionInsert::onTriggered()
{
    if (ComponentFactory* component = dynamic_cast<ComponentFactory*>(m_factory))
        emit InsertComponent(component);
}
