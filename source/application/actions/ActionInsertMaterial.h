#pragma once

#include <QAction>

class MaterialFactory;


//!  ActionInsertMaterial class is the action to insert material in the scene.
/*!
  ActionInsertMaterial is the action added to menu and toolbar for each material plugin.
*/
class ActionInsertMaterial: public QAction
{
    Q_OBJECT

public:
    ActionInsertMaterial(MaterialFactory* factory, QObject* parent);

public slots:
    void onTriggered();

signals:
    void CreateMaterial(MaterialFactory* factory);

private:
    MaterialFactory* m_factory;
};
