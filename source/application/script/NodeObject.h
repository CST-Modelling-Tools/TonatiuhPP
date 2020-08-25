#pragma once

#include <QObject>
#include <QScriptable>
#include <QScriptValue>
#include <Inventor/nodes/SoNode.h>

class PluginManager;


class NodeObject: public QObject, protected QScriptable
{
    Q_OBJECT

public:
    explicit NodeObject(QObject* parent = 0);
    NodeObject(SoNode* node);

    SoNode* getNode() const {return m_node;}

    static void setPlugins(PluginManager* plugins) {m_plugins = plugins;}

public slots:
    QScriptValue createNode(const QString& name = "");
    QScriptValue createShape();
    QScriptValue getPart(const QString& name = "");

    QScriptValue insertSurface(const QString& name = "");
    QScriptValue insertProfile(const QString& name = "");
    QScriptValue insertMaterial(const QString& name = "");
    QScriptValue insertTracker(const QString& name = "");

    void update();

    void setName(const QString& name);
    void setParameter(const QString& name, const QString& value);

private:
    SoNode* m_node;
    static PluginManager* m_plugins;
};

