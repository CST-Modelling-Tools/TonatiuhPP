#include <iostream>

#include <QDataStream>
#include <QTextStream>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>

#include "PhotonsFile.h"
#include "kernel/run/InstanceNode.h"
//#include "kernel/gui/SceneModel.h" ?

/*!
 * Creates export object to export photon map photons to a file.
 */
PhotonsFile::PhotonsFile():
    PhotonsAbstract(),
    m_photonsFilename("PhotonMap"),
    m_powerPerPhoton(0.),
    m_currentFile(1),
    m_exportDirectoryName(""),
    m_exportedPhotons(0),
    m_nPhotonsPerFile(-1),
    m_oneFile(true)
{

}

/*!
 * Returns the plugin parameters names.
 */
QStringList PhotonsFile::GetParameterNames()
{
    return {"ExportDirectory", "ExportFile", "FileSize"};
}

/*!
 *
 */
void PhotonsFile::EndExport()
{
    QDir exportDirectory(m_exportDirectoryName);
	QString exportFilename;
    if (m_oneFile)
        exportFilename = exportDirectory.absoluteFilePath( QString("%1_parameters.txt").arg( m_photonsFilename ) );
	else
	{
        QString newName = QString("%1_parameters.txt").arg(m_photonsFilename);
        exportFilename = exportDirectory.absoluteFilePath(newName);
	}

    QFile exportFile(exportFilename);

    WriteFileFormat(exportFilename);
    exportFile.open(QIODevice::Append);
    QTextStream out(&exportFile);

    out << m_powerPerPhoton;
}

/*!
 * Saves \a raysList photons to file.
 */
void PhotonsFile::SavePhotonMap(const std::vector<Photon>& photons)
{
    if (m_oneFile)
	{
        QDir exportDirectory(m_exportDirectoryName);
		QString filename = m_photonsFilename;
        QString exportFilename = exportDirectory.absoluteFilePath(filename.append(".dat") );

        if (m_saveCoordinates && m_saveSide && m_savePrevNexID && m_saveSurfaceID)
            ExportAllPhotonsAllData(exportFilename, photons);
        else if (m_saveCoordinates && m_saveSide && !m_savePrevNexID && m_saveSurfaceID)
            ExportAllPhotonsNotNextPrevID(exportFilename, photons);
		else
            ExportAllPhotonsSelectedData(exportFilename, photons);
	}
	else
        SaveToVariousFiles(photons);
}

/*!
 * Sets to parameter \a parameterName the value \a parameterValue.
 */
void PhotonsFile::SetSaveParameterValue(QString parameterName, QString parameterValue)
{
	QStringList parameters = GetParameterNames();

	//Directory name
    if (parameterName == parameters[0])
        m_exportDirectoryName = parameterValue;

	//File name
    else if (parameterName == parameters[1])
	{
		m_photonsFilename = parameterValue;

	}

	//Maximum number of photons that a file can store.
    else if (parameterName == parameters[2])
	{
        if (parameterValue.toDouble() < 0) m_oneFile = true;
		else
		{
			m_oneFile = false;
            m_nPhotonsPerFile = ( ulong ) parameterValue.toDouble();
		}
	}
}

/*!
 * Deletes the files that can be uset to export.
 */
bool PhotonsFile::StartExport()
{
    if (m_exportedPhotons < 1) RemoveExistingFiles();
	return 1;
}

/*!
 * Export \a a raysList all data to file \a filename.
 */
