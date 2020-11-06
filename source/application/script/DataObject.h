#pragma once

#include <QObject>
#include <QScriptable>
#include <QScriptValue>


class DataObject: public QObject, protected QScriptable
{
    Q_OBJECT

public:
    explicit DataObject(QObject* parent = 0);

public slots:
    QScriptValue read(const QString& fileName = "");
    QScriptValue write(const QString& fileName = "");
    void clear();

    QScriptValue rows();
    QScriptValue row(int n);
    void setRow(int n, const QString& line);
    void addRow(const QString& line);

    QScriptValue array(int n);
    void setArray(int n, QScriptValue value);
    void addArray(QScriptValue value);

//    QScriptValue part(int n, int m);

private:
    QStringList m_data;
};

