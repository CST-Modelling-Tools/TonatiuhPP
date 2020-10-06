#pragma once

#include <QMainWindow>
#include <QScriptValue>

class FilesModel;
class QItemSelectionModel;
class QLineEdit;
class QScriptContext;
class QScriptEngine;
class RandomFactory;
class MainWindow;
class SyntaxHighlighter;

namespace Ui {
class ScriptWindow;
}


class ScriptWindow: public QMainWindow
{
    Q_OBJECT

public:
    ScriptWindow(MainWindow* mw, QWidget* parent = 0);
    ~ScriptWindow();

    void runScript(QString fileName);
    bool isReady();

    static QScriptValue ImportExtension(QScriptContext* context, QScriptEngine* engine);
    static QScriptValue PrintMessage(QScriptContext* context, QScriptEngine* engine);
    static QScriptValue PrintMessageTimed(QScriptContext* context, QScriptEngine* engine);

public slots:
    void fileOpen(QString fileName = QString());
    void fileNew();
    bool fileSaveAs(QString fileName = QString());
    bool fileSave();
    void runScript();

protected:
    void setTitle(QString fileName);
    void closeEvent(QCloseEvent* event);

private slots:
    void writeMessage(QString message);
    void abortScript(QString error);

    void on_actionExamples_triggered();
    void on_actionAbout_triggered();

private:
    Ui::ScriptWindow* ui;

    QString m_fileName;
    QScriptEngine* m_engine;
    SyntaxHighlighter* m_syntax;
};
