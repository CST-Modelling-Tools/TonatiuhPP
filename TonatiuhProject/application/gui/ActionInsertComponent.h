#pragma once

#include <QAction>

class TComponentFactory;

class ActionInsertComponent : public QAction
{
    Q_OBJECT

public:
    ActionInsertComponent( const QString& text, QObject* parent, TComponentFactory* pTComponentFactory );
    ~ActionInsertComponent();

signals:
    void CreateComponent( TComponentFactory* pTComponentFactory );

public slots:
    void OnActionInsertComponentTriggered();

private:
    TComponentFactory* m_pTComponentFactory;

};
