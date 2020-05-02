#pragma once

#include <QAction>

class TrackerFactory;

//! ActionInsertTracker class is the action to insert a tracker in the scene.
/*!
  ActionInsertTracker is the action added to menu and toolbar for each tracker plugin.
*/

class ActionInsertTracker: public QAction
{
    Q_OBJECT

public:
    ActionInsertTracker(TrackerFactory* factory, QObject* parent);
       
public slots:
    void onTriggered();

signals:
    void CreateTracker(TrackerFactory* factory);
    
private:
    TrackerFactory* m_factory;
};
