#include "ActionInsertTracker.h"

/**
 * Constructor.
 */
ActionInsertTracker::ActionInsertTracker( const QString& text, QObject* parent, TrackerFactory* pTrackerFactory )
: QAction(text,parent), m_pTrackerFactory(pTrackerFactory)
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
    emit CreateTracker( m_pTrackerFactory );
}
