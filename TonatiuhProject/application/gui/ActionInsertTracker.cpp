#include "ActionInsertTracker.h"

/**
 * Constructor.
 */
ActionInsertTracker::ActionInsertTracker( const QString& text, QObject* parent, TTrackerFactory* pTTrackerFactory )
: QAction(text,parent), m_pTTrackerFactory(pTTrackerFactory)
{
}

/**
 * Destructor.
 */
ActionInsertTracker::~ActionInsertTracker()
{
}

/**
 * Emits a create tracker signal.
 */
void ActionInsertTracker::OnActionInsertTrackerTriggered()
{
    emit CreateTracker( m_pTTrackerFactory );
}
