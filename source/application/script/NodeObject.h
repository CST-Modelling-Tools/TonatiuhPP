#pragma once

#include <QObject>
#include <QScriptable>
#include <QScriptValue>
#include <Inventor/nodes/SoNode.h>


class NodeObject: public QObject, protected QScriptable
{
    Q_OBJECT
public:
    explicit NodeObject(QObject* parent = 0);
    NodeObject(SoNode* node);

    SoNode* getNode() const {return m_node;}

public slots:
    QScriptValue createNode();
    QScriptValue createShape();
    void setName(const QString& name);
    void setParameter(const QString& name, const QString& value);

private:
    SoNode* m_node;
};

