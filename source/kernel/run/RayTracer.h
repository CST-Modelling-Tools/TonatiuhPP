#pragma once

#include "kernel/TonatiuhKernel.h"
#include <vector>

#include <QVector>
#include <QMap>
#include <QPair>
#include <QObject>

#include "libraries/geometry/Transform.h"

class InstanceNode;
class RandomParallel;
struct Photon;
class RandomAbstract;
struct RayTracerPhoton;
class QMutex;
class QPoint;
class Photons;
class SunAperture;
class SunAbstract;
class AirAbstract;


class TONATIUH_KERNEL RayTracer
{

public:
    RayTracer(InstanceNode* instanceRoot,
              InstanceNode* instanceSun,
              SunAperture* sunAperture,
              SunAbstract* const sunShape,
              AirAbstract* air,
              RandomAbstract& rand,
              QMutex* mutex,
              Photons* photonMap,
              QMutex* mutexPhotonMap,
              QVector<InstanceNode*> exportSuraceList);

    typedef void result_type;

    void operator()(ulong nRays);

private:
    bool NewPrimitiveRay(Ray* ray, RandomParallel& rand);

    InstanceNode* m_instanceLayout;
    InstanceNode* m_instanceSun;
    SunAperture* m_sunAperture;
    const SunAbstract* m_sunShape;
    Transform m_sunTransform;
    AirAbstract* m_air;
    RandomAbstract* m_rand;
    QMutex* m_mutex;
    Photons* m_photonMap;
    QMutex* m_pPhotonMapMutex;
    QVector<InstanceNode*> m_exportSuraceList;

    const std::vector< QPair<int, int> >&  m_sunCells;
};
