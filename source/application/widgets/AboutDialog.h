#pragma once

#include <QDialog>

namespace Ui {
class AboutDialog;
}


class AboutDialog: public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget* parent = nullptr);
    ~AboutDialog();

private:
    void updateLabel();

private slots:
    void on_label_customContextMenuRequested(const QPoint& pos);

private:
    Ui::AboutDialog* ui;
};
