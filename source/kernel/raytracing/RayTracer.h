#pragma once

#include <vector>

#include <QMap>
#include <QPair>
#include <QObject>
#include <QVector>

#include "libraries/geometry/Transform.h"

class InstanceNode;
class ParallelRandomDeviate;
struct Photon;
class RandomDeviate;
struct RayTracerPhoton;
class QMutex;
class QPoint;
class TPhotonMap;
class TLightShape;
class TSunShape;
class TTransmissivity;


class RayTracer
{

public:
    RayTracer(InstanceNode* rootNode,
              InstanceNode* lightNode,
              TLightShape* lightShape,
              TSunShape* const lightSunShape,
              Transform lightToWorld,
              TTransmissivity* transmissivity,
              RandomDeviate& rand,
              QMutex* mutex,
              TPhotonMap* photonMap,
              QMutex* mutexPhotonMap,
              QVector< InstanceNode* > exportSuraceList);

    typedef void result_type;
    void operator()(double numberOfRays);


private:
    bool NewPrimitiveRay(Ray* ray, ParallelRandomDeviate& rand);
    void RayTracerCreatingAllPhotons(double numberOfRays);
    void RayTracerCreatingLightPhotons(double numberOfRays);
    void RayTracerNotCreatingLightPhotons(double numberOfRays);


    QVector< InstanceNode* > m_exportSuraceList;
    InstanceNode* m_rootNode;
    InstanceNode* m_lightNode;
    TLightShape* m_lightShape;
    const TSunShape* m_lightSunShape;
    Transform m_lightToWorld;
    RandomDeviate* m_pRand;
    QMutex* m_mutex;
    TPhotonMap* m_photonMap;
    QMutex* m_pPhotonMapMutex;
    TTransmissivity* m_transmissivity;
    std::vector< QPair<int, int > >  m_validAreasVector;
};
