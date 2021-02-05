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
    void onUpdates();
    void on_downloadButton_pressed();
    void onDownloaded();

private:
    Ui::UpdateDialog* ui;
    QString m_serverPath;
    QString m_update;
    FileDownloader* m_downloader;
};

