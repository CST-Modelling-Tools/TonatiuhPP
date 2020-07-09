#pragma once

#include <QString>
#include "libraries/math/2D/Matrix2D.h"

class TSceneKit;
class SceneModel;
class InstanceNode;
class Random;
class PhotonsBuffer;

class FluxAnalysis
{

public:
    FluxAnalysis(TSceneKit* sceneKit, SceneModel* sceneModel, InstanceNode* instanceRoot,
                 int sunWidthDivisions, int sunHeightDivisions, Random* randomDeviate);
    ~FluxAnalysis();

    QString getShapeType(QString nodeURL);
    void run(QString nodeURL, QString surfaceSide, ulong nOfRays, bool increasePhotonMap, int rows, int cols);
    void setBins(int rows, int cols);
    void write(QString directory, QString file, bool withCoords);
    void clear();

    Matrix2D<int>& getBins() {return m_bins;}
    double uMin() {return m_uMin;}
    double uMax() {return m_uMax;}
    double vMin() {return m_vMin;}
    double vMax() {return m_vMax;}
    int photonsMax() {return m_photonsMax;} // photons in bin with maximal photons
    int photonsMaxRow() {return m_photonsMaxRow;} // row of bin with maximal photons
    int photonsMaxCol() {return m_photonsMaxCol;} // col of bin with maximal photons
    int photonsError() {return m_photonsError;} //?
    double powerPhoton() {return m_powerPhoton;}
    double powerTotal() {return m_powerTotal;}

private:
    void fillBins();

    TSceneKit* m_sceneKit;
    SceneModel* m_sceneModel;
    InstanceNode* m_instanceRoot;
    int m_sunWidthDivisions;
    int m_sunHeightDivisions;
    Random* m_rand;

    PhotonsBuffer* m_photons;

    QString m_surfaceURL;
    QString m_surfaceSide;

    ulong m_tracedRays;
    double m_powerTotal;
    double m_powerPhoton;

    Matrix2D<int> m_bins;

    double m_uMin;
    double m_uMax;
    double m_vMin;
    double m_vMax;

    int m_photonsMax;
    int m_photonsMaxRow;
    int m_photonsMaxCol;
    int m_photonsError;
};
