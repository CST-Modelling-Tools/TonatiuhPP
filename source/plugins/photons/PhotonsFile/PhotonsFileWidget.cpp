#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

#include "PhotonsFile.h"
#include "PhotonsFileWidget.h"

/*!
 * Creates a widget for the plugin parameters.
 */
PhotonsFileWidget::PhotonsFileWidget(QWidget* parent):
    PhotonsWidget(parent)
{
    setupUi(this);
	SetupTriggers();
}

/*!
 * Returns the plugin parameters names.
 */
QStringList PhotonsFileWidget::GetParameterNames() const
{
    return PhotonsFile::GetParameterNames();
}

/*!
 * Return the value of
 */
QString PhotonsFileWidget::GetParameterValue( QString parameter ) const
{
	QStringList parametersName = GetParameterNames();

    if( parameter == parametersName[0] ) //Directory name
		return saveDirectoryLine->text();
    else if (parameter == parametersName[1]) //File name.
		return filenameLine->text();


	//Maximum number of photons that a file can store.
	else if( parameter == parametersName[2] )
	{
		if( !photonsPerFileCheck->isChecked() )	return QString::number( -1 );
		else	return QString::number( nOfPhotonsSpin->value() );
	}

	return QString();
}

/*!
 * Select existing directory to save the data exported from the photon.
 */
void PhotonsFileWidget::SelectSaveDirectory()
{
    QSettings settings("CyI", "Tonatiuh");
    QString lastUsedDirectory = settings.value( "PhotonsFileWidget.directoryToExport",
            ".").toString();


	QString directoryToExport = QFileDialog::getExistingDirectory ( this, tr( "Save Direcotry" ), lastUsedDirectory );
	if( directoryToExport.isEmpty() )	return;


	QDir dirToExport( directoryToExport );
	if( !dirToExport.exists() )
	{
        QMessageBox::information( this, "Tonatiuh", tr( "Selected directory is not valid." ), 1 );
		return;

	}

    settings.setValue("PhotonsFileWidget.directoryToExport", directoryToExport );
	saveDirectoryLine->setText( directoryToExport );
}

/*!
 * Setups triggers for the buttons.
 */
void PhotonsFileWidget::SetupTriggers()
{
	connect( selectDirectoryButton, SIGNAL( clicked() ), this, SLOT( SelectSaveDirectory() ) );
}
