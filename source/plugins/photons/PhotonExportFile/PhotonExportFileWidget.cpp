#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

#include "PhotonExportFile.h"
#include "PhotonExportFileWidget.h"

/*!
 * Creates a widget for the plugin parameters.
 */
PhotonExportFileWidget::PhotonExportFileWidget(QWidget* parent):
    PhotonExportWidget(parent)
{
    setupUi(this);
	SetupTriggers();
}

/*!
 * Returns the plugin parameters names.
 */
QStringList PhotonExportFileWidget::GetParameterNames() const
{
    return PhotonExportFile::GetParameterNames();
}

/*!
 * Return the value of
 */
QString PhotonExportFileWidget::GetParameterValue( QString parameter ) const
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
void PhotonExportFileWidget::SelectSaveDirectory()
{
    QSettings settings("NREL UTB CENER", "Tonatiuh");
    QString lastUsedDirectory = settings.value( "PhotonExportFileWidget.directoryToExport",
            ".").toString();


	QString directoryToExport = QFileDialog::getExistingDirectory ( this, tr( "Save Direcotry" ), lastUsedDirectory );
	if( directoryToExport.isEmpty() )	return;


	QDir dirToExport( directoryToExport );
	if( !dirToExport.exists() )
	{
        QMessageBox::information( this, "Tonatiuh", tr( "Selected directory is not valid." ), 1 );
		return;

	}

    settings.setValue("PhotonExportFileWidget.directoryToExport", directoryToExport );
	saveDirectoryLine->setText( directoryToExport );
}

/*!
 * Setups triggers for the buttons.
 */
void PhotonExportFileWidget::SetupTriggers()
{
	connect( selectDirectoryButton, SIGNAL( clicked() ), this, SLOT( SelectSaveDirectory() ) );
}
