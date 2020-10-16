#include "FileObject.h"

#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QMessageBox>

QDir FileObject::s_dir;

FileObject::FileObject(QObject* parent):
    QObject(parent)
{

}

QScriptValue FileObject::readCSV(const QString& name)
{
    m_data.clear();

    QFile file;
    file.setFileName(s_dir.filePath(name));
    if (!file.exists()) {
        QDir dir(qApp->applicationDirPath());
        file.setFileName(dir.filePath(name));
        if (!file.exists()) return false;
    }
//    QMessageBox::information(0, "info", file.fileName());

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        m_data << line;
    }
    return true;
}

QScriptValue FileObject::rows()
{
    return m_data.size();
}

QScriptValue FileObject::part(int n, int m)
{
    if (n < 0 || n >= m_data.size()) return "";
    QVector<QStringRef> refs = m_data[n].splitRef(',');
    if (m < 0 || m >= refs.size()) return "";
    return refs[m].toString();
}
