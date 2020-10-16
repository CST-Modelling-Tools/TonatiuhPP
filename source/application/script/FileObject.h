#pragma once

#include <QObject>
#include <QScriptable>
#include <QScriptValue>
#include <QDir>

class FileObject: public QObject, protected QScriptable
{
    Q_OBJECT

public:
    explicit FileObject(QObject* parent = 0);
    static void setDir(QDir dir) {s_dir = dir;}

public slots:
    QScriptValue readCSV(const QString& name = "");
    QScriptValue rows();
    QScriptValue part(int n, int m);

private:
    static QDir s_dir;
    QStringList m_data;
};

