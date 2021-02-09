#include "FileDownloader.h"


FileDownloader::FileDownloader(QUrl url, QObject* parent):
    QObject(parent)
{
    connect(
        &m_manager, SIGNAL(finished(QNetworkReply*)),
        this, SLOT(fileDownloaded(QNetworkReply*))
    );

//    url.setPassword();

//    QNetworkRequest requestSize(url);
//    requestSize.setHeader(QNetworkRequest::ContentLengthHeader);

//    QUrl url(UrlList.at(0));
//    req.setHeader(Q);
//    req.setUrl(url);
//    QNetworkAccessManager manager;
//    QNetworkReply* reply__ = manager.get(req);
//    FileSize = reply__->header(QNetworkRequest::ContentLengthHeader).toUInt();



    QNetworkRequest request(url);

//    QNetworkReply* reply = m_manager.get(request);
    m_reply = m_manager.get(request);
    qDebug() << "size " << m_reply->header(QNetworkRequest::ContentLengthHeader).toUInt();

//    connect(m_reply, SIGNAL(readyRead()), this, SLOT(updateProgressBar()));

//    connect(reply, &QNetworkReply::downloadProgress, this, &FileDownloader::updateProgress);
    connect(
        m_reply, SIGNAL(downloadProgress(qint64,qint64)),
        this, SLOT(updateProgress(qint64,qint64))
    );

//     connect(reply, SIGNAL(metaDataChanged()), this, SLOT(fileSize()));
}

void FileDownloader::fileDownloaded(QNetworkReply* pReply)
{
    if (pReply->error())
        m_status = pReply->errorString();
    else
        m_data = pReply->readAll();

    pReply->deleteLater();
    emit downloaded();
}

void FileDownloader::updateProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    emit downloadProgress(bytesReceived, bytesTotal);
}

void FileDownloader::updateProgressBar()
{
    qDebug() << "sizeddd " << m_reply->header(QNetworkRequest::ContentLengthHeader).toUInt();

}

