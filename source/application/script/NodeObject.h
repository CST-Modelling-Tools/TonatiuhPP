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

    static void setMainWindow(MainWindow* w) {m_mainWindow = w;}

public slots:
    QScriptValue getScene();
    QScriptValue getRoot();

    QScriptValue createNode(const QString& name = "");
    QScriptValue createShape();
    QScriptValue createTracker();
    QScriptValue getPart(const QString& name = "");

    QScriptValue insertSurface(const QString& name = "");
    QScriptValue insertProfile(const QString& name = "");
    QScriptValue insertMaterial(const QString& name = "");
    QScriptValue insertArmature(const QString& name = "");

    void setName(const QString& name);
    void setParameter(const QString& name, const QString& value);

private:
    SoNode* m_node;
    static MainWindow* m_mainWindow;
};

