#pragma once

#include <QAction>

class TrackerFactory;

//! ActionInsertTracker class is the action to insert a tracker in the scene.
/*!
  ActionInsertTracker is the action added to menu and toolbar for each tracker plugin.
*/

class ActionInsertTracker : public QAction
{
    Q_OBJECT

public:
    ActionInsertTracker( const QString& text, QObject* parent, TrackerFactory* pTrackerFactory );
    ~ActionInsertTracker();
    
signals:
    void CreateTracker( TrackerFactory* pTrackerFactory );
    
public slots:
    void OnActionInsertTrackerTriggered();
    
private:
    TrackerFactory* m_pTrackerFactory;
    
};
