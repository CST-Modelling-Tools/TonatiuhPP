/***************************************************************************
Copyright (C) 2008 by the Tonatiuh Software Development Team.

This file is part of Tonatiuh.

Tonatiuh program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.


Acknowledgments:

The development of Tonatiuh was started on 2004 by Dr. Manuel J. Blanco,
then Chair of the Department of Engineering of the University of Texas at
Brownsville. From May 2004 to July 2008, it was supported by the Department
of Energy (DOE) and the National Renewable Energy Laboratory (NREL) under
the Minority Research Associate (MURA) Program Subcontract ACQ-4-33623-06.
During 2007, NREL also contributed to the validation of Tonatiuh under the
framework of the Memorandum of Understanding signed with the Spanish
National Renewable Energy Centre (CENER) on February, 20, 2007 (MOU#NREL-07-117).
Since June 2006, the development of Tonatiuh is being led by the CENER, under the
direction of Dr. Blanco, now Director of CENER Solar Thermal Energy Department.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victlor Martin.

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#ifndef EXPORTPHOTONMAPFILE_H_
#define EXPORTPHOTONMAPFILE_H_

#include <QMap>
#include <QString>

#include "PhotonMapExport.h"

class Photon;

class PhotonMapExportFile : public PhotonMapExport
{

public:
	PhotonMapExportFile();
	virtual ~PhotonMapExportFile();

	static QStringList GetParameterNames();

	void EndExport();
	void SavePhotonMap( std::vector< Photon* > raysLists );
	void SetPowerPerPhoton( double wPhoton );
	void SetSaveParameterValue( QString parameterName, QString parameterValue );
	bool StartExport();

private:
	void ExportAllPhotonsAllData( QString filename, std::vector< Photon* > raysLists );
	void ExportAllPhotonsNotNextPrevID( QString filename, std::vector< Photon* > raysLists );
	void ExportAllPhotonsSelectedData( QString filename, std::vector< Photon* > raysLists );
	void ExportSelectedPhotonsAllData( QString filename, std::vector< Photon* > raysLists,
			unsigned long startIndex, 	unsigned long numberOfPhotons );
	void ExportSelectedPhotonsNotNextPrevID( QString filename, std::vector< Photon* > raysLists,
			unsigned long startIndex, 	unsigned long numberOfPhotons );
	void ExportSelectedPhotonsSelectedData( QString filename, std::vector <Photon* > raysLists,
			unsigned long startIndex, 	unsigned long numberOfPhotons );


    void RemoveExistingFiles();
    void SaveToVariousFiles( std::vector <Photon* > raysLists );
    void WriteFileFormat( QString exportFilename );


	QString m_photonsFilename;
	double m_powerPerPhoton;
	QVector< InstanceNode* > m_surfaceIdentfier;
	QVector< Transform > m_surfaceWorldToObject;
	int m_currentFile;
	QString m_exportDirecotryName;
	unsigned long m_exportedPhotons;
	unsigned long m_nPhotonsPerFile;
	bool m_oneFile;

};


#endif /* EXPORTPHOTONMAPFILE_H_ */