void PhotonsFile::ExportAllPhotonsAllData(QString filename, const std::vector<Photon>& raysLists)
{
    QFile exportFile(filename);
    exportFile.open(QIODevice::Append);

    QDataStream out(&exportFile);

    ulong nPhotonElements = raysLists.size();
	if( m_saveCoordinatesInGlobal )
	{
		double previousPhotonID = 0;
        for( ulong i = 0; i < nPhotonElements; ++i )
		{

            const Photon& photon = raysLists[i];
            ulong urlId = 0;
            if( photon.intersectedSurface )
			{
                if( !m_surfaceIdentfier.contains( photon.intersectedSurface ) )
				{
                    m_surfaceIdentfier.push_back( photon.intersectedSurface );
					urlId = m_surfaceIdentfier.size();
                    m_surfaceWorldToObject.push_back( photon.intersectedSurface->GetIntersectionTransform() );
				}
				else
                    urlId = m_surfaceIdentfier.indexOf( photon.intersectedSurface ) + 1;
			}

            out << double(++m_exportedPhotons);
            if (photon.id < 1)	previousPhotonID = 0;


			//m_saveCoordinates
            Point3D scenePos = m_transform(photon.pos);
            out << scenePos.x << scenePos.y << scenePos.z;

			//m_saveSide
            double side = double(photon.side);
            out << side;

			//m_savePrevNexID
            out << previousPhotonID;
            if( ( i < ( nPhotonElements - 1 ) ) && ( raysLists[i+1].id > 0  ) )
                out << double( m_exportedPhotons +1 );
			else
                out << 0.;

			//m_saveSurfaceID
            out << double(urlId);

			previousPhotonID = m_exportedPhotons;
		}
	}
	else
	{
		double previousPhotonID = 0;
        for( ulong i = 0; i < nPhotonElements; ++i )
		{

            const Photon& photon = raysLists[i];

            out << double( ++m_exportedPhotons );
            if (photon.id < 1)	previousPhotonID = 0;

            ulong urlId = 0;
			Transform worldToObject( 1.0, 0.0, 0.0, 0.0,
					0.0, 1.0, 0.0, 0.0,
					0.0, 0.0, 1.0, 0.0,
					0.0, 0.0, 0.0, 1.0 );
            if( photon.intersectedSurface )
			{

                if( !m_surfaceIdentfier.contains( photon.intersectedSurface ) )
				{
                    m_surfaceIdentfier.push_back( photon.intersectedSurface );
					urlId = m_surfaceIdentfier.size();
                    worldToObject = photon.intersectedSurface->GetIntersectionTransform();
					m_surfaceWorldToObject.push_back( worldToObject );
				}
				else
				{
                    urlId = m_surfaceIdentfier.indexOf( photon.intersectedSurface ) ;
					worldToObject = m_surfaceWorldToObject[urlId];
					urlId++;
				}
			}

            Point3D localPos = worldToObject(photon.pos);
            out << localPos.x << localPos.y << localPos.z;

            double side = double(photon.side);
            out << side;

			out<<previousPhotonID;
            if( ( i < nPhotonElements - 1 ) && ( raysLists[i+1].id > 0  ) )	out<< double( m_exportedPhotons +1 );
			else out <<0.0;

			//m_saveSurfaceID
            out << double(urlId);

			previousPhotonID = m_exportedPhotons;
		}
	}
	exportFile.close();
}

/*!
 * Exports \a raysLists photons data except previous and next photon identifier to file \a filename.
 */
