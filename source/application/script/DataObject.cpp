#include "DataObject.h"

#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>
#include <QJSEngine>
#include <QString>
#include <QStringList>


QJSEngine* DataObject::s_engine = 0;

DataObject::DataObject(QObject* parent):
    QObject(parent)
{

}

QJSValue DataObject::read(const QString& fileName)
{
    m_data.clear();

    QFileInfo info = QString("project:") + fileName;
    if (!info.exists()) {
        QMessageBox::warning(0, "Warning", QString("File not found:\n") + fileName);
        return false;
    }

    QFile file(info.absoluteFilePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
          QMessageBox::warning(0, "Warning", QString("File cannot be opened:\n") + fileName);
        return false;
    }

    QTextStream in(&file);
    QString line;
    while (in.readLineInto(&line)) {
        m_data << line;
    }
    return true;
}

QJSValue DataObject::write(const QString& fileName)
{
    QDir dir = QDir::searchPaths("project")[0];
    QFile file(dir.absoluteFilePath(fileName));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
          QMessageBox::warning(0, "Warning", QString("File cannot be opened:\n") + fileName);
        return false;
    }

    QTextStream out(&file);
    for (QString& line : m_data) {
        out << line << Qt::endl;
    }
    return true;
}

void DataObject::clear()
{
    m_data.clear();
}

QJSValue DataObject::rows()
{
    return m_data.size();
}

QJSValue DataObject::row(int n)
{
    if (n < 0 || n >= m_data.size()) return "";
    return m_data[n];
}

void DataObject::setRow(int n, const QString& line)
{
    if (n < 0 || n >= m_data.size()) return;
    m_data[n] = line;
}

void DataObject::addRow(const QString& line)
{
    m_data << line;
}

QJSValue DataObject::array(int n)
{
    if (n < 0 || n >= m_data.size()) return "";
    QStringList list = m_data[n].split(',');
    int iMax = list.size();

    QJSValue jsArray = s_engine->newArray(iMax);
    for (int i = 0; i < iMax; ++i)
        jsArray.setProperty(i, list[i]);
    return jsArray;
}

void DataObject::setArray(int n, QJSValue value)
{
    if (n < 0 || n >= m_data.size()) return;

    QStringList list;
    const int iMax = value.property("length").toInt();
    for (int i = 0; i < iMax; ++i)
        list << value.property(i).toString();

    m_data[n] = list.join(',');
}

void DataObject::addArray(QJSValue value)
{
    QStringList list;
    const int iMax = value.property("length").toInt();
    for (int i = 0; i < iMax; ++i)
        list << value.property(i).toString();

    m_data << list.join(',');
}

//QJSValue DataCSV::part(int n, int m)
//{
//    if (n < 0 || n >= m_data.size()) return "";
//    QVector<QStringRef> refs = m_data[n].splitRef(',');
//    if (m < 0 || m >= refs.size()) return "";
//    return refs[m].toString();
//}
