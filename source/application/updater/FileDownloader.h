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
    explicit FileDownloader(QUrl url, QObject* parent = 0);
    virtual ~FileDownloader() {}

    QString status() const {return m_status;}
    QByteArray downloadedData() const {return m_data;}
    void abort();

signals:
    void downloaded();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);


private slots:
    void fileDownloaded(QNetworkReply* pReply);
    void updateProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    QNetworkAccessManager m_manager;
    QString m_status;
    QByteArray m_data;

    QNetworkReply* m_reply;
};
