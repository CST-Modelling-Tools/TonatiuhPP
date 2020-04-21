#pragma once
#include <QAction>

class TMaterialFactory;

//!  ActionInsertMaterial class is the action to insert material in the scene.
/*!
  ActionInsertMaterial is the action added to menu and toolbar for each material plugin.
*/

class ActionInsertMaterial : public QAction
{
    Q_OBJECT

public:
    ActionInsertMaterial( const QString& text, QObject* parent, TMaterialFactory* pTMaterialFactory );
    ~ActionInsertMaterial();

signals:
    void CreateMaterial( TMaterialFactory* pTMaterialFactory );

public slots:
    void OnActionInsertMaterialTriggered();

private:
    TMaterialFactory* m_pTMaterialFactory;

};