void PhotonsFile::ExportAllPhotonsNotNextPrevID(QString filename, const std::vector<Photon>& raysLists )
{

	QFile exportFile( filename );
	exportFile.open( QIODevice::Append );

	QDataStream out( &exportFile );
	if( m_saveCoordinatesInGlobal )
	{
        ulong nPhotons = raysLists.size();
        for( ulong i = 0; i < nPhotons; ++i )
		{
            const Photon& photon = raysLists[i];
            ulong urlId = 0;
            if( photon.intersectedSurface )
			{
                if( !m_surfaceIdentfier.contains( photon.intersectedSurface ) )
				{
                    m_surfaceIdentfier.push_back( photon.intersectedSurface );
					urlId = m_surfaceIdentfier.size();
                    m_surfaceWorldToObject.push_back( photon.intersectedSurface->GetIntersectionTransform() );
				}
				else
                    urlId = m_surfaceIdentfier.indexOf( photon.intersectedSurface ) + 1;

			}

			out<<double( ++m_exportedPhotons );

			//m_saveCoordinates
            Point3D scenePos = m_transform( photon.pos );
			out<<scenePos.x << scenePos.y << scenePos.z;
            //out<<photon.pos.x << photon.pos.y << photon.pos.z;

			//m_saveSide
            out<<double( photon.side );

			//m_saveSurfaceID
			out<<double( urlId );
		}
	}
	else
	{

        ulong nPhotons = raysLists.size();
        for (ulong i = 0; i < nPhotons; ++i)
		{
            const Photon& photon = raysLists[i];
            ulong urlId = 0;
			Transform worldToObject( 1.0, 0.0, 0.0, 0.0,
							0.0, 1.0, 0.0, 0.0,
							0.0, 0.0, 1.0, 0.0,
							0.0, 0.0, 0.0, 1.0 );
            if( photon.intersectedSurface )
			{
                if( !m_surfaceIdentfier.contains( photon.intersectedSurface ) )
				{
                    m_surfaceIdentfier.push_back( photon.intersectedSurface );
					urlId = m_surfaceIdentfier.size();
                    worldToObject = photon.intersectedSurface->GetIntersectionTransform();
					m_surfaceWorldToObject.push_back( worldToObject );
				}
				else
				{
                    urlId = m_surfaceIdentfier.indexOf( photon.intersectedSurface ) ;
					worldToObject = m_surfaceWorldToObject[urlId];
					urlId++;
				}

			}
			out<<double( ++m_exportedPhotons );

            Point3D localPos = worldToObject( photon.pos );
			out<<localPos.x << localPos.y << localPos.z;

            out << double(photon.side);

            out << double(urlId);
		}

	}
	exportFile.close();
}

/*!
 * Exports \a raysLists all photons data to file \a filename.
 * For each photon only selected parameters will be exported.
 */
void PhotonsFile::ExportAllPhotonsSelectedData(QString filename, const std::vector<Photon>& raysLists )
{
	QFile exportFile( filename );
	exportFile.open( QIODevice::Append );

	QDataStream out( &exportFile );

	double previousPhotonID = 0;
    ulong nPhotons = raysLists.size();
    for( ulong i = 0; i < nPhotons; ++i )
	{
        const Photon& photon = raysLists[i];
        ulong urlId = 0;
		Transform worldToObject( 1.0, 0.0, 0.0, 0.0,
							0.0, 1.0, 0.0, 0.0,
							0.0, 0.0, 1.0, 0.0,
							0.0, 0.0, 0.0, 1.0 );
        if( photon.intersectedSurface )
		{
            if( !m_surfaceIdentfier.contains( photon.intersectedSurface ) )
			{
                m_surfaceIdentfier.push_back( photon.intersectedSurface );
				urlId = m_surfaceIdentfier.size();
                worldToObject = photon.intersectedSurface->GetIntersectionTransform();
				m_surfaceWorldToObject.push_back( worldToObject );
			}
			else
			{
                urlId = m_surfaceIdentfier.indexOf( photon.intersectedSurface ) ;
				worldToObject = m_surfaceWorldToObject[urlId];
				urlId++;
			}

		}

		out<<double( ++m_exportedPhotons );
        if( photon.id < 1 )	previousPhotonID = 0;

        if( m_saveCoordinates && m_saveCoordinatesInGlobal )	out<<photon.pos.x << photon.pos.y << photon.pos.z;
		else if( m_saveCoordinates && !m_saveCoordinatesInGlobal )
		{
            Point3D localPos = worldToObject( photon.pos );
			out<<localPos.x << localPos.y << localPos.z;
		}

		if(  m_saveSide )
		{
            double side = double( photon.side );
			out<<side;
		}
		if( m_savePrevNexID )
		{
			out<<previousPhotonID;
            if (i < nPhotons - 1 && raysLists[i+1].id > 0)	out << double( m_exportedPhotons +1 );
			else out <<0.0;
		}

		if( m_saveSurfaceID )
			out<<double( urlId );

		previousPhotonID = m_exportedPhotons;

	}
	exportFile.close();
}

