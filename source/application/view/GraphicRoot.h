#pragma once

#include <QObject>

//class GraphicRootTracker;
class SoPath;
class SoSelection;
class SoSeparator;
class SoTransform;
class TSceneKit;

class GraphicRoot: public QObject
{
    Q_OBJECT

public:
    GraphicRoot();
    ~GraphicRoot();

    void AddModel(TSceneKit* sceneKit);
    void RemoveModel();

    void ShowBackground(bool view);

    void AddGrid(SoSeparator* grid);
    void ShowGrid(bool view);
    void RemoveGrid();

    SoSeparator* rays() {return m_rays;}
    void removeRays();
    void showRays(bool on);
    void showPhotons(bool on);

    void DeselectAll();
    SoSeparator* GetNode() const;
    void Select(const SoPath* path);
    void SelectionChanged(SoSelection* selection);

signals:
    void ChangeSelection(SoSelection* selection);

private:
    SoSeparator* CreateGrid(int xDimension, int zDimension, double xSpacing, double zSpacing);

    SoSeparator* m_nodeRoot;
    SoSeparator* m_pGrid;
    SoSeparator* m_rays;
    SoTransform* m_pRootTransform;
    SoSeparator* m_pSceneSeparator;
    SoSelection* m_pSelectionNode;
};
