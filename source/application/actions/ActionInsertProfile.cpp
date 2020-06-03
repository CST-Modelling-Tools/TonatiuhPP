#include "ActionInsertProfile.h"

#include "kernel/profiles/ProfileRT.h"


ActionInsertProfile::ActionInsertProfile(ProfileFactory* factory, QObject* parent):
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

void ActionInsertProfile::onTriggered()
{
    emit CreateProfile(m_factory);
}
