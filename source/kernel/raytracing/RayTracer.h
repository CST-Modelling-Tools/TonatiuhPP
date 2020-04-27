#pragma once

#include "kernel/TonatiuhKernel.h"
#include <vector>

#include <QVector>
#include <QMap>
#include <QPair>
#include <QObject>

#include "libraries/geometry/Transform.h"

class InstanceNode;
class ParallelRandomDeviate;
struct Photon;
class RandomDeviate;
struct RayTracerPhoton;
class QMutex;
class QPoint;
class PhotonMap;
class TLightShape;
class SunShape;
class TTransmissivity;


class TONATIUH_KERNEL RayTracer
{

public:
    RayTracer(InstanceNode* rootNode,
              InstanceNode* sunNode,
              TLightShape* lightShape,
              SunShape* const lightSunShape,
              Transform lightToWorld,
              TTransmissivity* transmissivity,
              RandomDeviate& rand,
              QMutex* mutex,
              PhotonMap* photonMap,
              QMutex* mutexPhotonMap,
              QVector<InstanceNode*> exportSuraceList);

    typedef void result_type;

    void operator()(ulong nRays);

private:
    bool NewPrimitiveRay(Ray* ray, ParallelRandomDeviate& rand);

    InstanceNode* m_rootNode;
    InstanceNode* m_sunNode;
    TLightShape* m_lightShape;
    const SunShape* m_lightSunShape;
    Transform m_lightToWorld;
    TTransmissivity* m_transmissivity;
    RandomDeviate* m_pRand;
    QMutex* m_mutex;
    PhotonMap* m_photonMap;
    QMutex* m_pPhotonMapMutex;
    QVector<InstanceNode*> m_exportSuraceList;

    const std::vector< QPair<int, int> >&  m_validAreasVector;
};
