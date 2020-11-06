#pragma once

#include <QObject>
#include <QScriptable>
#include <QScriptValue>


class FileObject: public QObject, protected QScriptable
{
    Q_OBJECT

public:
    explicit FileObject(QObject* parent = 0);

public slots:
    QScriptValue readCSV(const QString& fileName = "");
    QScriptValue writeCSV(const QString& fileName = "");
    void clear();

    QScriptValue rows();
    QScriptValue row(int n);
    void setRow(int n, const QString& line);
    void addRow(const QString& line);

    QScriptValue array(int n);
    void setArray(int n, QScriptValue value);
    void addArray(QScriptValue value);

    QScriptValue part(int n, int m);

private:
    QStringList m_data;
};

