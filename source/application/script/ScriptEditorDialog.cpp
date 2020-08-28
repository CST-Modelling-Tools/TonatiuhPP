#include "ScriptEditorDialog.h"
#include "ui_ScriptEditorDialog.h"

#include <iostream>
#include <stdio.h>

#include <QDateTime>
#include <QElapsedTimer>
#include <QScriptEngine>
#include <QString>
#include <QDir>

#include "ScriptRayTracer.h"
#include "tonatiuh_script.h"

Q_DECLARE_METATYPE(QVector<QVariant>)

#include "NodeObject.h"
#include "FileObject.h"
#include "main/MainWindow.h"

Q_SCRIPT_DECLARE_QMETAOBJECT(NodeObject, QObject*)
Q_SCRIPT_DECLARE_QMETAOBJECT(FileObject, QObject*)

QString timeString()
{
    return QString("[%1] ").arg(QDateTime::currentDateTime().toString("hh:mm:ss"));
}

/**
 * Creates a dialog to edit scripts and run them. The list \a listRandomFactory is
 * the random generator types that can be defined in the scripts to run Tonatiuh. The dialog explorer shows the directories and scripts files from \a dirName path.
 */
ScriptEditorDialog::ScriptEditorDialog(QVector<RandomFactory*> randomFactories, QWidget* parent):
    QDialog(parent),
    ui(new Ui::ScriptEditorDialog),
    m_fileName("")
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowMaximizeButtonHint, true);

    int h = height();
    ui->splitter->setSizes({ int(0.8*h), int(0.2*h) });

    int q = fontMetrics().height();
    resize(64*q, 48*q);

    QFont font("Consolas", 9);
    ui->logWidget->setFont(font);

    // engine
    m_engine = new QScriptEngine(this);
    qScriptRegisterSequenceMetaType<QVector<QVariant>>(m_engine);

    // objects
    QScriptValue tonatiuhObject = m_engine->newQObject(parent);
    m_engine->globalObject().setProperty("tonatiuh", tonatiuhObject);
    m_engine->globalObject().setProperty("tn", tonatiuhObject);

    QScriptValue consoleObject = m_engine->newQObject(ui->logWidget);
    m_engine->globalObject().setProperty("console", consoleObject);

    QObject* rayTracer = new ScriptRayTracer(randomFactories);
    QScriptValue rayTracerObject = m_engine->newQObject(rayTracer);
    m_engine->globalObject().setProperty("rayTracer", rayTracerObject);

    NodeObject::setPlugins(static_cast<MainWindow*>(parent)->getPlugins());
    QScriptValue nodeObjectClass = m_engine->scriptValueFromQMetaObject<NodeObject>();
    m_engine->globalObject().setProperty("NodeObject", nodeObjectClass);

    QScriptValue fileObjectClass = m_engine->scriptValueFromQMetaObject<FileObject>();
    m_engine->globalObject().setProperty("FileObject", fileObjectClass);

    // functions
    //    QString pluginsDirectory = QApplication::applicationDirPath() + QDir::separator() + "plugins";
    //    QCoreApplication::addLibraryPath(pluginsDirectory);
    //    QCoreApplication::addLibraryPath(QApplication::applicationDirPath());
    QScriptValue importFunction = m_engine->newFunction(ScriptEditorDialog::ImportExtension);
    m_engine->globalObject().setProperty("Import", importFunction, QScriptValue::ReadOnly);

    QScriptValue printFunction = m_engine->newFunction(ScriptEditorDialog::PrintMessage);
    m_engine->globalObject().setProperty("print", printFunction);

    QScriptValue timeFunction = m_engine->newFunction(ScriptEditorDialog::PrintMessageTimed);
    m_engine->globalObject().setProperty("printTimed", timeFunction);

    connect(ui->codeEditorWidget, SIGNAL(opened(QString)), this, SLOT(setCurrentFile(QString)) );
    connect(ui->codeEditorWidget, SIGNAL(saved(QString)), this, SLOT(setCurrentFile(QString)) );
    connect(ui->codeEditorWidget, SIGNAL(runned()), this, SLOT(RunScript()) );

    connect(ui->runButton, SIGNAL(clicked(bool)), this, SLOT(RunScript()) );
    connect(ui->closeButton, SIGNAL(clicked(bool)), this, SLOT(close()) );
    connect(parent, SIGNAL(Abort(QString)), this, SLOT(abortScript(QString)) );
}

