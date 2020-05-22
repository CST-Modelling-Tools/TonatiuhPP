#include "ActionInsertMaterial.h"

#include "kernel/material/MaterialRT.h"


ActionInsertMaterial::ActionInsertMaterial(MaterialFactory* factory, QObject* parent):
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

void ActionInsertMaterial::onTriggered()
{
    emit CreateMaterial(m_factory);
}
