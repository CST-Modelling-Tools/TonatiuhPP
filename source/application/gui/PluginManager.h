#pragma once

#include <QVector>

class QDir;
class QStringList;
class QObject;

class TTransmissivityFactory;
class TComponentFactory;
class PhotonMapExportFactory;
class TMaterialFactory;
class RandomDeviateFactory;
class TShapeFactory;
class TSunShapeFactory;
class TTrackerFactory;

class PluginManager
{

public:
    PluginManager() {}

    QVector<TTransmissivityFactory*> getAirFactories() const
    {return m_airFactories;}
    QVector<TComponentFactory*> getComponentFactories() const
    {return m_componentFactories;}
    QVector<PhotonMapExportFactory*> getExportFactories() const
    {return m_exportFactories;}
    QVector<TMaterialFactory*> getMaterialFactories() const
    {return m_materialFactories;}
    QVector<RandomDeviateFactory*> getRandomFactories() const
    {return m_randomFactories;}
    QVector<TShapeFactory*> getShapeFactories() const
    {return m_shapeFactories;}
    QVector<TSunShapeFactory*> getSunFactories() const
    {return m_sunFactories;}
    QVector<TTrackerFactory*> getTrackerFactories() const
    {return m_trackerFactories;}

    void load(QDir dir);
    void sort();

private:
    void findFiles(QDir dir, QStringList& files);
    void loadTonatiuhPlugin(const QString& file);

    void loadAirPlugin(QObject* plugin);
    void LoadComponentPlugin(QObject* plugin);
    void loadExportPlugin(QObject* plugin);
    void loadMaterialPlugin(QObject* plugin);
    void loadRandomPlugin(QObject* plugin);
    void loadShapePlugin(QObject* plugin);
    void loadSunPlugin(QObject* plugin);
    void loadTrackerPlugin(QObject* plugin);

    QVector<TTransmissivityFactory*> m_airFactories;
    QVector<TComponentFactory*> m_componentFactories;
    QVector<PhotonMapExportFactory*> m_exportFactories;
    QVector<TMaterialFactory*> m_materialFactories;
    QVector<RandomDeviateFactory*> m_randomFactories;
    QVector<TShapeFactory*> m_shapeFactories;
    QVector<TSunShapeFactory*> m_sunFactories;
    QVector<TTrackerFactory*> m_trackerFactories;
};
