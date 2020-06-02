#pragma once

#include <QString>

class TSceneKit;
class SceneModel;
class InstanceNode;
class Random;
class Photons;

class FluxAnalysis
{

public:
    FluxAnalysis(TSceneKit* currentScene, SceneModel& currentSceneModel, InstanceNode* rootSeparatorInstance,
                 int sunWidthDivisions, int sunHeightDivisions, Random* randomDeviate);
    ~FluxAnalysis();
    QString GetSurfaceType(QString nodeURL);
    void RunFluxAnalysis(QString nodeURL, QString surfaceSide, ulong nOfRays, bool increasePhotonMap, int heightDivisions, int widthDivisions);
    void UpdatePhotonCounts(int heightDivisions, int widthDivisions);
    void ExportAnalysis(QString directory, QString fileName, bool saveCoords);
    int** photonCountsValue();
    double xminValue();
    double yminValue();
    double xmaxValue();
    double ymaxValue();
    int maximumPhotonsValue();
    int maximumPhotonsXCoordValue();
    int maximumPhotonsYCoordValue();
    int maximumPhotonsErrorValue();
    double wPhotonValue();
    double totalPowerValue();
    void clearPhotonMap();

private:
    bool CheckSurface();
    bool CheckSurfaceSide();
    void UpdatePhotonCounts();
    void FluxAnalysisCylinder(InstanceNode* node);
    void FluxAnalysisFlatDisk(InstanceNode* node);
    void FluxAnalysisPlanar(InstanceNode* node);

    TSceneKit* m_sceneKit;
    SceneModel* m_sceneModel;
    InstanceNode* m_instanceRoot;
    int m_sunWidthDivisions;
    int m_sunHeightDivisions;
    Random* m_rand;

    Photons* m_photons;

    QString m_surfaceURL;
    QString m_surfaceSide;
    unsigned long m_tracedRays;
    double m_photonPower;

    int** m_photonCounts;
    int m_heightDivisions;
    int m_widthDivisions;
    double m_uMin;
    double m_uMax;
    double m_vMin;
    double m_vMax;
    int m_maximumPhotons;
    int m_maximumPhotonsXCoord;
    int m_maximumPhotonsYCoord;
    int m_maximumPhotonsError;
    double m_totalPower;
};
