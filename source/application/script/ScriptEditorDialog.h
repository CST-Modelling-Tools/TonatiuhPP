#pragma once

#include <QDialog>
#include <QScriptValue>

#include "ui_scripteditordialog.h"

class FilesModel;
class QItemSelectionModel;
class QLineEdit;
class QScriptContext;
class QScriptEngine;
class TPhotonMapFactory;
class RandomDeviateFactory;

//!  ScriptEditorDialog class is the dialog to edit and run scripts with Tonatiuh.
/*!
  ScriptEditorDialog allow to the user open, edit, run and save scripts to automate the ray tracing.
*/

class ScriptEditorDialog : public QDialog, private Ui::ScriptEditorDialog
{
    Q_OBJECT

public:
    ScriptEditorDialog( QVector< RandomDeviateFactory* > listRandomDeviateFactory, QWidget* parent = 0 );
    ~ScriptEditorDialog();

    void ExecuteScript( QString tonatiuhScriptFile );


    static QScriptValue ImportExtension(QScriptContext *context, QScriptEngine *engine);
    static QScriptValue PrintMessage( QScriptContext* context, QScriptEngine* engine );

public slots:
    void AbortEvaluation( QString error );

protected:
    void closeEvent( QCloseEvent* event );

private slots:
    void Close( bool checked  );
    void OpenScriptFile( const QModelIndex& index );
    void RunScript();
    void SetCurrentFile( QString fileName );
    void WriteMessage(  QString message );

private:
    QString m_currentScritFileName;
    FilesModel* m_fileModel;
    QScriptEngine* m_interpreter;
};
