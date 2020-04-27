#pragma once

#include <QMap>
#include <QString>

#include "kernel/photons/PhotonMapExport.h"

class Photon;

class PhotonMapExportFile: public PhotonMapExport
{

public:
	PhotonMapExportFile();
	virtual ~PhotonMapExportFile();

	static QStringList GetParameterNames();

	void EndExport();
	void SavePhotonMap( std::vector< Photon* > raysLists );
    void SetPowerPerPhoton(double wPhoton);
	void SetSaveParameterValue( QString parameterName, QString parameterValue );
	bool StartExport();

    static const char* getClassName() {return "File";}
    static const char* getClassIcon() {return ":/PhotonMapExportFile.png";}
    const char* getIcon() const {return getClassIcon();}

private:
    void ExportAllPhotonsAllData(QString filename, std::vector< Photon* > raysLists);
    void ExportAllPhotonsNotNextPrevID(QString filename, std::vector< Photon* > raysLists);
    void ExportAllPhotonsSelectedData(QString filename, std::vector< Photon* > raysLists);
    void ExportSelectedPhotonsAllData(QString filename, std::vector< Photon* > raysLists,
                                      unsigned long startIndex,       unsigned long numberOfPhotons);
    void ExportSelectedPhotonsNotNextPrevID(QString filename, std::vector< Photon* > raysLists,
                                            unsigned long startIndex,       unsigned long numberOfPhotons);
    void ExportSelectedPhotonsSelectedData(QString filename, std::vector <Photon* > raysLists,
                                           unsigned long startIndex,       unsigned long numberOfPhotons);

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



#include "kernel/photons/PhotonMapExportFactory.h"
#include "PhotonMapExportFileWidget.h"

class PhotonMapExportFileFactory:
    public QObject,
    public PhotonExportFactory<PhotonMapExportFile, PhotonMapExportFileWidget>
{
    Q_OBJECT
    Q_INTERFACES(PhotonMapExportFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.PhotonMapExportFactory")
};
