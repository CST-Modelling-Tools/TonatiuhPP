#pragma once

#include <QObject>
//#include <QScriptable>
#include <QJSValue>
#include <Inventor/nodes/SoNode.h>


class MainWindow;


class NodeObject: public QObject//, protected QScriptable
{
    Q_OBJECT

public:
    Q_INVOKABLE NodeObject(QObject* parent = 0);
    NodeObject(SoNode* node);

    SoNode* getNode() const {return m_node;}

    static void setMainWindow(MainWindow* w) {s_mainWindow = w;}
    static void setEngine(QJSEngine* engine) {s_engine = engine;}

public slots:
    static QJSValue getScene();
    static QJSValue getRoot();

    QJSValue createNode(const QString& name = "");
    QJSValue createShape();
    QJSValue createTracker();

    QJSValue getPart(const QString& name = "");
    void setPart(const QString& name, QJSValue node);

    QJSValue insertSurface(const QString& name = "");
    QJSValue insertProfile(const QString& name = "");
    QJSValue insertMaterial(const QString& name = "");
    QJSValue insertArmature(const QString& name = "");

    void setName(const QString& name);
    void setParameter(const QString& name, const QString& value);

    static QJSValue FindInterception(QJSValue surface, QJSValue rays);

private:
    SoNode* m_node;
    static MainWindow* s_mainWindow;
    static QJSEngine* s_engine;
};

