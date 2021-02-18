#pragma once

#include <QString>
#include "libraries/math/2D/Matrix2D.h"
#include "libraries/math/2D/Box2D.h"
#include "libraries/math/2D/vec2i.h"

class TSceneKit;
class SceneTreeModel;
class InstanceNode;
class Random;
class PhotonsBuffer;

class FluxAnalysis
{

public:
    FluxAnalysis(TSceneKit* sceneKit, SceneTreeModel* sceneModel, int sunWidthDivisions, int sunHeightDivisions, Random* randomDeviate);
    ~FluxAnalysis();

    QString getShapeType(QString nodeURL);
    void run(QString nodeURL, QString surfaceSide, ulong nRays, bool increasePhotonMap, int uDivs, int vDivs, bool silent = false);
    void setBins(int rows, int cols);
    void write(QString fileName, bool withCoords);
    void clear();

    Matrix2D<int>& getBinsPhotons() {return m_binsPhotons;}
    Matrix2D<double>& getBinsFlux() {return m_binsFlux;}
    const Box2D& box() const {return m_box;}
    int photonsMax() {return m_photonsMax;} // photons in bin with maximal photons
    const vec2i& getPhotonMaxPos() const {return m_photonsMaxPos;} //  bin with maximal photons
    int photonsError() {return m_photonsError;} //?
    double powerPhoton() {return m_powerPhoton;}
    double powerTotal() {return m_powerTotal;}

private:
    void fillBins();

    TSceneKit* m_sceneKit;
    SceneTreeModel* m_sceneModel;
    InstanceNode* m_instanceLayout;
    vec2i m_sunDivs;
    Random* m_rand;

    PhotonsBuffer* m_photons;

    QString m_surfaceURL;
    QString m_surfaceSide;

    ulong m_tracedRays;
    double m_powerTotal; // photons * power
    double m_powerPhoton;

    Matrix2D<int> m_binsPhotons;
    Matrix2D<double> m_binsFlux;

    Box2D m_box;

    int m_photonsMax; // maximal number of photons in a cell
    vec2i m_photonsMaxPos; // indices of cell with maximal number of photons
    int m_photonsError; // ?maximal number of photons in a cell for a reduced grid
};
