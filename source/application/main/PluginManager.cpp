#include "PluginManager.h"

#include <QDir>
#include <QPluginLoader>
#include <QStringList>

#include "kernel/air/AirExponential.h"
#include "kernel/air/AirPolynomial.h"
#include "kernel/air/AirVacuum.h"
#include "kernel/component/ComponentFactory.h"
#include "kernel/material/MaterialAbsorber.h"
#include "kernel/material/MaterialVirtual.h"
#include "kernel/photons/PhotonsAbstract.h"
#include "kernel/photons/PhotonsWidget.h"
#include "kernel/profiles/ProfileBox.h"
#include "kernel/profiles/ProfileCircular.h"
#include "kernel/profiles/ProfileRegular.h"
#include "kernel/profiles/ProfileTriangle.h"
#include "kernel/profiles/ProfilePolygon.h"
#include "kernel/profiles/ProfileRectangular.h"
#include "kernel/random/RandomSTL.h"
#include "kernel/scene/TSceneKit.h"
#include "kernel/scene/TSeparatorKit.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/ShapeCube.h"
#include "kernel/shape/ShapePlanar.h"
#include "kernel/sun/SunAperture.h"
#include "kernel/sun/SunKit.h"
#include "kernel/sun/SunPillbox.h"
#include "kernel/trackers/Tracker.h"
#include "libraries/Coin3D/UserMField.h"
#include "libraries/Coin3D/UserSField.h"
#include "libraries/Coin3D/MFVec2.h"
#include "libraries/math/gcf.h"
#include "view/SkyBackground.h"


PluginManager::PluginManager()
{
    UserMField::initClass();
    UserSField::initClass();
    MFVec2::initClass();

    Tracker::initClass();

    TSceneKit::initClass();
    TSeparatorKit::initClass();

    SunShape::initClass();
    SunAperture::initClass();
    SunKit::initClass();
    Air::initClass();

    TShapeKit::initClass();
    ShapeRT::initClass();
    ProfileRT::initClass();
    MaterialRT::initClass();

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
        loadPlugin(f);
    }

    loadPlugin(new SunFactoryT<SunPillbox>);
    loadPlugin(new AirFactoryT<AirVacuum>);
    loadPlugin(new AirFactoryT<AirExponential>);
    loadPlugin(new AirFactoryT<AirPolynomial>);
    loadPlugin(new ShapeFactoryT<ShapePlanar>);
    loadPlugin(new ShapeFactoryT<ShapeCube>);

    loadPlugin(new ProfileFactoryT<ProfileBox>);
    loadPlugin(new ProfileFactoryT<ProfileRectangular>);
    loadPlugin(new ProfileFactoryT<ProfileCircular>);
    loadPlugin(new ProfileFactoryT<ProfileRegular>);
    loadPlugin(new ProfileFactoryT<ProfileTriangle>);
    loadPlugin(new ProfileFactoryT<ProfilePolygon>);

    loadPlugin(new MaterialFactoryT<MaterialAbsorber>);
    loadPlugin(new MaterialFactoryT<MaterialVirtual>);
    loadPlugin(new RandomFactoryT<RandomSTL>);
    loadPlugin(new PhotonsFactoryT<PhotonsAbstract, PhotonsWidget>);

    sort();
}

/*!
 * Creates a list with the files al files in the defined \a directory
 * and its subdirectories.
 */
void PluginManager::findFiles(QDir dir, QStringList& files)
{
    QString path = dir.absolutePath() + "/";

    for (QString f : dir.entryList(QDir::Files))
        files << path + f;

    for (QString d : dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
        findFiles(path + d, files);
}

/*!
 * Loads the plugin is defined in \a fileName, if it is a valid plugin.
 */
void PluginManager::loadPlugin(TFactory* p)
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
        m_materialMap[f->name()] = f;
    }
    else if (auto f = dynamic_cast<ProfileFactory*>(p))
    {
        f->init();
        m_profileFactories << f;
        m_profileMap[f->name()] = f;
    }
    else if (auto f = dynamic_cast<RandomFactory*>(p))
    {
        m_randomFactories << f;
        m_randomMap[f->name()] = f;
    }
    else if (auto f = dynamic_cast<ShapeFactory*>(p))
    {
        f->init();
        m_shapeFactories << f;
        m_shapeMap[f->name()] = f;
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
        m_trackerMap[f->name()] = f;
    }
}

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

void PluginManager::sort()
{
    QStringList sunNames = {
        "Pillbox",
        "Buie"
    };
    sortFactories(sunNames, m_sunFactories);

    QStringList shapeNames = {
        "Planar",
        "Parabolic",
        "Hyperbolic",
        "Elliptic",
        "",
        "Cube",
        "Sphere",
        "Cylinder",
        "Cone",
        "",
        "PlanarN",
        "Mesh"
    };
    sortFactories(shapeNames, m_shapeFactories);

    QStringList materialNames = {
        "Absorber",
        "Virtual",
        "Specular"
    };
    sortFactories(materialNames, m_materialFactories);

    QStringList trackerNames = {
        "Single",
        "Dual"
    };
    sortFactories(trackerNames, m_trackerFactories);

    QStringList exportNames = {
        "No export",
        "File"
    };
    sortFactories(exportNames, m_exportFactories);
}
