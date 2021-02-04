#include "UpdateDialog.h"
#include "ui_UpdateDialog.h"

#include "UpdateReader.h"
#include <QStringList>

UpdateDialog::UpdateDialog(QWidget* parent):
    QDialog(parent),
    ui(new Ui::UpdateDialog)
{
    ui->setupUi(this);

    m_path = "file:///C:/Users/Victor/Neo/Programming/Qt/Projects/Tonatiuh2020/releases/";

    QUrl urlUpdates(m_path + "updates.xml");
    m_downloader = new FileDownloader(urlUpdates, this);

    connect(
        m_downloader, SIGNAL(downloaded()),
        this, SLOT(onUpdates())
    );
}

UpdateDialog::~UpdateDialog()
{
    delete ui;
}

void UpdateDialog::onUpdates()
{
    UpdateReader reader;
    if (!reader.checkUpdates(m_downloader->downloadedData()))
    {
        QString t = "Updates failed:\n" + reader.m_message;
        ui->labelResult->setText(t);
        return;
    }
    if (!reader.isNewer())
    {
        QString t = "No updates available";
        ui->labelResult->setText(t);
        return;
    }
    QString message = QString("Updates available:\n%1\n%2")
            .arg(reader.m_date.toString("d MMM yyyy"))
            .arg(reader.m_path);
    ui->labelResult->setText(message);
    ui->buttonDownload->setEnabled(true);
    m_update = reader.m_path;
}

void UpdateDialog::on_buttonDownload_pressed()
{
    delete m_downloader;
    QUrl urlUpdates(m_path + m_update);
    m_downloader = new FileDownloader(urlUpdates, this);
    connect(
        m_downloader, SIGNAL(downloaded()),
        this, SLOT(onDownloaded())
    );
}

#include <QMessageBox>
void UpdateDialog::onDownloaded()
{
    QMessageBox::information(this, "Downloaded", "Finished");
}

