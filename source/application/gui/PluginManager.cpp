/*!
 * \class PluginManager
 * \brief PluginManager class manages plugin loading.
 * PluginManager is used to load plugins, manage the list of loaded plugins.
 */

#include <QDir>
#include <QPluginLoader>
#include <QStringList>
#include <iostream>

#include "libraries/geometry/gf.h"

#include "kernel/gui/PhotonMapExportFactory.h"
#include "PluginManager.h"
#include "kernel/statistics/RandomDeviateFactory.h"
#include "TComponentFactory.h"
#include "kernel/raytracing/TMaterialFactory.h"
#include "kernel/raytracing/TShapeFactory.h"
#include "kernel/raytracing/TSunShapeFactory.h"
#include "kernel/raytracing/TTrackerFactory.h"
#include "kernel/raytracing/TTransmissivityFactory.h"

/*!
 * Creates a new PluginManager object.
 */
PluginManager::PluginManager()
    : m_componentFactoryList(0),
    m_exportPMModeFactoryList(0),
    m_materialFactoryList(0),
    m_randomDeviateFactoryList(0),
    m_shapeFactoryList(0),
    m_sunshapeFactoryList(0)
{

}

/*!
 * Destroyes PluginManager object.
 */
PluginManager::~PluginManager()
{

}

/*!
 * Returns available component plugins factory list.
 */
QVector< TComponentFactory* > PluginManager::GetComponentFactories() const
{
    return m_componentFactoryList;
}

/*!
 *    Returns available export plugins factory list.
 */
QVector< PhotonMapExportFactory* > PluginManager::GetExportPMModeFactories() const
{
    return m_exportPMModeFactoryList;
}

/*!
 * Returns available random deviates plugins factory list.
 */
QVector< TMaterialFactory* > PluginManager::GetMaterialFactories() const
{
    return m_materialFactoryList;
}

/*!
 * Returns available random deviates plugins factory list.
 */
QVector< RandomDeviateFactory* > PluginManager::GetRandomDeviateFactories() const
{
    return m_randomDeviateFactoryList;
}


/*!
 * Returns available shape plugins factory list.
 */
QVector< TShapeFactory* > PluginManager::GetShapeFactories() const
{
    return m_shapeFactoryList;
}

/*!
 * Returns available sunshape plugins factory list.
 */
QVector< TSunShapeFactory* > PluginManager::GetSunShapeFactories() const
{
    return m_sunshapeFactoryList;
}

/*!
 * Returns available tracker plugins factory list.
 */
QVector< TTrackerFactory* > PluginManager::GetTrackerFactories() const
{
    return m_trackerFactoryList;
}

/*!
 * Returns available tracker plugins factory list.
 */
QVector< TTransmissivityFactory* > PluginManager::GetTransmissivityFactories() const
{
    return m_transmissivityFactoryList;
}

/*!
 * Loads all the valid plugins from "plugins" subdirecotry of the directory in
 * which the running version of Tonatiuh is located.
 */
void PluginManager::LoadAvailablePlugins(QDir pluginsDirectory)
{
    QStringList filesList;
    BuildFileList(pluginsDirectory, filesList);
    for (QString fileName: filesList)
        LoadTonatiuhPlugin(fileName);
}

/*!
 * Appends to \a fileList directory files.
 */
void PluginManager::AddFilesToList(QDir directory, QStringList& filesList)
{
    QString directoryPath(directory.absolutePath().append(QLatin1String("/") ) );

    QStringList filenamesList = directory.entryList(QDir::Files, QDir::Unsorted);
    for (int i = 0; i < filenamesList.size(); ++i)
        filesList << (directoryPath + filenamesList[i]);
}


/*!
 * Creates a list with the files al files in the defined \a directory
 * and its subdirectories.
 */
void PluginManager::BuildFileList(QDir directory, QStringList& filesList)
{
    AddFilesToList(directory, filesList);

    QString directoryPath(directory.absolutePath().append(QLatin1String("/") ) );
    QStringList subdirectoriesList = directory.entryList(QDir::Dirs, QDir::Unsorted);

    for (int i = 0; i< subdirectoriesList.size(); ++i)
    {
        QString subdirectoryName = subdirectoriesList[i];
        if (ValidDirectoryName(subdirectoryName) )
            BuildFileList(QDir(directoryPath + subdirectoryName), filesList);
    }

}

/*!
 * Loads the \a plugin as component type.
 */
void PluginManager::LoadComponentPlugin(QObject* plugin)
{
    TComponentFactory* pTComponentFactory = qobject_cast<TComponentFactory* >(plugin);
    if (!pTComponentFactory) gf::SevereError("PluginManager::LoadComponentPlugin: Component plug-in not recognized");
    m_componentFactoryList.push_back(pTComponentFactory);
}

