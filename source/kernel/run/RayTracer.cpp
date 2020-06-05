#include <QPoint>

#include "shape/DifferentialGeometry.h"
#include "random/RandomParallel.h"
#include "libraries/geometry/Ray.h"
#include "RayTracer.h"
#include "kernel/photons/Photons.h"
#include "sun/SunAperture.h"
#include "sun/SunShape.h"
#include "air/Air.h"


RayTracer::RayTracer(InstanceNode* instanceRoot,
    InstanceNode* instanceSun,
    SunAperture* sunAperture,
    SunShape* const sunShape,
    Air* air,
    Random& rand,
    QMutex* mutexRand,
    Photons* photons,
    QMutex* mutexPhotons,
    QVector<InstanceNode*> exportSuraceList
):
    m_instanceLayout(instanceRoot),
    m_instanceSun(instanceSun),
    m_sunAperture(sunAperture),
    m_sunShape(sunShape),
    m_sunTransform(instanceSun->getTransform()),
    m_air(air),
    m_rand(&rand),
    m_mutexRand(mutexRand),
    m_photons(photons),
    m_mutexPhotons(mutexPhotons),
    m_exportSuraceList(exportSuraceList),
    m_sunCells(sunAperture->getCells())
{   

}

void RayTracer::operator()(ulong nRays)
{
    if (m_sunCells.empty()) return;
    bool bExportAll = m_exportSuraceList.empty();
    bool bExportLight = bExportAll ? true : m_exportSuraceList.contains(m_instanceSun);

    std::vector<Photon> photons;
    // Photon(Point3D pos, int side, double id = 0, InstanceNode* intersectedSurface = 0, int absorbedPhoton = 0);
    RandomParallel rand(m_rand, m_mutexRand);

    for (ulong n = 0; n < nRays; ++n)
    {
        // Part 1: first photon point (on sun surface)
        Ray ray;
        NewPrimitiveRay(&ray, rand);
        bool isFront = true;
        int rayLength = 0;
        InstanceNode* intersectedSurface = m_instanceSun;
        if (bExportLight)
            photons.push_back(Photon(ray.origin, isFront, rayLength, m_instanceSun) );

        // Part 2: middle photon points (intersection with shapes)
        bool isReflected = true;
        while (isReflected) {
            Ray rayReflected; // scattered?
            isFront = false;
            intersectedSurface = 0;
            isReflected = m_instanceLayout->intersect(ray, rand, isFront, intersectedSurface, rayReflected);

            // check absorption after the first reflection
            if (m_air && rayLength > 0) {
                if (m_air->transmission(ray.tMax) < rand.RandomDouble()) {
                    ++rayLength;
                    intersectedSurface = 0;
                    ray.tMax = gcf::infinity;
                    break;
                }
            }

            // save intersection
            if (!isReflected) break;
            ++rayLength;
            if (bExportAll || m_exportSuraceList.contains(intersectedSurface))
                photons.push_back(Photon(ray.point(ray.tMax), isFront, rayLength, intersectedSurface, 1) );
            ray = rayReflected;
        }

        // Part 3: last photon point (absorption in air)
        // skip rays without intersections
        if (rayLength == 0 && ray.tMax == gcf::infinity) continue;
        if (!bExportAll && !m_exportSuraceList.contains(intersectedSurface)) continue;
        // limit length of other rays
        if (ray.tMax == gcf::infinity) {// always true?
            ray.tMax = 1.;
            isFront = 0; // ? back for air
        }
        photons.push_back(Photon(ray.point(ray.tMax), isFront, ++rayLength, intersectedSurface) );
    }

    m_mutexPhotons->lock();
    m_photons->addPhotons(photons);
    m_mutexPhotons->unlock();
}

bool RayTracer::NewPrimitiveRay(Ray* ray, RandomParallel& rand)
{
    int index = int(rand.RandomDouble()*m_sunCells.size());
    QPair<int, int> cell = m_sunCells[index];

    Vector3D origin = m_sunAperture->Sample(rand.RandomDouble(), rand.RandomDouble(), cell.first, cell.second);
    Vector3D direction = m_sunShape->generateRay(rand);
    *ray = m_sunTransform(Ray(origin, direction));
    return true;
}
