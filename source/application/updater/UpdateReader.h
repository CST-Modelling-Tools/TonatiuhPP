#pragma once

#include <QDate>

class UpdateReader
{
public:
    UpdateReader();

    bool checkUpdates(const QString& data);
    bool isNewer();

    QList<int> m_versionCurrent;

    QDate m_date;
    QList<int> m_version;
    QString m_path;

    QString m_message;

protected:
    QList<int> toVersion(const QString& version) const;
    bool isGrowing(const QList<int>& a, const QList<int>& b) const;
};

