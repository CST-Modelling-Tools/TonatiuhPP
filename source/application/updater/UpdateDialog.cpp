#include "UpdateDialog.h"
#include "ui_UpdateDialog.h"

#include "UpdateReader.h"
#include <QStringList>


UpdateDialog::UpdateDialog(QWidget* parent):
    QDialog(parent),
    ui(new Ui::UpdateDialog)
{
    ui->setupUi(this);

//    m_serverPath = "file:///C:/Users/..../Projects/Tonatiuh2020/releases/";
//    m_serverPath = "https://scmt.cyi.ac.cy/bitbucket/projects/TNH/repos/main/browse/releases/";
    QString temp = "https://scmt.cyi.ac.cy/bitbucket/login?j_username=%1&j_password=%2&next=/projects/TNH/repos/main/raw/releases/";
    ui->serverEdit->setText(temp);
}

UpdateDialog::~UpdateDialog()
{
    delete ui;
}

void UpdateDialog::on_checkButton_pressed()
{
    QString temp = ui->serverEdit->text().arg(ui->userEdit->text(), ui->passwordEdit->text());
    temp += "updates.xml";

    QUrl url(temp);
    m_downloader = new FileDownloader(url, this);

    connect(
        m_downloader, SIGNAL(downloaded()),
        this, SLOT(onUpdates())
    );

    ui->downloadButton->setEnabled(false);
    ui->progressBar->reset();
}

void UpdateDialog::onUpdates()
{
//    qDebug() << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();

    if (!m_downloader->status().isEmpty())
    {
        QString t = "Connection failed:\n" + m_downloader->status();
        ui->resultText->setPlainText(t);
        m_downloader->deleteLater();
        return;
    }

    UpdateReader reader;
    if (!reader.checkUpdates(m_downloader->downloadedData()))
    {
        QString t = "Update failed:\n" + reader.m_message;
        ui->resultText->setPlainText(t);
        m_downloader->deleteLater();
        return;
    }
    if (!reader.isNewer())
    {
        QString t = QString("New update (%1):\n%2\nSize:%3B")
                .arg(reader.m_date.toString("d MMM yyyy"))
                .arg(reader.m_path)
                .arg(reader.m_size);
        ui->resultText->setPlainText(t);
        m_downloader->deleteLater();
        return;
    }
    QString t = QString("New update (%1):\n%2\nSize:%3B")
            .arg(reader.m_date.toString("d MMM yyyy"))
            .arg(reader.m_path)
            .arg(reader.m_size);
    m_size = reader.m_size;
    ui->resultText->setPlainText(t);
    ui->downloadButton->setEnabled(true);
    m_update = reader.m_path;

    m_downloader->deleteLater();
}

void UpdateDialog::on_downloadButton_pressed()
{
    QString temp = ui->serverEdit->text().arg(ui->userEdit->text(), ui->passwordEdit->text());
    QString temp2 = m_update;
    temp += temp2.replace('+', "%2B");

    QUrl url(temp);
    m_downloader = new FileDownloader(url, this); //? signals before ? async

    connect(
        m_downloader, SIGNAL(downloaded()),
        this, SLOT(onDownloaded())
    );

    connect(
        m_downloader, SIGNAL(downloadProgress(qint64,qint64)),
        this, SLOT(updateProgress(qint64,qint64))
    );
}

void UpdateDialog::updateProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug() << bytesReceived << " " << bytesTotal;
    ui->progressBar->setRange(0, m_size);
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
    qDebug() << file.fileName();
    if (file.open(QFile::WriteOnly)) {
        file.write(m_downloader->downloadedData());
        file.close();
    }

    m_downloader->deleteLater();
    QProcess::startDetached(dir.filePath(m_update));
//    close();
//    ((QMainWindow*) parent())->close();
}

