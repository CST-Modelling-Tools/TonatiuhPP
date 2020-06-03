#pragma once

#include <QAction>

class ProfileFactory;


//!  ActionInsertProfile class is the action to insert a shape in the scene.
/*!
   ActionInsertProfile is the action added to menu and toolbar for each shape plugin.
 */
class ActionInsertProfile: public QAction
{
    Q_OBJECT

public:
    ActionInsertProfile(ProfileFactory* factory, QObject* parent);

public slots:
    void onTriggered();

signals:
    void CreateProfile(ProfileFactory* factory);

private:
    ProfileFactory* m_factory;
};
