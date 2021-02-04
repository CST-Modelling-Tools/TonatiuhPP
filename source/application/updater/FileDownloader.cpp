#include "FileDownloader.h"

FileDownloader::FileDownloader(QUrl imageUrl, QObject* parent):
    QObject(parent)
{
    connect(
        &m_manager, SIGNAL(finished(QNetworkReply*)),
        this, SLOT(fileDownloaded(QNetworkReply*))
    );

    QNetworkRequest request(imageUrl);
    m_manager.get(request);
}

void FileDownloader::fileDownloaded(QNetworkReply* pReply)
{
    m_data = pReply->readAll();
    pReply->deleteLater();
    emit downloaded();
}

