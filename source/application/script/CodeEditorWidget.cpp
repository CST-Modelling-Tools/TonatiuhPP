#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QSettings>
#include <QTextBlock>
#include <QTextStream>

#include "CodeEditorWidget.h"
#include "LineNumberWidget.h"

/*!
 * Creates a new code editor widget.
 */
CodeEditorWidget::CodeEditorWidget(QWidget* parent, Qt::WindowFlags f):
    QWidget(parent, f),
    m_currentScritFileName("")
{
    setupUi(this);

    SetupToolbar();
    lineNumberWidget->SetCodeEditor(codeEditor);
    connect(
        codeEditor, SIGNAL( updateRequest(QRect, int) ),
        lineNumberWidget, SLOT(UpdateLineNumberArea(QRect,int) )
    );
}

CodeEditorWidget::~CodeEditorWidget()
{

}

QTextDocument* CodeEditorWidget::Document() const
{
    return codeEditor->document();
}

/*!
 * Returns \a true if the application is ready to start with other script. Otherwise,
 * returns \a false.
 */
bool CodeEditorWidget::OkToContinue()
{
    QTextDocument* document = codeEditor->document();

    if ( document->isModified() )
    {
        int answer = QMessageBox::warning( this, tr( "Tonatiuh" ),
                         tr( "The document has been modified.\n"
                             "Do you want to save your changes?"),
                         QMessageBox::Yes | QMessageBox::Default,
                         QMessageBox::No,
                         QMessageBox::Cancel | QMessageBox::Escape );

        if (answer == QMessageBox::Yes)
            return SaveScript();
        else if (answer == QMessageBox::Cancel)
            return false;
    }
    return true;
}

/*!
 * Opens the script saved at \a fileName.
 */
void CodeEditorWidget::OpenScriptFile(QString fileName)
{
    if (!OkToContinue()) return;
    StartDocument(fileName);
}

/*!
 * Creates a new empty script.
 *
 * Before creates the new scripts, it verifies if the current script can be closed.
 */
void CodeEditorWidget::NewScriptFile()
{
    if( !OkToContinue() ) return;

    QTextDocument* document = codeEditor->document();
    document->clear();

    m_currentScritFileName = "";
    emit FileOpened("");

    document->setModified(false);
}

/*!
 * Opens a script file into the script editor and sets this file as current file.
 *
 * A dialog is created to select an existing script file.
 *
 * Before open the selected script, it verifies if the current script can be closed.
 */
void CodeEditorWidget::OpenScriptFile()
{
    if (!OkToContinue()) return;

    QSettings settings("CyI", "Tonatiuh");
    QString dirName = settings.value("script.openDirectory", "../examples").toString();

    QString fileName = QFileDialog::getOpenFileName(
        this, "Open File", dirName,
        "Tonatiuh script file (*.tnhs)"
    );
    if (fileName.isEmpty()) return;

    QFileInfo file(fileName);
    settings.setValue("script.openDirectory", file.path());

    StartDocument(fileName);
}

/**
 * Executes the code editor script.
 */
void CodeEditorWidget::Run()
{
     emit RunScript();
}

/*!
 * Returns \a true if script is correctly saved. Otherwise, returns \a false. A file dialog is created to select a file.
 *
 * \sa SaveScriptFile, SaveFile.
 */
bool CodeEditorWidget::SaveAsScriptFile()
{
    QSettings settings("CyI", "Tonatiuh");
    QString dirName = settings.value("script.saveDirectory", "../examples").toString();

    QString fileName = QFileDialog::getSaveFileName(
        this, "Save File", dirName,
        "Tonatiuh script file (*.tnhs)"
    );
    if( fileName.isEmpty() ) return false;

    QFileInfo file(fileName);
    settings.setValue("script.saveDirectory", file.path());

    return SaveScriptFile(fileName);
}

/*!
 * Saves the editor code in the file defined as current. If a current file is not defined, it calls to \a SaveAsScriptFile to define a current file.
 *
 * \sa SaveScriptFile, SaveAsScriptFile.
 */
bool CodeEditorWidget::SaveScript()
{
    if ( m_currentScritFileName.isEmpty() ) return SaveAsScriptFile();
    else return SaveScriptFile( m_currentScritFileName );
}

/*!
 * Returns \a true if the scripts is correctly saved into the the given \a fileName. Otherwise,
 * returns \a false.
 *
 * \sa SaveAsScriptFile, SaveScript.
 */
bool CodeEditorWidget::SaveScriptFile( const QString& fileName )
{
    QTextDocument* document = codeEditor->document();

    QFile scriptFile(fileName);
    if (!scriptFile.open(QIODevice::WriteOnly) )
    {
        QMessageBox::warning(this, tr("Tonatiuh warning"),
                             tr("Cannot open file %1.")
                             .arg(fileName) );
        return false;
    }

    QTextStream out(&scriptFile);
    out << document->toPlainText();

    scriptFile.close();
    document->setModified(false);


    m_currentScritFileName = fileName;
    emit FileSaved(fileName);
    return true;
}

/*!
 * Updates code editor width.
 */
void CodeEditorWidget::UpdateCodeEditorWidth( int /* width*/ )
{
    QWidget* viewport = codeEditor->viewport();
    viewport->setContentsMargins( lineNumberWidget->LineNumberAreaWidth(), 0, 0, 0 );
}

/*!
 * Creates connector for toolbar buttons.
 */
void CodeEditorWidget::SetupToolbar()
{
    connect(newButton, SIGNAL(clicked(bool)), this, SLOT(NewScriptFile()) );
    connect(openButton, SIGNAL(clicked(bool)), this, SLOT(OpenScriptFile()) );
    connect(saveButton, SIGNAL(clicked(bool)), this, SLOT(SaveScript()) );
    connect(saveAsButton, SIGNAL(clicked(bool)), this, SLOT(SaveAsScriptFile()) );

    connect(undoButton, SIGNAL(clicked(bool)), codeEditor, SLOT(undo()) );
    connect(redoButton, SIGNAL(clicked(bool)), codeEditor, SLOT(redo()) );

    connect(cutButton, SIGNAL(clicked(bool)), codeEditor, SLOT(cut()) );
    connect(copyButton, SIGNAL(clicked(bool)), codeEditor, SLOT(copy()) );
    connect(pasteButton, SIGNAL(clicked(bool)), codeEditor, SLOT(paste()) );

    connect(runButton, SIGNAL(clicked(bool)), this, SLOT(Run()) );
}

/*!
 * Sets the code editor's contents to \a fileName contents and sets the currents file to \a fileName.
 */
void CodeEditorWidget::StartDocument(QString fileName)
{
    QFile scriptFile(fileName);
    if( !scriptFile.open( QIODevice::ReadOnly) )
    {
        QMessageBox::warning( this, tr( "Tonatiuh warning" ),
                                      tr( "Cannot open file %1." )
                                    .arg( fileName ) );
        return;
    }

    QTextStream in(&scriptFile);

    QTextDocument* document = codeEditor->document();
    document->clear();
    document->setPlainText(in.readAll() );
    scriptFile.close();

    m_currentScritFileName = fileName;
    emit FileOpened(fileName);
    document->setModified(false);
}