/*!
 * Exports \a numberOfPhotons photons from \a raysLists to file \a filename starting from [\a startIndexRaysList, \a endIndexRaysList ].
 */
void PhotonsFile::ExportSelectedPhotonsAllData(QString filename, const std::vector<Photon>& raysLists,
        ulong startIndex, 	ulong numberOfPhotons )
{

	QFile exportFile( filename );
	exportFile.open( QIODevice::Append );

	QDataStream out( &exportFile );

	unsigned int nPhotonElements = raysLists.size();
	if( m_saveCoordinatesInGlobal )
	{
		double previousPhotonID = 0;
        ulong exportedPhotonsToFile = 0;
		while( exportedPhotonsToFile < numberOfPhotons )
		{
            const Photon& photon = raysLists[startIndex + exportedPhotonsToFile];
            ulong urlId = 0;
            if( photon.intersectedSurface )
			{
                if( !m_surfaceIdentfier.contains( photon.intersectedSurface ) )
				{
                    m_surfaceIdentfier.push_back( photon.intersectedSurface );
					urlId = m_surfaceIdentfier.size();
                    m_surfaceWorldToObject.push_back( photon.intersectedSurface->GetIntersectionTransform() );
				}
				else
                    urlId = m_surfaceIdentfier.indexOf( photon.intersectedSurface ) + 1;
			}

			out<<double( ++m_exportedPhotons );
            if( photon.id < 1 )	previousPhotonID = 0;

			//m_saveCoordinates
            Point3D scenePos = m_transform( photon.pos );
			out<<scenePos.x << scenePos.y << scenePos.z;
            //out<<photon.pos.x << photon.pos.y << photon.pos.z;

			//m_saveSide
            double side = double( photon.side );
			out<<side;

			//m_savePrevNexID
			out<<previousPhotonID;
            if (startIndex + exportedPhotonsToFile < nPhotonElements - 1 && raysLists[startIndex + exportedPhotonsToFile + 1].id > 0)
                out << double( m_exportedPhotons +1 );
			else
                out << 0.0;


			//m_saveSurfaceID
			out<<double( urlId );

			previousPhotonID = m_exportedPhotons;
			exportedPhotonsToFile++;

		}
	}
	else
	{
		double previousPhotonID = 0;
        ulong exportedPhotonsToFile = 0;
		while( exportedPhotonsToFile < numberOfPhotons )
		{
            const Photon& photon = raysLists[startIndex + exportedPhotonsToFile];
            ulong urlId = 0;
			Transform worldToObject( 1.0, 0.0, 0.0, 0.0,
							0.0, 1.0, 0.0, 0.0,
							0.0, 0.0, 1.0, 0.0,
							0.0, 0.0, 0.0, 1.0 );
            if( photon.intersectedSurface )
			{
                if( !m_surfaceIdentfier.contains( photon.intersectedSurface ) )
				{
                    m_surfaceIdentfier.push_back( photon.intersectedSurface );
					urlId = m_surfaceIdentfier.size();
                    worldToObject = photon.intersectedSurface->GetIntersectionTransform();
					m_surfaceWorldToObject.push_back( worldToObject );
				}
				else
				{
                    urlId = m_surfaceIdentfier.indexOf( photon.intersectedSurface ) ;
					worldToObject = m_surfaceWorldToObject[urlId];
					urlId++;
				}
			}

            out << double( ++m_exportedPhotons );
            if (photon.id < 1)	previousPhotonID = 0;

			//m_saveCoordinates
            Point3D localPos = worldToObject( photon.pos );
			out<<localPos.x << localPos.y << localPos.z;

			//m_saveSide
            double side = double( photon.side );
			out<<side;

			//m_savePrevNexID
			out<<previousPhotonID;
            if( ( ( startIndex + exportedPhotonsToFile ) < ( nPhotonElements - 1 ) ) && ( raysLists[startIndex + exportedPhotonsToFile + 1].id > 0  ) )
				out<< double( m_exportedPhotons +1 );
			else
                out << 0.;

			//m_saveSurfaceID
			out<<double( urlId );

			previousPhotonID = m_exportedPhotons;
			exportedPhotonsToFile++;
		}

	}
	exportFile.close();
}

