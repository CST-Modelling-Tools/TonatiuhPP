#pragma once

#include <QDialog>

namespace Ui {
class ParameterTextEditor;
}


class ParametersEditor: public QDialog
{
    Q_OBJECT

public:
    explicit ParametersEditor(QWidget* parent = nullptr);
    ~ParametersEditor();

    void setText(const QString& text);
    QString text();

private:
    Ui::ParameterTextEditor* ui;
};
