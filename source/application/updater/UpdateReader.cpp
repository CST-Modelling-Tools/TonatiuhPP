#include "UpdateReader.h"

#include <QApplication>
#include <QXmlStreamReader>


UpdateReader::UpdateReader()
{
    m_versionCurrent = toVersion(qApp->applicationVersion());
}

bool UpdateReader::checkUpdates(const QString& data)
{
    try
    {
        QXmlStreamReader xml(data);
//        qDebug() << data;

        xml.readNextStartElement();
        if (xml.name().toString() != "Updates")
            throw QString("Updates tag expected");

        xml.readNextStartElement();
        if (xml.name().toString() != "Release")
            throw QString("Release tag expected");

        m_date = QDate::fromString(xml.attributes().value("date").toString(), "yyyy-M-d");
        m_version = toVersion(xml.attributes().value("version").toString());
        m_path = xml.attributes().value("path").toString();

        if (xml.hasError())
            throw QString("XML error");
    }
    catch (QString m) {
        m_message = m;
        return false;
    }

    return true;
}

bool UpdateReader::isNewer()
{
    return isGrowing(m_versionCurrent, m_version);
}

QList<int> UpdateReader::toVersion(const QString& version) const
{
    QList<int> ans;
    for (QString s : version.split("."))
        ans << s.toInt();
    for (int n = ans.size(); n < 4; ++n)
        ans << 0;
    return ans;
}

bool UpdateReader::isGrowing(const QList<int>& a, const QList<int>& b) const
{
    for (int i = 0; i < a.size(); ++i)
        if (a[i] < b[i]) return true;
    return false;
}

