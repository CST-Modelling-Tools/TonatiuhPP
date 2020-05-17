#include "ScriptEditorDialog.h"
#include "ui_scripteditordialog.h"

#include <iostream>
#include <stdio.h>

#include <QDateTime>
#include <QScriptEngine>
#include <QString>
#include <QDir>

#include "ScriptRayTracer.h"
#include "tonatiuh_script.h"


Q_DECLARE_METATYPE(QVector<QVariant>)

/**
 * Creates a dialog to edit scripts and run them. The list \a listRandomFactory is
 * the random generator types that can be defined in the scripts to run Tonatiuh. The dialog explorer shows the directories and scripts files from \a dirName path.
 */
ScriptEditorDialog::ScriptEditorDialog(QVector<RandomFactory*> listRandomFactory, QWidget* parent):
    QDialog(parent),
    ui(new Ui::ScriptEditorDialog),
    m_currentScritFileName("")
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);

    QSize windowSize = size();
    QList<int> sizes;
    sizes << windowSize.height() * 0.8 << windowSize.height() * 0.2;
    ui->splitter->setSizes(sizes);

    QString pluginsDirectory = QApplication::applicationDirPath() + QDir::separator() + "plugins";
    QCoreApplication::addLibraryPath(pluginsDirectory);

    // init QtScript environment
    m_interpreter = new QScriptEngine(this);
    qScriptRegisterSequenceMetaType<QVector<QVariant> >(m_interpreter);

    QScriptValue tonatiuh = m_interpreter->newQObject(parent);
    m_interpreter->globalObject().setProperty("tonatiuh", tonatiuh);

    QScriptValue logConsoleObject = m_interpreter->newQObject(ui->logWidget);
    m_interpreter->globalObject().setProperty("console", logConsoleObject);

    QObject* rayTracer = new ScriptRayTracer(listRandomFactory);
    QScriptValue rayTracerValue = m_interpreter->newQObject(rayTracer);
    m_interpreter->globalObject().setProperty("rayTracer", rayTracerValue);

    QScriptValue printFunction = m_interpreter->newFunction(ScriptEditorDialog::PrintMessage);
    m_interpreter->globalObject().setProperty("print", printFunction);
    //m_interpreter->globalObject().setProperty( "print", m_interpreter->newFunction( ScriptEditorDialog::WriteMessage ) );

    QScriptValue import = m_interpreter->newFunction(ScriptEditorDialog::ImportExtension);
    m_interpreter->globalObject().setProperty("Import", import, QScriptValue::ReadOnly);

    connect(ui->codeEditorWidget, SIGNAL(FileOpened(QString)), this, SLOT(SetCurrentFile(QString)) );
    connect(ui->codeEditorWidget, SIGNAL(FileSaved(QString)), this, SLOT(SetCurrentFile(QString)) );
    connect(ui->codeEditorWidget, SIGNAL(RunScript()), this, SLOT(RunScript()) );
    connect(ui->runButton, SIGNAL(clicked(bool)), this, SLOT(RunScript()) );
    connect(ui->closeButton, SIGNAL(clicked(bool)), this, SLOT(close()) );
    connect(parent, SIGNAL(Abort(QString)), this, SLOT(AbortEvaluation(QString)) );
}

/**
 * Destroys the file dialog.
 */
ScriptEditorDialog::~ScriptEditorDialog()
{
    delete ui;
}

void ScriptEditorDialog::ExecuteScript(QString file)
{
    if (file.isEmpty()) return;
    ui->codeEditorWidget->OpenScriptFile(file);
    SetCurrentFile(file);
    RunScript();
    close();
}

void ScriptEditorDialog::AbortEvaluation(QString error)
{
    QScriptContext* context = m_interpreter->currentContext();
    context->throwError(error);
}

/**
 * Reimplemented from QDialog::closeEvent().
 */
void ScriptEditorDialog::closeEvent(QCloseEvent* event)
{
    if (ui->codeEditorWidget->OkToContinue())
        event->accept();
    else
        event->ignore();
}

/**
 * Executes the code editor script.
 */
