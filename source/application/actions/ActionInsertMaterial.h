#pragma once

#include <QAction>

class TMaterialFactory;


//!  ActionInsertMaterial class is the action to insert material in the scene.
/*!
  ActionInsertMaterial is the action added to menu and toolbar for each material plugin.
*/
class ActionInsertMaterial: public QAction
{
    Q_OBJECT

public:
    ActionInsertMaterial(TMaterialFactory* factory, QObject* parent);

public slots:
    void onTriggered();

signals:
    void CreateMaterial(TMaterialFactory* factory);

private:
    TMaterialFactory* m_factory;
};
