#include "PluginManager.h"

#include <QDir>
#include <QPluginLoader>
#include <QStringList>

#include "kernel/air/AirExponential.h"
#include "kernel/air/AirFactory.h"
#include "kernel/air/AirPolynomial.h"
#include "kernel/air/AirVacuum.h"
#include "kernel/component/ComponentFactory.h"
#include "kernel/material/MaterialFactory.h"
#include "kernel/material/MaterialVirtual.h"
#include "kernel/photons/PhotonsDefault.h"
#include "kernel/photons/PhotonsFactory.h"
#include "kernel/photons/PhotonsWidget.h"
#include "kernel/random/RandomFactory.h"
#include "kernel/random/RandomSTL.h"
#include "kernel/scene/TSceneKit.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/ShapeCube.h"
#include "kernel/shape/ShapeFactory.h"
#include "kernel/sun/SunFactory.h"
#include "kernel/sun/SunPillbox.h"
#include "kernel/sun/TLightKit.h"
#include "kernel/sun/TLightShape.h"
#include "kernel/trackers/TrackerAbstract.h"
#include "kernel/trackers/TrackerFactory.h"
#include "libraries/fields/UserMField.h"
#include "libraries/fields/UserSField.h"
#include "libraries/geometry/gf.h"
#include "view/SkyBackground.h"


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
    }
}


PluginManager::PluginManager()
{
    UserMField::initClass();
    UserSField::initClass();

    TrackerAbstract::initClass();

    TSceneKit::initClass();
    TSeparatorKit::initClass();

    TLightShape::initClass();
    TLightKit::initClass();
    AirAbstract::initClass();
    SunAbstract::initClass();

    TShapeKit::initClass();
    ShapeAbstract::initClass();
    MaterialAbstract::initClass();

    SkyBackground::initClass();
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

    loadTonatiuhPlugin(new SunFactoryT<SunPillbox>);

    loadTonatiuhPlugin(new AirFactoryT<AirVacuum>);
    loadTonatiuhPlugin(new AirFactoryT<AirExponential>);
    loadTonatiuhPlugin(new AirFactoryT<AirPolynomial>);

    loadTonatiuhPlugin(new MaterialFactoryT<MaterialVirtual>);

//    loadTonatiuhPlugin(new ShapeFactoryT<ShapeCube>);

    loadTonatiuhPlugin(new RandomFactoryT<RandomSTL>);
    loadTonatiuhPlugin(new PhotonsFactoryT<PhotonsDefault, PhotonsWidget>);

    sort();
}

void PluginManager::sort()
{
    QStringList shapeNames = {
        "Plane",
        "Parabolic",
        "Sphere",
        "Cylinder"
    };
    sortFactories(shapeNames, m_shapeFactories);

    QStringList exportNames = {
        "No export",
        "File"
    };
    sortFactories(exportNames, m_exportFactories);

    QStringList trackerNames = {
        "Trough",
        "Heliostat"
    };
    sortFactories(trackerNames, m_trackerFactories);

    QStringList sunNames = {
        "Pillbox",
        "Buie"
    };
    sortFactories(sunNames, m_sunFactories);
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
    if (auto f = dynamic_cast<AirFactory*>(p))
    {
        f->init();
        m_airFactories << f;
        m_airMap[f->name()] = f;
    }
    else if (auto f = dynamic_cast<ComponentFactory*>(p))
    {
        m_componentFactories << f;
    }
    else if (auto f = dynamic_cast<PhotonsFactory*>(p))
    {
        m_exportFactories << f;
    }
    else if (auto f = dynamic_cast<MaterialFactory*>(p))
    {
        f->init();
        m_materialFactories << f;
        m_materialsMap[f->name()] = f;
    }
    else if (auto f = dynamic_cast<RandomFactory*>(p))
    {
        m_randomFactories << f;
    }
    else if (auto f = dynamic_cast<ShapeFactory*>(p))
    {
        f->init();
        m_shapeFactories << f;
        m_shapesMap[f->name()] = f;
    }
    else if (auto f = dynamic_cast<SunFactory*>(p))
    {
        f->init();
        m_sunFactories << f;
        m_sunMap[f->name()] = f;
    }
    else if (auto f = dynamic_cast<TrackerFactory*>(p))
    {
        f->init();
        m_trackerFactories << f;
        m_trackersMap[f->name()] = f;
    }
}
