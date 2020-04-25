#include "ActionInsertShape.h"

#include "kernel/raytracing/TShapeFactory.h"


ActionInsertShape::ActionInsertShape(TShapeFactory* factory, QObject* parent):
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

void ActionInsertShape::onTriggered()
{
    emit CreateShape(m_factory);
}