/*!
 * Exports \a numberOfPhotons photons from \a raysLists to file \a filename starting from [\a startIndexRaysList, \a endIndexRaysList ].
 */
void PhotonsFile::ExportSelectedPhotonsNotNextPrevID( QString filename, const std::vector<Photon>& raysLists,
        ulong startIndex, ulong numberOfPhotons )
{
	QFile exportFile( filename );
	exportFile.open( QIODevice::Append );

	QDataStream out( &exportFile );

    if (m_saveCoordinatesInGlobal)
	{
        ulong exportedPhotonsToFile = 0;
		while( exportedPhotonsToFile < numberOfPhotons )
		{
            const Photon& photon = raysLists[startIndex + exportedPhotonsToFile];
            ulong urlId = 0;
            if (photon.intersectedSurface)
			{
                if( !m_surfaceIdentfier.contains( photon.intersectedSurface ) )
				{
                    m_surfaceIdentfier.push_back( photon.intersectedSurface );
					urlId = m_surfaceIdentfier.size();
                    m_surfaceWorldToObject.push_back( photon.intersectedSurface->GetIntersectionTransform() );
				}
				else
                    urlId = m_surfaceIdentfier.indexOf( photon.intersectedSurface ) + 1;
			}

			out<<double( ++m_exportedPhotons );

			//m_saveCoordinates
            Point3D scenePos = m_transform( photon.pos );
			out<<scenePos.x << scenePos.y << scenePos.z;
            //out<<photon.pos.x << photon.pos.y << photon.pos.z;

			//m_saveSide
            double side = double( photon.side );
			out<<side;

			//m_saveSurfaceID
			out<<double( urlId );

			exportedPhotonsToFile++;
		}
	}
	else
	{
        ulong exportedPhotonsToFile = 0;
		while( exportedPhotonsToFile < numberOfPhotons )
		{
            const Photon& photon = raysLists[startIndex + exportedPhotonsToFile];
            ulong urlId = 0;
			Transform worldToObject( 1.0, 0.0, 0.0, 0.0,
							0.0, 1.0, 0.0, 0.0,
							0.0, 0.0, 1.0, 0.0,
							0.0, 0.0, 0.0, 1.0 );
            if( photon.intersectedSurface )
			{
                if( !m_surfaceIdentfier.contains( photon.intersectedSurface ) )
				{
                    m_surfaceIdentfier.push_back( photon.intersectedSurface );
					urlId = m_surfaceIdentfier.size();
                    worldToObject = photon.intersectedSurface->GetIntersectionTransform();
					m_surfaceWorldToObject.push_back( worldToObject );
				}
				else
				{
                    urlId = m_surfaceIdentfier.indexOf( photon.intersectedSurface ) ;
					worldToObject = m_surfaceWorldToObject[urlId];
					urlId++;
				}
			}

			out<<double( ++m_exportedPhotons );

			//m_saveCoordinates
            Point3D localPos = worldToObject( photon.pos );
			out<<localPos.x << localPos.y << localPos.z;

            out << double(photon.side);

            out << double(urlId);

			exportedPhotonsToFile++;
		}

	}
	exportFile.close();
}