/*!
 * Loads the \a plugin as component type.
 */
void PluginManager::LoadExportPhotonMapModePlugin(QObject* plugin)
{
    PhotonMapExportFactory* pPhotonMapExportFactory = qobject_cast<PhotonMapExportFactory* >(plugin);
    if (!pPhotonMapExportFactory) gf::SevereError("PluginManager::LoadExportPhotonMapModePlugin: Component plug-in not recognized");
    m_exportPMModeFactoryList.push_back(pPhotonMapExportFactory);
}

/*!
 * Loads the \a plugin as material type.
 */
void PluginManager::LoadMaterialPlugin(QObject* plugin)
{
    TMaterialFactory* pTMaterialFactory = qobject_cast<TMaterialFactory*>(plugin);
    if (!pTMaterialFactory) gf::SevereError("MainWindow::LoadPlugins: Material plug-in not recognized");
    pTMaterialFactory->CreateTMaterial();
    m_materialFactoryList.push_back(pTMaterialFactory);
}

/*!
 * Loads the \a plugin as RandomDeviatePlugin type.
 */
void PluginManager::LoadRandomDeviatePlugin(QObject* plugin)
{
    RandomDeviateFactory* pRamdomDeviateFactory = qobject_cast<RandomDeviateFactory* >(plugin);
    m_randomDeviateFactoryList.push_back(pRamdomDeviateFactory);

}

void PluginManager::LoadShapePlugin(QObject* plugin)
{
    TShapeFactory* pTShapeFactory = qobject_cast<TShapeFactory* >(plugin);
    if (!pTShapeFactory) gf::SevereError("MainWindow::LoadPlugins: Shape plug-in not recognized");
    pTShapeFactory->CreateTShape();
    m_shapeFactoryList.push_back(pTShapeFactory);
}

void PluginManager::LoadSunshapePlugin(QObject* plugin)
{
    TSunShapeFactory* pTSunShapeFactory = qobject_cast<TSunShapeFactory* >(plugin);
    if (!pTSunShapeFactory) gf::SevereError("MainWindow::LoadPlugins: SunShape plug-in not recognized"); ;
    pTSunShapeFactory->CreateTSunShape();
    m_sunshapeFactoryList.push_back(pTSunShapeFactory);
}

void PluginManager::LoadTrackerPlugin(QObject* plugin)
{
    TTrackerFactory* pTTrackerFactory = qobject_cast< TTrackerFactory* >(plugin);
    if (!pTTrackerFactory) gf::SevereError("MainWindow::LoadPlugins: Tracker plug-in not recognized");
    pTTrackerFactory->CreateTTracker();
    m_trackerFactoryList.push_back(pTTrackerFactory);
}

/*!
 * Loads \a plugin as transmissivity plugin.
 */
void PluginManager::LoadTransmissivityPlugin(QObject* plugin)
{
    TTransmissivityFactory* pTTransmissivityFactory = qobject_cast< TTransmissivityFactory* >(plugin);
    if (!pTTransmissivityFactory) gf::SevereError("MainWindow::LoadPlugins: Transmissivity plug-in not recognized");
    pTTransmissivityFactory->CreateTTransmissivity();
    m_transmissivityFactoryList.push_back(pTTransmissivityFactory);
}


/*!
 * Loads the plugin is defined in \a fileName, if it is a valid plugin.
 */
void PluginManager::LoadTonatiuhPlugin(const QString& fileName)
{
    QPluginLoader loader(fileName);
    QObject* plugin = loader.instance();
    if (plugin) {
        if (dynamic_cast<PhotonMapExportFactory*>(plugin)) LoadExportPhotonMapModePlugin(plugin);
        if (dynamic_cast<RandomDeviateFactory*>(plugin)) LoadRandomDeviatePlugin(plugin);
        if (plugin->inherits("TComponentFactory") ) LoadComponentPlugin(plugin);
        if (dynamic_cast<TShapeFactory*>(plugin)) LoadShapePlugin(plugin);
        if (dynamic_cast<TSunShapeFactory*>(plugin)) LoadSunshapePlugin(plugin);
        if (dynamic_cast<TMaterialFactory*>(plugin)) LoadMaterialPlugin(plugin);
        if (plugin->inherits("TTrackerFactory") ) LoadTrackerPlugin(plugin);
        if (plugin->inherits("TTransmissivityFactory") ) LoadTransmissivityPlugin(plugin);
    }
}

/*!
 * Checks if the \a directoryName is a valid directory name.
 * '.' and '..' are not valid names.
 */
bool PluginManager::ValidDirectoryName(QString& directoryName)
{
    return directoryName != "."  && directoryName != "..";
}
