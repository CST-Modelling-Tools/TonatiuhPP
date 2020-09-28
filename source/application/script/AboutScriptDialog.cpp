#include "AboutScriptDialog.h"
#include "ui_AboutScriptDialog.h"


AboutScriptDialog::AboutScriptDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutScriptDialog)
{
    ui->setupUi(this);

    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
}

AboutScriptDialog::~AboutScriptDialog()
{
    delete ui;
}
