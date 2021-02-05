#include "UpdateDialog.h"
#include "ui_UpdateDialog.h"

#include "UpdateReader.h"
#include <QStringList>


UpdateDialog::UpdateDialog(QWidget* parent):
    QDialog(parent),
    ui(new Ui::UpdateDialog)
{
    ui->setupUi(this);

    m_serverPath = "file:///C:/Users/Victor/Neo/Programming/Qt/Projects/Tonatiuh2020/releases/";
    ui->serverEdit->setText(m_serverPath);

    checkUpdates();
}

UpdateDialog::~UpdateDialog()
{
    delete ui;
}

void UpdateDialog::checkUpdates()
{
    QUrl urlUpdates(m_serverPath + "updates.xml");
    m_downloader = new FileDownloader(urlUpdates, this);

    connect(
        m_downloader, SIGNAL(downloaded()),
        this, SLOT(onUpdates())
    );
}

void UpdateDialog::onUpdates()
{
    if (!m_downloader->status().isEmpty())
    {
        QString t = "Connection failed:\n" + m_downloader->status();
        ui->resultText->setPlainText(t);
        return;
    }

    UpdateReader reader;
    if (!reader.checkUpdates(m_downloader->downloadedData()))
    {
        QString t = "Update failed:\n" + reader.m_message;
        ui->resultText->setPlainText(t);
        return;
    }
    if (!reader.isNewer())
    {
        QString t = QString("Last update (%1):\n%2")
            .arg(reader.m_date.toString("d MMM yyyy"))
            .arg(reader.m_path);
        ui->resultText->setPlainText(t);
        return;
    }
    QString t = QString("New update (%1):\n%2")
            .arg(reader.m_date.toString("d MMM yyyy"))
            .arg(reader.m_path);
    ui->resultText->setPlainText(t);
    ui->downloadButton->setEnabled(true);
    m_update = reader.m_path;

    m_downloader->deleteLater();
}

void UpdateDialog::on_downloadButton_pressed()
{
    QUrl urlUpdates(m_serverPath + m_update);
    m_downloader = new FileDownloader(urlUpdates, this);

    connect(
        m_downloader, SIGNAL(downloaded()),
        this, SLOT(onDownloaded())
    );
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
    close();
    ((QMainWindow*) parent())->close();
}

