#include "ActionInsert.h"

#include "kernel/shape/ShapeRT.h"
#include "kernel/profiles/ProfileRT.h"
#include "kernel/material/MaterialRT.h"
#include "kernel/trackers/Tracker.h"
#include "kernel/component/ComponentFactory.h"


ActionInsert::ActionInsert(TFactory* factory, QObject* parent):
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
    if (ShapeFactory* shape = dynamic_cast<ShapeFactory*>(m_factory))
        emit CreateShape(shape);
    else if (ProfileFactory* profile = dynamic_cast<ProfileFactory*>(m_factory))
        emit CreateProfile(profile);
    else if (MaterialFactory* material = dynamic_cast<MaterialFactory*>(m_factory))
        emit CreateMaterial(material);
    else if (TrackerFactory* tracker = dynamic_cast<TrackerFactory*>(m_factory))
        emit CreateTracker(tracker);
    else if (ComponentFactory* component = dynamic_cast<ComponentFactory*>(m_factory))
        emit CreateComponent(component);
}
