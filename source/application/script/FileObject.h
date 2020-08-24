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
    QScriptValue readCSV(const QString& name = "");
    QScriptValue rows();
    QScriptValue part(int n, int m);

private:
    QStringList m_data;
};

