#pragma once

#include <QMap>
#include <QString>

#include "kernel/gui/PhotonMapExport.h"

class Photon;

class PhotonMapExportNull : public PhotonMapExport
{

public:
	PhotonMapExportNull();
	virtual ~PhotonMapExportNull();

	static QStringList GetParameterNames();

	void EndExport();
	void SavePhotonMap( std::vector< Photon* > raysLists );
	void SetPowerPerPhoton( double wPhoton );
	void SetSaveParameterValue( QString parameterName, QString parameterValue );
	bool StartExport();
};
