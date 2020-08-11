#pragma once

#include <QObject>

class SoSeparator;
class SoSelection;
class SoPath;
class Document;
class SkyBackground;
class SoTransform;


class GraphicRoot: public QObject
{
    Q_OBJECT

public:
    GraphicRoot();
    ~GraphicRoot();

    SoSeparator* getRoot() const {return m_root;}
    void setDocument(Document* document);
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
    SoTransform* m_transformSun;
    SkyBackground* m_sky;
    SoSeparator* m_grid;
    SoSelection* m_selection;
    SoSeparator* m_rays;
};
