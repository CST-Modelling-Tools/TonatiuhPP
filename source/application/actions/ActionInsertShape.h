#pragma once

#include <QAction>

class TShapeFactory;


//!  ActionInsertShape class is the action to insert a shape in the scene.
/*!
   ActionInsertShape is the action added to menu and toolbar for each shape plugin.
 */
class ActionInsertShape: public QAction
{
    Q_OBJECT

public:
    ActionInsertShape(TShapeFactory* factory, QObject* parent);

public slots:
    void onTriggered();

signals:
    void CreateShape(TShapeFactory* factory);

private:
    TShapeFactory* m_factory;
};
