#pragma once

#include <QMap>
#include <QString>

#include "kernel/photons/PhotonsAbstract.h"

class Photon;

class PhotonsFile: public PhotonsAbstract
{

public:
    PhotonsFile();
    ~PhotonsFile() {}

	static QStringList GetParameterNames();

	void EndExport();
    void SavePhotonMap(const std::vector<Photon>& photons);
    void SetPowerPerPhoton(double wPhoton) {m_powerPerPhoton = wPhoton;}
    void SetSaveParameterValue(QString parameterName, QString parameterValue);
	bool StartExport();

    static const char* getClassName() {return "File";}
    static const char* getClassIcon() {return ":/PhotonsFile.png";}
    const char* getIcon() const {return getClassIcon();}

private:
    void ExportAllPhotonsAllData(QString filename, const std::vector<Photon>& raysLists);
    void ExportAllPhotonsNotNextPrevID(QString filename, const std::vector<Photon>& raysLists);
    void ExportAllPhotonsSelectedData(QString filename, const std::vector<Photon>& raysLists);
    void ExportSelectedPhotonsAllData(QString filename, const std::vector<Photon>& raysLists,
                                      ulong startIndex, ulong numberOfPhotons);
    void ExportSelectedPhotonsNotNextPrevID(QString filename, const std::vector<Photon>&,
                                            ulong startIndex, ulong numberOfPhotons);
    void ExportSelectedPhotonsSelectedData(QString filename, const std::vector<Photon>&,
                                           ulong startIndex, ulong numberOfPhotons);

    void RemoveExistingFiles();
    void SaveToVariousFiles(const std::vector<Photon>& raysLists);
    void WriteFileFormat(QString exportFilename);

	QString m_photonsFilename;
	double m_powerPerPhoton;
	QVector<InstanceNode*> m_surfaceIdentfier;
	QVector<Transform> m_surfaceWorldToObject;
	int m_currentFile;
    QString m_exportDirectoryName;
    ulong m_exportedPhotons;
    ulong m_nPhotonsPerFile;
	bool m_oneFile;
};



#include "kernel/photons/PhotonsFactory.h"
#include "PhotonsFileWidget.h"

class PhotonsFileFactory:
    public QObject,
    public PhotonsFactoryT<PhotonsFile, PhotonsFileWidget>
{
    Q_OBJECT
    Q_INTERFACES(PhotonsFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.PhotonsFactory")
};
