#pragma once

#include <QVector>
#include <QMap>

class QDir;
class QStringList;
class QObject;

class TFactory;
class TTransmissivityFactory;
class TComponentFactory;
class PhotonExportFactory;
class TMaterialFactory;
class RandomDeviateFactory;
class TShapeFactory;
class TSunFactory;
class TTrackerFactory;

class PluginManager
{

public:
    PluginManager() {}

    QVector<TTransmissivityFactory*> getAirFactories() const {return m_airFactories;}
    QVector<TComponentFactory*> getComponentFactories() const {return m_componentFactories;}
    QVector<PhotonExportFactory*> getExportFactories() const {return m_exportFactories;}
    QVector<TMaterialFactory*> getMaterialFactories() const {return m_materialFactories;}
    QVector<RandomDeviateFactory*> getRandomFactories() const {return m_randomFactories;}
    QVector<TShapeFactory*> getShapeFactories() const {return m_shapeFactories;}
    QVector<TSunFactory*> getSunFactories() const {return m_sunFactories;}
    QVector<TTrackerFactory*> getTrackerFactories() const {return m_trackerFactories;}

    const QMap<QString, TTransmissivityFactory*>& getAirMap() const {return m_airMap;}
    const QMap<QString, PhotonExportFactory*>& getExportMap() const {return m_exportMap;}
    const QMap<QString, TMaterialFactory*>& getMaterialMap() const {return m_materialsMap;}
    const QMap<QString, TShapeFactory*>& getShapeMap() const {return m_shapesMap;}
    const QMap<QString, TSunFactory*>& getSunMap() const {return m_sunMap;}
    const QMap<QString, TTrackerFactory*>& getTrackerMap() const {return m_trackersMap;}

    void load(QDir dir);
    void sort();

private:
    void findFiles(QDir dir, QStringList& files);
    void loadTonatiuhPlugin(TFactory* p);

    QVector<TTransmissivityFactory*> m_airFactories;
    QVector<TComponentFactory*> m_componentFactories;
    QVector<PhotonExportFactory*> m_exportFactories;
    QVector<TMaterialFactory*> m_materialFactories;
    QVector<RandomDeviateFactory*> m_randomFactories;
    QVector<TShapeFactory*> m_shapeFactories;
    QVector<TSunFactory*> m_sunFactories;
    QVector<TTrackerFactory*> m_trackerFactories;

    QMap<QString, TTransmissivityFactory*> m_airMap;
    QMap<QString, PhotonExportFactory*> m_exportMap;
    QMap<QString, TMaterialFactory*> m_materialsMap;
    QMap<QString, TShapeFactory*> m_shapesMap;
    QMap<QString, TSunFactory*> m_sunMap;
    QMap<QString, TTrackerFactory*> m_trackersMap;
};
