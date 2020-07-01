#include "ParametersEditor.h"
#include "ui_ParameterTextEditor.h"


ParametersEditor::ParametersEditor(QWidget* parent):
    QDialog(parent),
    ui(new Ui::ParameterTextEditor)
{
    ui->setupUi(this);
}

ParametersEditor::~ParametersEditor()
{
    delete ui;
}

void ParametersEditor::setText(const QString& text)
{
    ui->plainTextEdit->setPlainText(text);
}

QString ParametersEditor::getText()
{
    return ui->plainTextEdit->toPlainText();
}
