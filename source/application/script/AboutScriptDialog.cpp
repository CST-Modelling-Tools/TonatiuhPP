#include "AboutScriptDialog.h"
#include "ui_AboutScriptDialog.h"


AboutScriptDialog::AboutScriptDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutScriptDialog)
{
    ui->setupUi(this);

    int q = fontMetrics().height();
    resize(48*q, 36*q);

    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
}

AboutScriptDialog::~AboutScriptDialog()
{
    delete ui;
}