ScriptEditorDialog::~ScriptEditorDialog()
{
    delete ui;
}

void ScriptEditorDialog::run(QString fileName)
{
    if (fileName.isEmpty()) return;
    ui->codeEditorWidget->open(fileName);
    setCurrentFile(fileName);
    RunScript();
    close();
}

void ScriptEditorDialog::abortScript(QString error)
{
    QScriptContext* context = m_engine->currentContext();
    context->throwError(error);
}

void ScriptEditorDialog::closeEvent(QCloseEvent* event)
{
    if (ui->codeEditorWidget->isReady())
        event->accept();
    else
        event->ignore();
}

// ScriptRayTracer is less general than MainWindow
// its functions are added by tonatiuh_script::init
// todo: why not to use as NodeObject?
// better make a class interface as NodeObject on top of ScriptRayTracer
// instead of global functions

void ScriptEditorDialog::RunScript()
{
    try {
        int initialized = tonatiuh_script::init(m_engine);
        if (!initialized)
            throw timeString() + "Script engine could not start.";

        QScriptValue rayTracerObject = m_engine->globalObject().property("rayTracer");
        ScriptRayTracer* rayTracer = (ScriptRayTracer*) rayTracerObject.toQObject();
        QFileInfo fileInfo(m_fileName);
        rayTracer->setDir(fileInfo.absoluteDir().absolutePath());

        QTextDocument* document = ui->codeEditorWidget->document();
        QString program = document->toPlainText();

        QScriptSyntaxCheckResult check = m_engine->checkSyntax(program);
        if (check.state() != QScriptSyntaxCheckResult::Valid)
            throw timeString() + QString("Syntax error in line %1.\n%2")
                .arg(check.errorLineNumber())
                .arg(check.errorMessage());

        QElapsedTimer timer;
        timer.start();
        writeMessage(timeString() + "Script started.");

        QScriptValue result = m_engine->evaluate(document->toPlainText());
        if (result.isError())
            throw timeString() + QString("Runtime error in line %1\n%2")
                .arg(result.property("lineNumber").toNumber())
                .arg(result.toString());

        QString msg = timeString() + QString("Script finished in %1 s.")
                .arg(timer.elapsed()/1000., 0, 'f', 3);
        writeMessage(msg);
    }
    catch (QString msg)
    {
        writeMessage(msg);
        std::cerr << msg.toStdString() << std::endl;
    }
}

void ScriptEditorDialog::setCurrentFile(QString fileName)
{
    m_fileName = fileName;

    QString title = "Untitled";
    if (!fileName.isEmpty())
        title = QFileInfo(fileName).fileName();
    setWindowTitle(tr("%1[*] - Tonatiuh").arg(title));
}

void ScriptEditorDialog::writeMessage(QString message)
{
    ui->logWidget->appendPlainText(message);
}

QScriptValue ScriptEditorDialog::ImportExtension(QScriptContext* context, QScriptEngine* engine)
{
    if (context->argumentCount() != 1) return false;
    if (!context->argument(0).isString()) return false;
    QString name = context->argument(0).toString();

    if (!engine->importExtension(name).isUndefined())
    {
        QString msg = QString("Warning! %1 not found!").arg(name);

        //    writeMessage(msg); //?
        QScriptValue object = engine->globalObject().property("console");
        QPlainTextEdit* console = (QPlainTextEdit*) object.toQObject();
        if (!console) return false;
        console->appendPlainText(msg);
        return false;
    }

    return true;
}

QScriptValue ScriptEditorDialog::PrintMessage(QScriptContext* context, QScriptEngine* engine)
{
    if (context->argumentCount() < 1) return false;
    if (!context->argument(0).isString()) return false;
    QString msg = context->argument(0).toString();

    QScriptValue object = engine->globalObject().property("console");
    QPlainTextEdit* console = (QPlainTextEdit*) object.toQObject();
    if (!console) return false;
    console->appendPlainText(msg);
    return true;
}

QScriptValue ScriptEditorDialog::PrintMessageTimed(QScriptContext* context, QScriptEngine* engine)
{
    if (context->argumentCount() < 1) return false;
    if (!context->argument(0).isString()) return false;
    QString msg = timeString() + context->argument(0).toString();

    QScriptValue object = engine->globalObject().property("console");
    QPlainTextEdit* console = (QPlainTextEdit*) object.toQObject();
    if (!console) return false;
    console->appendPlainText(msg);
    return true;
}
