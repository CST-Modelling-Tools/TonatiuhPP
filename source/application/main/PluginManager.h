#pragma once

#include <QVector>
#include <QMap>

class QDir;
class QStringList;
class QObject;

class TFactory;
class AirFactory;
class ComponentFactory;
class PhotonsFactory;
class MaterialFactory;
class RandomFactory;
class ShapeFactory;
class SunFactory;
class TrackerFactory;


/*!
 * \class PluginManager
 * \brief PluginManager class manages plugin loading.
 * PluginManager is used to load plugins, manage the list of loaded plugins.
 */
class PluginManager
{

public:
    PluginManager();

    QVector<AirFactory*> getAirFactories() const {return m_airFactories;}
    QVector<ComponentFactory*> getComponentFactories() const {return m_componentFactories;}
    QVector<PhotonsFactory*> getExportFactories() const {return m_exportFactories;}
    QVector<MaterialFactory*> getMaterialFactories() const {return m_materialFactories;}
    QVector<RandomFactory*> getRandomFactories() const {return m_randomFactories;}
    QVector<ShapeFactory*> getShapeFactories() const {return m_shapeFactories;}
    QVector<SunFactory*> getSunFactories() const {return m_sunFactories;}
    QVector<TrackerFactory*> getTrackerFactories() const {return m_trackerFactories;}

    const QMap<QString, AirFactory*>& getAirMap() const {return m_airMap;}
    const QMap<QString, PhotonsFactory*>& getExportMap() const {return m_exportMap;}
    const QMap<QString, MaterialFactory*>& getMaterialMap() const {return m_materialsMap;}
    const QMap<QString, RandomFactory*>& getRandomMap() const {return m_randomMap;}
    const QMap<QString, ShapeFactory*>& getShapeMap() const {return m_shapesMap;}
    const QMap<QString, SunFactory*>& getSunMap() const {return m_sunMap;}
    const QMap<QString, TrackerFactory*>& getTrackerMap() const {return m_trackersMap;}

    void load(QDir dir);
    void sort();

private:
    void findFiles(QDir dir, QStringList& files);
    void loadTonatiuhPlugin(TFactory* p);

    QVector<AirFactory*> m_airFactories;
    QVector<ComponentFactory*> m_componentFactories;
    QVector<PhotonsFactory*> m_exportFactories;
    QVector<MaterialFactory*> m_materialFactories;
    QVector<RandomFactory*> m_randomFactories;
    QVector<ShapeFactory*> m_shapeFactories;
    QVector<SunFactory*> m_sunFactories;
    QVector<TrackerFactory*> m_trackerFactories;

    QMap<QString, AirFactory*> m_airMap;
    QMap<QString, PhotonsFactory*> m_exportMap;
    QMap<QString, MaterialFactory*> m_materialsMap;
    QMap<QString, RandomFactory*> m_randomMap;
    QMap<QString, ShapeFactory*> m_shapesMap;
    QMap<QString, SunFactory*> m_sunMap;
    QMap<QString, TrackerFactory*> m_trackersMap;
};


