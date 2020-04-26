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
#include "kernel/raytracing/TSunFactory.h"
#include "kernel/raytracing/TTrackerFactory.h"
#include "kernel/raytracing/TSquare.h"
#include "kernel/raytracing/TCube.h"


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
    QStringList files;
    findFiles(dir, files);

    for (QString file: files) {
        QPluginLoader loader(file);
        QObject* p = loader.instance();
        TFactory* f = dynamic_cast<TFactory*>(p);
        loadTonatiuhPlugin(f);
    }

    loadTonatiuhPlugin(new ShapeFactory<TSquare>);
    loadTonatiuhPlugin(new ShapeFactory<TCube>);

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
void PluginManager::loadTonatiuhPlugin(TFactory* p)
{
    if (auto f = dynamic_cast<TTransmissivityFactory*>(p))
    {
        f->init();
        m_airFactories << f;
        m_airMap[f->name()] = f;
    }
    else if (auto f = dynamic_cast<TComponentFactory*>(p))
    {
        m_componentFactories << f;
    }
    else if (auto f = dynamic_cast<PhotonMapExportFactory*>(p))
    {
        m_exportFactories << f;
    }
    else if (auto f = dynamic_cast<TMaterialFactory*>(p))
    {
        f->init();
        m_materialFactories << f;
        m_materialsMap[f->name()] = f;
    }
    else if (auto f = dynamic_cast<RandomDeviateFactory*>(p))
    {
        m_randomFactories << f;
    }
    else if (auto f = dynamic_cast<TShapeFactory*>(p))
    {
        f->init();
        m_shapeFactories << f;
        m_shapesMap[f->name()] = f;
    }
    else if (auto f = dynamic_cast<TSunFactory*>(p))
    {
        f->init();
        m_sunFactories << f;
    }
    else if (auto f = dynamic_cast<TTrackerFactory*>(p))
    {
        f->create();
        m_trackerFactories << f;
    }
}
