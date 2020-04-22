#include "PhotonMapExportNull.h"

/*!
 * Creates an object that dont export photons.
 */
PhotonMapExportNull::PhotonMapExportNull()
:PhotonMapExport()
{

}

/*!
 * Destroys the object
 */
PhotonMapExportNull::~PhotonMapExportNull()
{

}

/*!
 * Returns an empty parameters name list.
 */
QStringList PhotonMapExportNull::GetParameterNames()
{
	return QStringList();
}

/*!
 * Nothing is done
 */
void PhotonMapExportNull::EndExport()
{

}

/*!
 * Nothing is done
 */
void PhotonMapExportNull::SavePhotonMap( std::vector< Photon* > /*raysLists*/ )
{

}

/*!
 * Nothing is done
 */
void PhotonMapExportNull::SetPowerPerPhoton( double /*wPhoton*/ )
{

}

/*!
 * Nothing is done
 */
void PhotonMapExportNull::SetSaveParameterValue( QString /*parameterName*/, QString /*parameterValue*/ )
{

}

/*!
 * Nothing is done
 */
bool PhotonMapExportNull::StartExport()
{
	return 1;
}
