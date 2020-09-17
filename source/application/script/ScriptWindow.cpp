#include "ScriptWindow.h"
#include "ui_ScriptWindow.h"

#include <iostream>

#include <QDateTime>
#include <QElapsedTimer>
#include <QScriptEngine>
#include <QDir>
#include <QMessageBox>
#include <QSettings>
#include <QFileDialog>
#include <QTextStream>

Q_DECLARE_METATYPE(QVector<QVariant>)

#include "NodeObject.h"
#include "FileObject.h"
#include "main/MainWindow.h"
#include "kernel/scene/TSceneKit.h"
#include "main/PluginManager.h"
#include "SyntaxHighlighter.h"
#include "ScriptRayTracer.h"
#include "tonatiuh_script.h"

Q_SCRIPT_DECLARE_QMETAOBJECT(NodeObject, QObject*)
Q_SCRIPT_DECLARE_QMETAOBJECT(FileObject, QObject*)

QString timeString()
{
    return QString("[%1] ").arg(QDateTime::currentDateTime().toString("hh:mm:ss"));
}

ScriptWindow::ScriptWindow(MainWindow* mw, QWidget* parent):
    QMainWindow(parent),
    ui(new Ui::ScriptWindow)
{
    ui->setupUi(this);

    int q = fontMetrics().height();
    resize(64*q, 48*q);

    int h = height();
    ui->splitter->setSizes({ int(0.8*h), int(0.2*h) });

    ui->logWidget->setFont(ui->codeEditor->font());

    m_syntax = new SyntaxHighlighter(ui->codeEditor->document());

    // engine
    m_engine = new QScriptEngine(this);
    qScriptRegisterSequenceMetaType<QVector<QVariant>>(m_engine);

    // objects
    QScriptValue tonatiuhObject = m_engine->newQObject(mw);
    m_engine->globalObject().setProperty("tonatiuh", tonatiuhObject);
    m_engine->globalObject().setProperty("tn", tonatiuhObject);

    QScriptValue consoleObject = m_engine->newQObject(ui->logWidget);
    m_engine->globalObject().setProperty("console", consoleObject);

    QObject* rayTracer = new ScriptRayTracer(mw->getPlugins()->getRandomFactories());
    QScriptValue rayTracerObject = m_engine->newQObject(rayTracer);
    m_engine->globalObject().setProperty("rayTracer", rayTracerObject);

    NodeObject::setMainWindow(mw);
    QScriptValue nodeObjectClass = m_engine->scriptValueFromQMetaObject<NodeObject>();
    m_engine->globalObject().setProperty("NodeObject", nodeObjectClass);

    QScriptValue fileObjectClass = m_engine->scriptValueFromQMetaObject<FileObject>();
    m_engine->globalObject().setProperty("FileObject", fileObjectClass);

    // functions
    //    QString pluginsDirectory = QApplication::applicationDirPath() + QDir::separator() + "plugins";
    //    QCoreApplication::addLibraryPath(pluginsDirectory);
    //    QCoreApplication::addLibraryPath(QApplication::applicationDirPath());
    QScriptValue importFunction = m_engine->newFunction(ScriptWindow::ImportExtension);
    m_engine->globalObject().setProperty("Import", importFunction, QScriptValue::ReadOnly);

    QScriptValue printFunction = m_engine->newFunction(ScriptWindow::PrintMessage);
    m_engine->globalObject().setProperty("print", printFunction);

    QScriptValue timeFunction = m_engine->newFunction(ScriptWindow::PrintMessageTimed);
    m_engine->globalObject().setProperty("printTimed", timeFunction);

    connect(mw, SIGNAL(Abort(QString)), this, SLOT(abortScript(QString)) );

    connect(ui->actionFileNew, SIGNAL(triggered()), this, SLOT(fileNew()) );
    connect(ui->actionFileOpen, SIGNAL(triggered()), this, SLOT(fileOpen()) );
    connect(ui->actionFileSave, SIGNAL(triggered()), this, SLOT(fileSave()) );
    connect(ui->actionFileSaveAs, SIGNAL(triggered()), this, SLOT(fileSaveAs()) );
    connect(ui->actionRunScript, SIGNAL(triggered()), this, SLOT(runScript()) );
}

ScriptWindow::~ScriptWindow()
{
    delete ui;
}

