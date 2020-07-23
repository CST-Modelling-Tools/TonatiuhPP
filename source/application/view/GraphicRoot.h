#pragma once

#include <QObject>

class SoSeparator;
class SoSelection;
class SoPath;
class TSceneKit;


class GraphicRoot: public QObject
{
    Q_OBJECT

public:
    GraphicRoot();
    ~GraphicRoot();

    SoSeparator* getRoot() const {return m_root;}
    void addScene(TSceneKit* sceneKit);
    void removeScene();

    SoSeparator* grid() {return m_grid;}
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
    SoSeparator* m_grid;
    SoSeparator* m_rays;
    SoSelection* m_selection;
};