/*!
 * Exports \a numberOfPhotons photons from \a raysLists to file \a filename starting from [\a startIndexRaysList, \a endIndexRaysList ].
 *  * For each photon only selected parameters will be exported.
 */
void PhotonsFile::ExportSelectedPhotonsSelectedData( QString filename, const std::vector<Photon>& raysLists,
        ulong startIndex, 	ulong numberOfPhotons )
{
    QFile exportFile(filename);
    exportFile.open(QIODevice::Append);

    QDataStream out(&exportFile);

	double previousPhotonID = 0;
    ulong nPhotonElements = raysLists.size();
    ulong exportedPhotonsToFile = 0;
	while( exportedPhotonsToFile < numberOfPhotons )
	{
        const Photon& photon = raysLists[startIndex + exportedPhotonsToFile];
        ulong urlId = 0;
		Transform worldToObject( 1.0, 0.0, 0.0, 0.0,
						0.0, 1.0, 0.0, 0.0,
						0.0, 0.0, 1.0, 0.0,
						0.0, 0.0, 0.0, 1.0 );
        if( photon.intersectedSurface )
		{
            if( !m_surfaceIdentfier.contains( photon.intersectedSurface ) )
			{
                m_surfaceIdentfier.push_back( photon.intersectedSurface );
				urlId = m_surfaceIdentfier.size();
                worldToObject = photon.intersectedSurface->GetIntersectionTransform();
				m_surfaceWorldToObject.push_back( worldToObject );
			}
			else
			{
                urlId = m_surfaceIdentfier.indexOf( photon.intersectedSurface ) ;
				worldToObject = m_surfaceWorldToObject[urlId];
				urlId++;
			}
		}

        out << double( ++m_exportedPhotons );
        if (photon.id < 1) previousPhotonID = 0;

		if( m_saveCoordinates && m_saveCoordinatesInGlobal )
		{

            Point3D scenePos = m_transform( photon.pos );
			out<<scenePos.x << scenePos.y << scenePos.z;
            //out<<photon.pos.x << photon.pos.y << photon.pos.z;
		}
		else if( m_saveCoordinates && !m_saveCoordinatesInGlobal )
		{
            Point3D localPos = worldToObject( photon.pos );
			out<<localPos.x << localPos.y << localPos.z;
		}

		if(  m_saveSide )
		{
            double side = double( photon.side );
			out<<side;
		}
		if( m_savePrevNexID )
		{
			out<<previousPhotonID;
			if( ( ( startIndex + exportedPhotonsToFile ) < nPhotonElements )
                    && ( raysLists[startIndex + exportedPhotonsToFile + 1].id > 0  ) )
                out<< double(m_exportedPhotons +1 );
            else out << 0.;
		}

		if( m_saveSurfaceID )
            out << double(urlId);

		previousPhotonID = m_exportedPhotons;
		exportedPhotonsToFile++;
	}
	exportFile.close();
}

/*!
 * Remove existing files that this export type can used.
 */
void PhotonsFile::RemoveExistingFiles()
{
    QDir exportDirectory( m_exportDirectoryName );
	QString filename = m_photonsFilename;
	if( m_oneFile )
	{
		QString exportFilename = exportDirectory.absoluteFilePath( filename.append( QLatin1String( ".dat" ) ) );
		QFile exportFile( exportFilename );
		if(exportFile.exists()&&!exportFile.remove()) {
				QString message= QString( "Error deleting %1.\nThe file is in use. Please, close it before continuing. \n" ).arg( QString( exportFilename ) );
				QMessageBox::warning( NULL, QLatin1String( "Tonatiuh" ), message );
				RemoveExistingFiles();
			}
	}
	else
	{
		QStringList filters;
        filters << filename.append("_*.dat");
		exportDirectory.setNameFilters(filters);

		QFileInfoList partialFilesList = exportDirectory.entryInfoList();
		for( int i = 0; i< partialFilesList.count(); ++i )
		{
			QFile partialFile( partialFilesList[i].absoluteFilePath() );
			if(partialFile.exists() && !partialFile.remove()) {
					QString message= QString( "Error deleting %1.\nThe file is in use. Please, close it before continuing. \n" ).arg( QString( partialFilesList[i].absoluteFilePath() ) );
					QMessageBox::warning( NULL, QLatin1String( "Tonatiuh" ), message );
					RemoveExistingFiles();
				}
		}
	}
}

