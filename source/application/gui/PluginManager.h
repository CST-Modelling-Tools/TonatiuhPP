#pragma once

#include <QVector>

class PhotonMapExportFactory;
class QDir;
class QStringList;
class RandomDeviateFactory;
class TComponentFactory;
class TMaterialFactory;
class TShapeFactory;
class TSunShapeFactory;
class TTrackerFactory;
class TTransmissivityFactory;

class PluginManager
{

public:
    PluginManager();
    ~PluginManager();

    QVector<TComponentFactory*> GetComponentFactories() const;
    QVector<PhotonMapExportFactory*> GetExportPMModeFactories() const;
    QVector<TMaterialFactory*> GetMaterialFactories() const;
    QVector<RandomDeviateFactory*> GetRandomDeviateFactories() const;
    QVector<TShapeFactory*> GetShapeFactories() const;
    QVector<TSunShapeFactory*> GetSunShapeFactories() const;
    QVector<TTrackerFactory*> GetTrackerFactories() const;
    QVector<TTransmissivityFactory*> GetTransmissivityFactories() const;
    void LoadAvailablePlugins(QDir pluginsDirectory);

private:
    void AddFilesToList(QDir directory, QStringList& filesList);
    void BuildFileList(QDir directory, QStringList& filesList);
    void LoadAvailablePlugins();
    void LoadComponentPlugin(QObject* plugin);
    void LoadExportPhotonMapModePlugin(QObject* plugin);
    void LoadMaterialPlugin(QObject* plugin);
    void LoadRandomDeviatePlugin(QObject* plugin);
    void LoadShapePlugin(QObject* plugin);
    void LoadSunshapePlugin(QObject* plugin);
    void LoadTrackerPlugin(QObject* plugin);
    void LoadTransmissivityPlugin(QObject* plugin);
    void LoadTonatiuhPlugin(const QString& fileName);
    bool ValidDirectoryName(QString& directoryName);

    QVector<TComponentFactory*> m_componentFactoryList;
    QVector<PhotonMapExportFactory*> m_exportPMModeFactoryList;
    QVector<TMaterialFactory*> m_materialFactoryList;
    QVector<RandomDeviateFactory*> m_randomDeviateFactoryList;
    QVector<TShapeFactory*> m_shapeFactoryList;
    QVector<TSunShapeFactory*> m_sunshapeFactoryList;
    QVector<TTrackerFactory*> m_trackerFactoryList;
    QVector<TTransmissivityFactory*> m_transmissivityFactoryList;
};
