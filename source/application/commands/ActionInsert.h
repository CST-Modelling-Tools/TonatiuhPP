#pragma once

#include <QAction>

class TFactory;
class ComponentFactory;


//!  ActionInsertShape class is the action to insert a shape in the scene.
/*!
   ActionInsertShape is the action added to menu and toolbar for each shape plugin.
 */
class ActionInsert: public QAction
{
    Q_OBJECT

public:
    ActionInsert(TFactory* factory, QObject* parent);

public slots:
    void onTriggered();

signals:
    void InsertComponent(ComponentFactory* factory);

private:
    TFactory* m_factory;
};
