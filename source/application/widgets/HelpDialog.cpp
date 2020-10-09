#include "HelpDialog.h"
#include "ui_HelpDialog.h"


HelpDialog::HelpDialog(QWidget* parent):
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);

    QList<int> sizes = {200, 700};
    ui->splitter->setSizes(sizes);

    int q = fontMetrics().height();
    resize(64*q, 48*q);

    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
}

HelpDialog::~HelpDialog()
{
    delete ui;
}
