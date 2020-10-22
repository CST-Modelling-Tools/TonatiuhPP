#pragma once

#include <QObject>
#include <QScriptable>
#include <QScriptValue>
#include <Inventor/nodes/SoNode.h>


class MainWindow;


class NodeObject: public QObject, protected QScriptable
{
    Q_OBJECT

public:
    explicit NodeObject(QObject* parent = 0);
    NodeObject(SoNode* node);

    SoNode* getNode() const {return m_node;}

    static void setMainWindow(MainWindow* w) {s_mainWindow = w;}
    static void setEngine(QScriptEngine* engine) {s_engine = engine;}

public slots:
    static QScriptValue getScene();
    static QScriptValue getRoot();

    QScriptValue createNode(const QString& name = "");
    QScriptValue createShape();
    QScriptValue createTracker();

    QScriptValue getPart(const QString& name = "");
    void setPart(const QString& name, QScriptValue node);

    QScriptValue insertSurface(const QString& name = "");
    QScriptValue insertProfile(const QString& name = "");
    QScriptValue insertMaterial(const QString& name = "");
    QScriptValue insertArmature(const QString& name = "");

    void setName(const QString& name);
    void setParameter(const QString& name, const QString& value);

    static QScriptValue FindInterception(QScriptValue surface, QScriptValue rays);

private:
    SoNode* m_node;
    static MainWindow* s_mainWindow;
    static QScriptEngine* s_engine;
};

