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
    ActionInsertShape(const QString& text, QObject* parent, TShapeFactory* pTShapeFactory);
    ~ActionInsertShape() {}

signals:
    void CreateShape(TShapeFactory* pTShapeFactory);

public slots:
    void OnActionInsertShapeTriggered();

private:
    TShapeFactory* m_pTShapeFactory;
};
