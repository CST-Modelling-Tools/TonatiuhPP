#pragma once

#include <QDialog>

namespace Ui {
class AboutScriptDialog;
}

class AboutScriptDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutScriptDialog(QWidget *parent = nullptr);
    ~AboutScriptDialog();

private:
    Ui::AboutScriptDialog *ui;
};

