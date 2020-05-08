#pragma once

#include <QObject>

class GraphicRootTracker;
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

    void AddGrid(SoSeparator* grid);
    void AddRays(SoSeparator* rays);
    void AddModel(TSceneKit* sceneModel);

    void DeselectAll();

    SoSeparator* GetNode() const;

    void RemoveGrid();
    void RemoveRays();
    void RemoveModel();

    void Select(const SoPath* path);
    void SelectionChanged(SoSelection* selection);

    void ShowBackground(bool view);
    void ShowGrid(bool view);
    void ShowRays(bool view);

signals:
    void ChangeSelection(SoSelection* selection);

private:
    SoSeparator* CreateGrid(int xDimension, int zDimension, double xSpacing, double zSpacing);

    SoSeparator* m_nodeRoot;
    SoSeparator* m_pGrid;
    SoSeparator* m_pRays;
    SoTransform* m_pRootTransform;
    SoSeparator* m_pSceneSeparator;
    SoSelection* m_pSelectionNode;
    GraphicRootTracker* m_pTracker;
};
