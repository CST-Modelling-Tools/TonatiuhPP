#pragma once

#include <QMap>
#include <QString>

#include "kernel/photons/PhotonExport.h"

class Photon;

class PhotonExportFile: public PhotonExport
{

public:
	PhotonExportFile();
	virtual ~PhotonExportFile() {}

	static QStringList GetParameterNames();

	void EndExport();
    void SavePhotonMap(std::vector<Photon*> raysLists);
    void SetPowerPerPhoton(double wPhoton) {m_powerPerPhoton = wPhoton;}
    void SetSaveParameterValue(QString parameterName, QString parameterValue);
	bool StartExport();

    static const char* getClassName() {return "File";}
    static const char* getClassIcon() {return ":/PhotonExportFile.png";}
    const char* getIcon() const {return getClassIcon();}

private:
    void ExportAllPhotonsAllData(QString filename, std::vector<Photon*> raysLists);
    void ExportAllPhotonsNotNextPrevID(QString filename, std::vector<Photon*> raysLists);
    void ExportAllPhotonsSelectedData(QString filename, std::vector<Photon*> raysLists);
    void ExportSelectedPhotonsAllData(QString filename, std::vector<Photon*> raysLists,
                                      ulong startIndex, ulong numberOfPhotons);
    void ExportSelectedPhotonsNotNextPrevID(QString filename, std::vector<Photon*> raysLists,
                                            ulong startIndex, ulong numberOfPhotons);
    void ExportSelectedPhotonsSelectedData(QString filename, std::vector<Photon*> raysLists,
                                           ulong startIndex, ulong numberOfPhotons);

    void RemoveExistingFiles();
    void SaveToVariousFiles(std::vector<Photon*> raysLists);
    void WriteFileFormat(QString exportFilename);

	QString m_photonsFilename;
	double m_powerPerPhoton;
	QVector<InstanceNode*> m_surfaceIdentfier;
	QVector<Transform> m_surfaceWorldToObject;
	int m_currentFile;
    QString m_exportDirectoryName;
	unsigned long m_exportedPhotons;
	unsigned long m_nPhotonsPerFile;
	bool m_oneFile;
};



#include "kernel/photons/PhotonExportFactory.h"
#include "PhotonExportFileWidget.h"

class PhotonExportFileFactory:
    public QObject,
    public PhotonExportFactoryT<PhotonExportFile, PhotonExportFileWidget>
{
    Q_OBJECT
    Q_INTERFACES(PhotonExportFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.PhotonExportFactory")
};
