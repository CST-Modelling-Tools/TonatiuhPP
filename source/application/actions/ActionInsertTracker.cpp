#include "ActionInsertTracker.h"

#include "kernel/trackers/TrackerFactory.h"


ActionInsertTracker::ActionInsertTracker(TrackerFactory* factory, QObject* parent):
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

void ActionInsertTracker::onTriggered()
{
    emit CreateTracker(m_factory);
}
