#include <QPoint>

#include "kernel/shape/DifferentialGeometry.h"
#include "kernel/random/ParallelRandomDeviate.h"
#include "libraries/geometry/Ray.h"
#include "RayTracer.h"
#include "kernel/photons/PhotonMap.h"
#include "TLightShape.h"
#include "kernel/sun/SunShape.h"
#include "kernel/air/TTransmissivity.h"


RayTracer::RayTracer(InstanceNode* rootNode,
                     InstanceNode* sunNode,
                     TLightShape* lightShape,
                     SunShape* const lightSunShape,
                     Transform lightToWorld,
                     TTransmissivity* transmissivity,
                     RandomDeviate& rand,
                     QMutex* mutex,
                     PhotonMap* photonMap,
                     QMutex* mutexPhotonMap,
                     QVector<InstanceNode*> exportSuraceList):
    m_rootNode(rootNode),
    m_sunNode(sunNode),
    m_lightShape(lightShape),
    m_lightSunShape(lightSunShape),
    m_lightToWorld(lightToWorld),
    m_transmissivity(transmissivity),
    m_pRand(&rand),
    m_mutex(mutex),
    m_photonMap(photonMap),
    m_pPhotonMapMutex(mutexPhotonMap),
    m_exportSuraceList(exportSuraceList),
    m_validAreasVector(lightShape->GetValidAreasCoord())
{   

}

void RayTracer::operator()(ulong nRays)
{
    if (m_validAreasVector.empty()) return;
    bool bExportAll = m_exportSuraceList.empty();
    bool bExportLight = bExportAll ? true : m_exportSuraceList.contains(m_sunNode);

    std::vector<Photon> photons;
    // Photon(Point3D pos, int side, double id = 0, InstanceNode* intersectedSurface = 0, int absorbedPhoton = 0);
    ParallelRandomDeviate rand(m_pRand, m_mutex);

    for (ulong n = 0; n < nRays; ++n)
    {
        // Part 1: first photon point (on sun surface)
        Ray ray;
        NewPrimitiveRay(&ray, rand);
        bool isFront = true;
        int rayLength = 0;
        InstanceNode* intersectedSurface = m_sunNode;
        if (bExportLight)
            photons.push_back(Photon(ray.origin, isFront, rayLength, m_sunNode) );

        // Part 2: middle photon points (intersection with shapes)
        bool isReflected = true;
        while (isReflected) {
            Ray rayReflected;
            isFront = false;
            intersectedSurface = 0;
            isReflected = m_rootNode->Intersect(ray, rand, &isFront, &intersectedSurface, &rayReflected);

            // check absorption after the first reflection
            if (m_transmissivity && rayLength > 0) {
                if (m_transmissivity->transmission(ray.tMax) < rand.RandomDouble()) {
                    ++rayLength;
                    intersectedSurface = 0;
                    ray.tMax = gc::Infinity;
                    break;
                }
            }

            // save intersection
            if (!isReflected) break;
            ++rayLength;
            if (bExportAll || m_exportSuraceList.contains(intersectedSurface))
                photons.push_back(Photon( (ray)(ray.tMax), isFront, rayLength, intersectedSurface, 1) );
            ray = rayReflected;
        }

        // Part 3: last photon point (absorption in air)
        // skip rays without intersections
        if (rayLength == 0 && ray.tMax == gc::Infinity) continue;
        if (!bExportAll && !m_exportSuraceList.contains(intersectedSurface)) continue;
        // limit length of other rays
        if (ray.tMax == gc::Infinity) {// always true?
            ray.tMax = 0.1;
            isFront = 0; // ? back for air
        }
        photons.push_back(Photon( (ray)(ray.tMax), isFront, ++rayLength, intersectedSurface) );
    }

    m_pPhotonMapMutex->lock();
    m_photonMap->StoreRays(photons);
    m_pPhotonMapMutex->unlock();
}

bool RayTracer::NewPrimitiveRay(Ray* ray, ParallelRandomDeviate& rand)
{
    int area = int(rand.RandomDouble()*m_validAreasVector.size());
    QPair<int, int> areaIndex = m_validAreasVector[area];

    Point3D origin = m_lightShape->Sample(rand.RandomDouble(), rand.RandomDouble(), areaIndex.first, areaIndex.second);

    Vector3D direction;
    m_lightSunShape->GenerateRayDirection(direction, rand);

    *ray =  m_lightToWorld(Ray(origin, direction) );
    return true;
}