/*!
 * Exports \a raysLists photons data to files with the same number of photons in each file.
 * Each file stores \a m_nPhotonsPerFile photons.
 */
void PhotonsFile::SaveToVariousFiles(const std::vector<Photon>& raysLists)
{
    QDir exportDirectory(m_exportDirectoryName);

    ulong nPhotons = raysLists.size();

    ulong startIndex = 0;
    ulong filePhotons = m_exportedPhotons - ( m_nPhotonsPerFile * ( m_currentFile - 1 ) );

    ulong nPhotonsToExport = 0;
    for (ulong i = 0; i < nPhotons; i++ )
	{
		if( !( filePhotons < m_nPhotonsPerFile ) )
		{
			QString newName = QString( QLatin1String( "%1_%2.dat" ) ).arg(
					m_photonsFilename,
					QString::number( m_currentFile ) );

			QString currentFileName = exportDirectory.absoluteFilePath( newName );

			if( m_saveCoordinates && m_saveSide && m_savePrevNexID && m_saveSurfaceID )
				ExportSelectedPhotonsAllData( currentFileName, raysLists, startIndex, nPhotonsToExport );
			else if( m_saveCoordinates && m_saveSide && !m_savePrevNexID && m_saveSurfaceID )
				ExportSelectedPhotonsNotNextPrevID( currentFileName, raysLists, startIndex, nPhotonsToExport );
			else
				ExportSelectedPhotonsSelectedData( currentFileName, raysLists, startIndex, nPhotonsToExport );
			m_currentFile++;

			filePhotons = 0;
			nPhotonsToExport = 0;
			startIndex = i;

		}
		filePhotons++;
		nPhotonsToExport++;
	}

    if (filePhotons > 0) {
        QString newName = QString("%1_%2.dat").arg(
            m_photonsFilename,
            QString::number(m_currentFile) );

        QString currentFileName = exportDirectory.absoluteFilePath(newName);

        if (m_saveCoordinates && m_saveSide && m_savePrevNexID && m_saveSurfaceID)
            ExportSelectedPhotonsAllData(currentFileName, raysLists, startIndex, nPhotonsToExport);
        else if (m_saveCoordinates && m_saveSide && !m_savePrevNexID && m_saveSurfaceID)
            ExportSelectedPhotonsNotNextPrevID(currentFileName, raysLists, startIndex, nPhotonsToExport);
        else
            ExportSelectedPhotonsSelectedData(currentFileName, raysLists, startIndex, nPhotonsToExport);
    }
}

/*!
 * Writes the file or first file header with the format.
 */
void PhotonsFile::WriteFileFormat(QString exportFilename)
{
    QFile file(exportFilename);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);

    out << "START PARAMETERS\n";
    out << "id\n";
    if (m_saveCoordinates) {
        out << "x\n";
        out << "y\n";
        out << "z\n";
	}
    if (m_saveSide)
        out << "side\n";
    if (m_savePrevNexID) {
        out << "previous ID\n";
        out << "next ID\n";
	}
    if (m_saveSurfaceID)
        out << "surface ID\n";
    out << "END PARAMETERS\n";


    out << "START SURFACES\n";
    for (int s = 0; s < m_surfaceIdentfier.size(); s++) {
		QString surfaceURL = m_surfaceIdentfier[s]->GetNodeURL();
        out << QString("%1 %2\n").arg(QString::number(s + 1), surfaceURL);
	}

    out << "END SURFACES\n";
}
