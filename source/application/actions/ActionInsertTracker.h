#pragma once

#include <QAction>

class TTrackerFactory;

//! ActionInsertTracker class is the action to insert a tracker in the scene.
/*!
  ActionInsertTracker is the action added to menu and toolbar for each tracker plugin.
*/

class ActionInsertTracker : public QAction
{
    Q_OBJECT

public:
    ActionInsertTracker( const QString& text, QObject* parent, TTrackerFactory* pTTrackerFactory );
    ~ActionInsertTracker();
    
signals:
    void CreateTracker( TTrackerFactory* pTTrackerFactory );
    
public slots:
    void OnActionInsertTrackerTriggered();
    
private:
    TTrackerFactory* m_pTTrackerFactory;
    
};
