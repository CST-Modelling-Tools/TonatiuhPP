#include "ParametersEditor.h"
#include "ui_ParametersEditor.h"


ParametersEditor::ParametersEditor(QWidget* parent):
    QDialog(parent),
    ui(new Ui::ParameterTextEditor)
{
    ui->setupUi(this);

    int q = fontMetrics().height();
    resize(48*q, 36*q);

    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
}

ParametersEditor::~ParametersEditor()
{
    delete ui;
}

void ParametersEditor::setText(const QString& text)
{
    ui->plainTextEdit->setPlainText(text);
}

QString ParametersEditor::text()
{
    return ui->plainTextEdit->toPlainText();
}
