#include "CodeEditorWidget.h"
#include "ui_CodeEditorWidget.h"

#include <QMessageBox>
#include <QSettings>
#include <QFileDialog>
#include <QTextStream>

#include "LineNumbers.h"


CodeEditorWidget::CodeEditorWidget(QWidget* parent, Qt::WindowFlags f):
    QWidget(parent, f),
    ui(new Ui::CodeEditorWidget)
{
    ui->setupUi(this);

    // toolbar
    connect(ui->newButton, SIGNAL(clicked(bool)), this, SLOT(openNew()) );
    connect(ui->openButton, SIGNAL(clicked(bool)), this, SLOT(open()) );
    connect(ui->saveButton, SIGNAL(clicked(bool)), this, SLOT(saveThis()) );
    connect(ui->saveAsButton, SIGNAL(clicked(bool)), this, SLOT(save()) );

    connect(ui->undoButton, SIGNAL(clicked(bool)), ui->codeEditor, SLOT(undo()) );
    connect(ui->redoButton, SIGNAL(clicked(bool)), ui->codeEditor, SLOT(redo()) );

    connect(ui->cutButton, SIGNAL(clicked(bool)), ui->codeEditor, SLOT(cut()) );
    connect(ui->copyButton, SIGNAL(clicked(bool)), ui->codeEditor, SLOT(copy()) );
    connect(ui->pasteButton, SIGNAL(clicked(bool)), ui->codeEditor, SLOT(paste()) );

    connect(ui->runButton, SIGNAL(clicked(bool)), this, SLOT(run()) );
}

CodeEditorWidget::~CodeEditorWidget()
{
    delete ui;
}

QTextDocument* CodeEditorWidget::document() const
{
    return ui->codeEditor->document();
}

bool CodeEditorWidget::isReady()
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
        return saveThis();
    if (answer == QMessageBox::No)
        return true;
    return false;
}

void CodeEditorWidget::open(QString fileName)
{
    if (!isReady()) return;

    if (fileName.isEmpty())
    {
        QSettings settings("CyI", "Tonatiuh");
        QString dirName = settings.value("script.openDirectory", "../examples").toString();

        fileName = QFileDialog::getOpenFileName(
            this, "Open File", dirName,
            "Tonatiuh script file (*.tnhs)"
        );
        if (fileName.isEmpty()) return;

        QFileInfo fileInfo(fileName);
        settings.setValue("script.openDirectory", fileInfo.path());
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(
            this, "Tonatiuh",
            tr("Cannot open file %1.").arg(fileName)
        );
        return;
    }
    QTextStream in(&file);

    QTextDocument* document = ui->codeEditor->document();
    document->clear();
    document->setPlainText(in.readAll());
    document->setModified(false);

    m_fileName = fileName;
    emit opened(m_fileName);
}

void CodeEditorWidget::openNew()
{
    if (!isReady()) return;

    QTextDocument* document = ui->codeEditor->document();
    document->clear();
    document->setModified(false);

    m_fileName = "";
    emit opened(m_fileName);
}

bool CodeEditorWidget::save(QString fileName)
{
    if (fileName.isEmpty())
    {
        QSettings settings("CyI", "Tonatiuh");
        QString dirName = settings.value("script.saveDirectory", "../examples").toString();

        QString fileName = QFileDialog::getSaveFileName(
            this, "Save File", dirName,
            "Tonatiuh script file (*.tnhs)"
        );
        if (fileName.isEmpty()) return false;

        QFileInfo fileInfo(fileName);
        settings.setValue("script.saveDirectory", fileInfo.path());
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(
            this, "Tonatiuh",
            tr("Cannot open file %1.").arg(fileName)
        );
        return false;
    }
    QTextStream out(&file);

    QTextDocument* document = ui->codeEditor->document();
    out << document->toPlainText();
    document->setModified(false);

    m_fileName = fileName;
    emit saved(m_fileName);
    return true;
}

bool CodeEditorWidget::saveThis()
{
    return save(m_fileName);
}

void CodeEditorWidget::run()
{
     emit runned();
}
