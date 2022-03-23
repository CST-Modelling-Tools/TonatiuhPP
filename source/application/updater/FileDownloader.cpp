#include "FileDownloader.h"


FileDownloader::FileDownloader(QUrl url, QByteArray data, QObject* parent):
    QObject(parent)
{
    connect(
        &m_manager, SIGNAL(finished(QNetworkReply*)),
        this, SLOT(fileDownloaded(QNetworkReply*))
    );

//    url.setPassword();
    QNetworkRequest request(url);
//    qDebug() << "size head " << request.header(QNetworkRequest::ContentLengthHeader).toUInt();

    // avoid expired ssl
//    QSslConfiguration conf = request.sslConfiguration();
//    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
//    request.setSslConfiguration(conf);

// attempt to get file sizeB
//    QNetworkReply* reply = m_manager.head(request);
//    qDebug() << "size head " << reply->header(QNetworkRequest::ContentLengthHeader).toUInt();

    if (!data.isEmpty()) {
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");
//    networkManager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    m_reply = m_manager.post(request, data);
    }
    else
        m_reply = m_manager.get(request);

    connect(m_reply, &QNetworkReply::downloadProgress, this, &FileDownloader::updateProgress);

//    connect(
//        m_reply, SIGNAL(downloadProgress(qint64,qint64)),
//        this, SLOT(updateProgress(qint64,qint64))
//    );
}

void FileDownloader::abort()
{
    m_reply->abort();
}

void FileDownloader::fileDownloaded(QNetworkReply* pReply)
{
    if (pReply->error())
        m_status = pReply->errorString();
    else
        m_data = pReply->readAll();

//    qDebug() << "headers " << pReply->rawHeaderList();
//    qDebug() << "ContentLengthHeader " << pReply->header(QNetworkRequest::ContentLengthHeader).toUInt();
//    qDebug() << "Transfer-Encoding " << pReply->rawHeader("Transfer-Encoding");
//     ContentLengthHeader is 0 for chunked Transfer-Encoding!

    pReply->deleteLater();
    emit downloaded();
}

void FileDownloader::updateProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    emit downloadProgress(bytesReceived, bytesTotal);
}
