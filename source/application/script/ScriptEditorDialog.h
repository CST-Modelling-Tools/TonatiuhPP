#pragma once

#include <QDialog>
#include <QScriptValue>

class FilesModel;
class QItemSelectionModel;
class QLineEdit;
class QScriptContext;
class QScriptEngine;
class RandomFactory;

namespace Ui {
class ScriptEditorDialog;
}

//!  ScriptEditorDialog class is the dialog to edit and run scripts with Tonatiuh.
/*!
  ScriptEditorDialog allow to the user open, edit, run and save scripts to automate the ray tracing.
*/
class ScriptEditorDialog: public QDialog
{
    Q_OBJECT

public:
    ScriptEditorDialog(QVector<RandomFactory*> listRandomFactory, QWidget* parent = 0);
    ~ScriptEditorDialog();

    void ExecuteScript(QString file);

    static QScriptValue ImportExtension(QScriptContext* context, QScriptEngine* engine);
    static QScriptValue PrintMessage(QScriptContext* context, QScriptEngine* engine);

public slots:
    void AbortEvaluation(QString error);

protected:
    void closeEvent(QCloseEvent* event);

private slots:
    void RunScript();
    void SetCurrentFile(QString fileName);
    void WriteMessage(QString message);

private:
    Ui::ScriptEditorDialog* ui;

    QString m_currentScritFileName;
    QScriptEngine* m_interpreter;
};
