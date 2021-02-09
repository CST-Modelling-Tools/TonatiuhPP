#pragma once

#include <QDialog>
#include "FileDownloader.h"

namespace Ui {
class UpdateDialog;
}

class UpdateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateDialog(QWidget* parent = 0);
    ~UpdateDialog();

    void checkUpdates();

private slots:
    void on_checkButton_pressed();
    void onUpdates();

    void onDownloaded();
    void on_downloadButton_pressed();

    void updateProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    Ui::UpdateDialog* ui;
    QString m_update;
    int m_size;
    FileDownloader* m_downloader;
};

