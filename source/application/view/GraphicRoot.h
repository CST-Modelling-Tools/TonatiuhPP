#pragma once

#include <QObject>
#include "GridNode3D.h"

class SoSeparator;
class SoSelection;
class SoPath;
class Document;
class SkyNode3D;
class SunNode3D;
class SoTransform;
class TSceneKit;
class SoSensor;
class SoFieldSensor;

class GraphicRoot: public QObject
{
    Q_OBJECT

public:
    GraphicRoot();
    ~GraphicRoot();

    SoSeparator* getRoot() const {return m_root;}
    void setDocument(Document* document);
    void updateScene(TSceneKit* scene);
    void removeScene();

    GridNode3D* grid() {return m_grid;}
    void showGrid(bool on);

    SoSeparator* rays() {return m_rays;}
    void showRays(bool on);
    void showPhotons(bool on);

    void select(const SoPath* path);
    void deselectAll();
    void onSelectionChanged(SoSelection* selection);

signals:
    void selectionChanged(SoSelection* selection);

private:
    SoSeparator* m_root;
    SkyNode3D* m_sky;
    SunNode3D* m_sun;
    GridNode3D* m_grid;
    SoSelection* m_selection;
    SoSeparator* m_rays;

    TSceneKit* m_scene;
    SoFieldSensor* m_sensor;
    static void update(void* data, SoSensor*);
};
