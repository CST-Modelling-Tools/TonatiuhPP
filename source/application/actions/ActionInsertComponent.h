#pragma once

#include <QAction>

class ComponentFactory;

class ActionInsertComponent: public QAction
{
    Q_OBJECT

public:
    ActionInsertComponent(const QString& text, QObject* parent, ComponentFactory* pComponentFactory);
    ~ActionInsertComponent();

signals:
    void CreateComponent(ComponentFactory* pComponentFactory);

public slots:
    void OnActionInsertComponentTriggered();

private:
    ComponentFactory* m_pComponentFactory;
};
