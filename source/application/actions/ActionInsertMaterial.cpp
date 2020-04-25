#include "ActionInsertMaterial.h"

#include "kernel/raytracing/TMaterialFactory.h"


ActionInsertMaterial::ActionInsertMaterial(TMaterialFactory* factory, QObject* parent):
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
