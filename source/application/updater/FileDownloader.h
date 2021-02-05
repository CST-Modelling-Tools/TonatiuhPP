#pragma once

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class FileDownloader : public QObject
{
    Q_OBJECT

public:
    explicit FileDownloader(QUrl imageUrl, QObject* parent = 0);
    virtual ~FileDownloader() {}

    QString status() const {return m_status;}
    QByteArray downloadedData() const {return m_data;}

signals:
    void downloaded();

private slots:
    void fileDownloaded(QNetworkReply* pReply);

private:
    QNetworkAccessManager m_manager;
    QString m_status;
    QByteArray m_data;
};