void ScriptWindow::runScript(QString fileName)
{
    if (fileName.isEmpty()) return;
    fileOpen(fileName);
    runScript();
    close();
}

bool ScriptWindow::isReady()
{
    if (!ui->codeEditor->document()->isModified())
        return true;

    QMessageBox::StandardButton answer = QMessageBox::warning(
        this, "Tonatiuh",
        "The document was modified.\n"
        "Do you want to save changes?",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
        QMessageBox::Yes
    );

    if (answer == QMessageBox::Yes)
        return fileSave();
    if (answer == QMessageBox::No)
        return true;
    return false;
}

void ScriptWindow::fileOpen(QString fileName)
{
    if (!isReady()) return;

    if (fileName.isEmpty())
    {
        QSettings settings("CyI", "Tonatiuh");
        QString dirName = settings.value("dirScript", "../examples").toString();

        fileName = QFileDialog::getOpenFileName(
            this, "Open File", dirName,
            "Tonatiuh script file (*.tnhs)"
        );
        if (fileName.isEmpty()) return;

        QFileInfo fileInfo(fileName);
        settings.setValue("dirScript", fileInfo.path());
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(
            this, "Tonatiuh",
            tr("Cannot open file %1.").arg(fileName)
        );
        return;
    }

    QTextDocument* document = ui->codeEditor->document();
    document->clear();
    document->setPlainText(file.readAll());
    document->setModified(false);

    setTitle(fileName);
}

void ScriptWindow::fileNew()
{
    if (!isReady()) return;

    QTextDocument* document = ui->codeEditor->document();
    document->clear();
    document->setModified(false);

    setTitle("");
}

bool ScriptWindow::fileSaveAs(QString fileName)
{
    if (fileName.isEmpty())
    {
        QSettings settings("CyI", "Tonatiuh");
        QString dirName = settings.value("dirScript", "../examples").toString();

        fileName = QFileDialog::getSaveFileName(
            this, "Save File", dirName,
            "Tonatiuh script file (*.tnhs)"
        );
        if (fileName.isEmpty()) return false;

        QFileInfo fileInfo(fileName);
        settings.setValue("dirScript", fileInfo.path());
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(
            this, "Tonatiuh",
            tr("Cannot open file %1.").arg(fileName)
        );
        return false;
    }
    QTextStream out(&file); //? without stream

    QTextDocument* document = ui->codeEditor->document();
    out << document->toPlainText();
    document->setModified(false);

    setTitle(fileName);
    return true;
}

bool ScriptWindow::fileSave()
{
    return fileSaveAs(m_fileName);
}

// ScriptRayTracer is less general than MainWindow
// its functions are added by tonatiuh_script::init
// todo: why not to use as NodeObject?
// better make a class interface as NodeObject on top of ScriptRayTracer
// instead of global functions

void ScriptWindow::runScript()
{
    try {
        int initialized = tonatiuh_script::init(m_engine);
        if (!initialized)
            throw timeString() + "Script engine could not start.";

        QScriptValue rayTracerObject = m_engine->globalObject().property("rayTracer");
        ScriptRayTracer* rayTracer = (ScriptRayTracer*) rayTracerObject.toQObject();
        QFileInfo fileInfo(m_fileName);
        rayTracer->setDir(fileInfo.absoluteDir().absolutePath());

        QTextDocument* document = ui->codeEditor->document();
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

void ScriptWindow::setTitle(QString fileName)
{
    m_fileName = fileName;

    QString title = "Untitled";
    if (!fileName.isEmpty())
        title = QFileInfo(fileName).fileName();
    setWindowTitle(tr("%1[*] - Tonatiuh").arg(title));
}

void ScriptWindow::closeEvent(QCloseEvent* event)
{
    if (isReady())
        event->accept();
    else
        event->ignore();
}

void ScriptWindow::writeMessage(QString message)
{
    ui->logWidget->appendPlainText(message);
}

void ScriptWindow::abortScript(QString error)
{
    QScriptContext* context = m_engine->currentContext();
    context->throwError(error);
}

QScriptValue ScriptWindow::ImportExtension(QScriptContext* context, QScriptEngine* engine)
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

QScriptValue ScriptWindow::PrintMessage(QScriptContext* context, QScriptEngine* engine)
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

QScriptValue ScriptWindow::PrintMessageTimed(QScriptContext* context, QScriptEngine* engine)
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