void  ScriptEditorDialog::RunScript()
{
    QDateTime start = QDateTime::currentDateTime();
    QString message = QString("[%1]\t Start running script.\n").arg(start.toString());
    WriteMessage(message);

    int initialized = tonatiuh_script::init(m_interpreter);
    if (!initialized)
    {
        message = QString("[%1]\t Script Execution Error.\n").arg(QDateTime::currentDateTime().toString() );
        WriteMessage(message);
        std::cerr << message.toStdString() << std::endl;
        return;
    }

    QScriptValue rayTracerValue = m_interpreter->globalObject().property("rayTracer");
    ScriptRayTracer* rayTracer = (ScriptRayTracer*) rayTracerValue.toQObject();
    QFileInfo currentFile(m_currentScritFileName);
    rayTracer->SetDir(currentFile.absoluteDir().absolutePath());

    QTextDocument* document = ui->codeEditorWidget->Document();
    QString program = document->toPlainText();
    QScriptSyntaxCheckResult checkResult = m_interpreter->checkSyntax(program);
    if (checkResult.state() != QScriptSyntaxCheckResult::Valid)
    {
        message = QString("[%1]\t Script Execution Error.\n"
                                     "\t Line: %2. %3\n").arg(QDateTime::currentDateTime().toString(), QString::number(checkResult.errorLineNumber() ), checkResult.errorMessage () );
        WriteMessage(message);
        std::cerr << message.toStdString() << std::endl;
        return;
    }

    QScriptValue result = m_interpreter->evaluate(document->toPlainText());
    if (result.isError())
    {
        message = QString("[%1]\t Script Execution Error. %2\n").arg(QDateTime::currentDateTime().toString(), result.toString() );
        WriteMessage(message);
        //std::cerr<<logmessage.toStdString()<<std::endl;
    }
    else
    {
        /*
         * Display a dialog
         *
           QString st;
           QString area( QString("The valid sun area is %2" ).arg(st.setNum(rayTracer->GetArea())));
           QString drawRays( QString("Number of rays traced per interaction: %1").arg(st.setNum(int(rayTracer->GetNumrays()))));
           QString logmessage = QString( "[%1]\t The script execution is successfully finished in %2 seconds.\n \t\t %3 \n \t\t %4 \n" ).arg( QDateTime::currentDateTime().toString(),st.setNum(start.secsTo( QDateTime::currentDateTime() )),area,drawRays );
           WriteMessage( logmessage );
         *
         */
        message = QString("[%1]\t The script execution is successfully finished.\n").arg(QDateTime::currentDateTime().toString());
        WriteMessage(message);
    }
}

/*!
 * Sets the editor current script file to \a fileName.
 *
 * Changes the editor dialog title to shows the current file base name.
 */
void ScriptEditorDialog::SetCurrentFile(QString fileName)
{
    m_currentScritFileName = fileName;

    QString title = "Untitled";
    if ( !m_currentScritFileName.isEmpty() )
        title = QFileInfo(fileName).fileName();
    setWindowTitle(tr("%1[*] - Tonatiuh").arg(title));

    QString time = QDateTime::currentDateTime().toString();
    QString message = QString("[%1]\t Current script file: '%2'").arg(time, m_currentScritFileName);
    WriteMessage(message);
}

/*!
 * Write the \a message at log window.
 */
void ScriptEditorDialog::WriteMessage(QString message)
{
    ui->logWidget->appendPlainText(message);
}

QScriptValue ScriptEditorDialog::ImportExtension(QScriptContext *context, QScriptEngine* engine)
{
    if (context->argumentCount() != 1)
        return QScriptValue();

    const QString name = context->argument(0).toString();

    if (!engine->importExtension(name).isUndefined())
    {
        QScriptValue consoleObject = engine->globalObject().property("console");
        QPlainTextEdit* console = (QPlainTextEdit*) consoleObject.toQObject();
        if (!console) return 0;

        QString message = QString("Warning! %1 not found!").arg(name);
        console->insertPlainText(message);
    }

    return QScriptValue();
}

QScriptValue ScriptEditorDialog::PrintMessage(QScriptContext* context, QScriptEngine* engine)
{
    QScriptValue object = engine->globalObject().property("console");
    QPlainTextEdit* console = (QPlainTextEdit*) object.toQObject();
    if (!console) return 0;

    if (context->argumentCount() < 1) return 0;
    if (!context->argument(0).isString()) return 0;

    QString message = context->argument(0).toString();
    console->insertPlainText(message);

    return 1;
}
