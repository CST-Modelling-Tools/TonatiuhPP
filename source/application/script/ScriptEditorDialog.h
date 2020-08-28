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


class ScriptEditorDialog: public QDialog
{
    Q_OBJECT

public:
    ScriptEditorDialog(QVector<RandomFactory*> listRandomFactory, QWidget* parent = 0);
    ~ScriptEditorDialog();

    void run(QString fileName);

    static QScriptValue ImportExtension(QScriptContext* context, QScriptEngine* engine);
    static QScriptValue PrintMessage(QScriptContext* context, QScriptEngine* engine);
    static QScriptValue PrintMessageTimed(QScriptContext* context, QScriptEngine* engine);

public slots:
    void abortScript(QString error);

protected:
    void closeEvent(QCloseEvent* event);

private slots:
    void RunScript();
    void setCurrentFile(QString fileName);
    void writeMessage(QString message);

private:
    Ui::ScriptEditorDialog* ui;

    QString m_fileName;
    QScriptEngine* m_engine;
};
