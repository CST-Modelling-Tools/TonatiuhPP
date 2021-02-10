#include "UpdateDialog.h"
#include "ui_UpdateDialog.h"

#include "UpdateReader.h"
#include <QStringList>


UpdateDialog::UpdateDialog(QWidget* parent):
    QDialog(parent),
    ui(new Ui::UpdateDialog)
{
    ui->setupUi(this);

//    m_serverPath = "file:///C:/Users/";
//    m_serverPath = "https://scmt.cyi.ac.cy/bitbucket/projects/TNH/repos/updates/raw/nsis/";
//    QString temp = "https://scmt.cyi.ac.cy/bitbucket/login?j_username=%1&j_password=%2&next=/projects/TNH/repos/updates/raw/nsis/";
    QString temp = "https://scmt.cyi.ac.cy/bitbucket/login";
    ui->serverEdit->setText(temp);
    ui->serverEdit->setCursorPosition(0);
    ui->userEdit->setText("updates.tonatiuh");
    ui->passwordEdit->setText("gIZ5QS2WKqsA");

    ui->downloadButton->setEnabled(false);
    ui->progressBar->hide();

    m_downloaderF = 0;

    ui->checkButton->setFocus();
}

UpdateDialog::~UpdateDialog()
{
    delete ui;
}

void UpdateDialog::on_checkButton_pressed()
{
    QString temp = ui->serverEdit->text();
    temp += "updates.xml";

    QByteArray postData;
    if (!ui->userEdit->text().isEmpty()) {
        postData.append(
            QString("j_username=%1&j_password=%2&next=/projects/TNH/repos/updates/raw/nsis/%3").arg(
                ui->userEdit->text(),
                ui->passwordEdit->text(),
                "updates.xml"
        ).toUtf8());
    }

    QUrl url(temp);
    m_downloaderU = new FileDownloader(url, postData, this);

    connect(
        m_downloaderU, SIGNAL(downloaded()),
        this, SLOT(onUpdates())
    );

    ui->downloadButton->setEnabled(false);
    ui->progressBar->hide();
}

void UpdateDialog::onUpdates()
{
//    qDebug() << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();

    if (!m_downloaderU->status().isEmpty())
    {
        QString t = "Connection failed:\n" + m_downloaderU->status();
        ui->resultText->setPlainText(t);
        m_downloaderU->deleteLater();
        return;
    }

    UpdateReader reader;
    if (!reader.checkUpdates(m_downloaderU->downloadedData()))
    {
        QString t = "Update failed:\n" + reader.m_message;
        ui->resultText->setPlainText(t);
        m_downloaderU->deleteLater();
        return;
    }
    QString info = QString("Date: %1\nFile: \"%2\"\nSize: %3 MB")
            .arg(reader.m_date.toString("d MMM yyyy"))
            .arg(reader.m_path)
            .arg(reader.m_size/1e6, 0, 'f', 1);
    if (!reader.isNewer())
    {
        QString t = QString("Last update\n") + info;
        ui->resultText->setPlainText(t);
        m_downloaderU->deleteLater();
        return;
    }
    QString t = QString("New update\n") + info;
    m_size = reader.m_size;
    m_update = reader.m_path;
    ui->resultText->setPlainText(t);

    ui->downloadButton->setEnabled(true);

    ui->progressBar->setRange(0, m_size);
    ui->progressBar->show();
    m_downloaderU->deleteLater();
}

void UpdateDialog::on_downloadButton_pressed()
{
    QString temp = ui->serverEdit->text();
    QString temp2 = m_update;
    temp2 = temp2.replace('+', "%2B");

    if (m_downloaderF) {
        disconnect(m_downloaderF, SIGNAL(downloaded()), 0, 0);
        disconnect(m_downloaderF, SIGNAL(downloadProgress(qint64,qint64)), 0, 0);
        m_downloaderF->abort();
    }

    QByteArray postData;
    if (!ui->userEdit->text().isEmpty()) {
        postData.append(
            QString("j_username=%1&j_password=%2&next=/projects/TNH/repos/updates/raw/nsis/%3").arg(
                ui->userEdit->text(),
                ui->passwordEdit->text(),
                temp2
            ).toUtf8());
    } else {
        temp += temp2;
    }

    QUrl url(temp);
    m_downloaderF = new FileDownloader(url, postData, this); //? signals before ? async

    connect(
        m_downloaderF, SIGNAL(downloaded()),
        this, SLOT(onDownloaded())
    );

    connect(
        m_downloaderF, SIGNAL(downloadProgress(qint64,qint64)),
        this, SLOT(updateProgress(qint64,qint64))
    );
}

void UpdateDialog::updateProgress(qint64 bytesReceived, qint64 bytesTotal)
{
//    qDebug() << bytesReceived << " " << bytesTotal;
//    ui->progressBar->setRange(0, m_size);
    ui->progressBar->setValue(bytesReceived);
}

#include <QMessageBox>
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QProcess>
#include <QMainWindow>

void UpdateDialog::onDownloaded()
{
//    QMessageBox::information(this, "Downloaded", "Finished");

    QDir dir(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));

    QFile file(dir.filePath(m_update));
//    qDebug() << file.fileName();
    if (file.open(QFile::WriteOnly)) {
        file.write(m_downloaderF->downloadedData());
        file.close();
    }

    m_downloaderF->deleteLater();
    QProcess::startDetached(dir.filePath(m_update));
    close();
    ((QMainWindow*) parent())->close();
}

