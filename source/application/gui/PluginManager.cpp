/*!
 * \class PluginManager
 * \brief PluginManager class manages plugin loading.
 * PluginManager is used to load plugins, manage the list of loaded plugins.
 */

#include "PluginManager.h"

#include <QDir>
#include <QPluginLoader>
#include <QStringList>
#include <iostream>

#include "libraries/geometry/gf.h"
#include "kernel/raytracing/TTransmissivityFactory.h"
#include "kernel/raytracing/TComponentFactory.h"
#include "kernel/photons/PhotonMapExportFactory.h"
#include "kernel/raytracing/TMaterialFactory.h"
#include "kernel/statistics/RandomDeviateFactory.h"
#include "kernel/raytracing/TShapeFactory.h"
#include "kernel/raytracing/TSunShapeFactory.h"
#include "kernel/raytracing/TTrackerFactory.h"


template <class T>
void sortFactories(const QStringList& sorting, QVector<T*>& factories) {
    int nA = 0;
    for (const QString& name : sorting) {
        if (name == "") {
            factories.append(nullptr);
            std::swap(factories[nA++], factories.last());
        } else {
            for (int n = 0; n < factories.size(); ++n) {
                if (factories[n] == nullptr) continue;
                if (factories[n]->name() == name) {
                    std::swap(factories[nA++], factories[n]);
                    break;
                }
            }
        }
    };
}


/*!
 * Loads all the valid plugins from "plugins" subdirecotry of the directory in
 * which the running version of Tonatiuh is located.
 */
void PluginManager::load(QDir dir)
{
    QStringList filesList;
    findFiles(dir, filesList);

    for (QString fileName: filesList)
        loadTonatiuhPlugin(fileName);
    sort();
}

void PluginManager::sort()
{
    QStringList shapeNames = {
        "Sphere",
        "Cylinder"
    };
    sortFactories(shapeNames, m_shapeFactories);

    QStringList exportNames = {
        "No export",
        "File"
    };
    sortFactories(exportNames, m_exportFactories);
}

/*!
 * Creates a list with the files al files in the defined \a directory
 * and its subdirectories.
 */
void PluginManager::findFiles(QDir dir, QStringList& files)
{
    QString path(dir.absolutePath().append("/"));

    QStringList fileList = dir.entryList(QDir::Files, QDir::Unsorted);
    for (QString file : fileList)
        files << path + file;

    QStringList dirList = dir.entryList(QDir::Dirs, QDir::Unsorted);
    for (QString subdir : dirList) {
        if (subdir == "."  || subdir == "..") continue;
        findFiles(QDir(path + subdir), files);
    }
}

/*!
 * Loads the plugin is defined in \a fileName, if it is a valid plugin.
 */
void PluginManager::loadTonatiuhPlugin(const QString& file)
{
    QPluginLoader loader(file);
    QObject* plugin = loader.instance();
    if (plugin) {
        if (dynamic_cast<TTransmissivityFactory*>(plugin)) loadAirPlugin(plugin);
        if (plugin->inherits("TComponentFactory") ) LoadComponentPlugin(plugin);
        if (dynamic_cast<PhotonMapExportFactory*>(plugin)) loadExportPlugin(plugin);
        if (dynamic_cast<TMaterialFactory*>(plugin)) loadMaterialPlugin(plugin);
        if (dynamic_cast<RandomDeviateFactory*>(plugin)) loadRandomPlugin(plugin);
        if (dynamic_cast<TShapeFactory*>(plugin)) loadShapePlugin(plugin);
        if (dynamic_cast<TSunShapeFactory*>(plugin)) loadSunPlugin(plugin);
        if (plugin->inherits("TTrackerFactory") ) loadTrackerPlugin(plugin);
    }
}

/*!
 * Loads \a plugin as transmissivity plugin.
 */
void PluginManager::loadAirPlugin(QObject* plugin)
{
    TTransmissivityFactory* f = qobject_cast<TTransmissivityFactory*>(plugin);
    if (!f) gf::SevereError("MainWindow::LoadPlugins: Transmissivity plug-in not recognized");
    f->create();
    m_airFactories << f;
}

/*!
 * Loads the \a plugin as component type.
 */
void PluginManager::LoadComponentPlugin(QObject* plugin)
{
    TComponentFactory* f = qobject_cast<TComponentFactory*>(plugin);
    if (!f) gf::SevereError("PluginManager::LoadComponentPlugin: Component plug-in not recognized");
    m_componentFactories << f;
    // template with void create
}

/*!
 * Loads the \a plugin as component type.
 */
void PluginManager::loadExportPlugin(QObject* plugin)
{
    PhotonMapExportFactory* f = qobject_cast<PhotonMapExportFactory*>(plugin);
    if (!f) gf::SevereError("PluginManager::LoadExportPhotonMapModePlugin: Component plug-in not recognized");
    m_exportFactories << f;
}

/*!
 * Loads the \a plugin as material type.
 */
void PluginManager::loadMaterialPlugin(QObject* plugin)
{
    TMaterialFactory* f = qobject_cast<TMaterialFactory*>(plugin);
    if (!f) gf::SevereError("MainWindow::LoadPlugins: Material plug-in not recognized");
    f->create();
    m_materialFactories << f;
}

/*!
 * Loads the \a plugin as RandomDeviatePlugin type.
 */
void PluginManager::loadRandomPlugin(QObject* plugin)
{
    RandomDeviateFactory* f = qobject_cast<RandomDeviateFactory*>(plugin);
    m_randomFactories << f;
}

void PluginManager::loadShapePlugin(QObject* plugin)
{
    TShapeFactory* f = qobject_cast<TShapeFactory*>(plugin);
    if (!f) gf::SevereError("MainWindow::LoadPlugins: Shape plug-in not recognized");
    f->create();
    m_shapeFactories << f;
}

void PluginManager::loadSunPlugin(QObject* plugin)
{
    TSunShapeFactory* f = qobject_cast<TSunShapeFactory*>(plugin);
    if (!f) gf::SevereError("MainWindow::LoadPlugins: SunShape plug-in not recognized"); ;
    f->create();
    m_sunFactories << f;
}

void PluginManager::loadTrackerPlugin(QObject* plugin)
{
    TTrackerFactory* f = qobject_cast< TTrackerFactory*>(plugin);
    if (!f) gf::SevereError("MainWindow::LoadPlugins: Tracker plug-in not recognized");
    f->create();
    m_trackerFactories << f;
}
