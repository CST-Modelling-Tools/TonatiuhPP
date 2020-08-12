#include "SunKitW.h"

#include "SunPosition.h"
#include "SunPillbox.h"
#include "SunAperture.h"

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoLabel.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodekits/SoNodekitCatalog.h>

#include "kernel/TonatiuhFunctions.h"
#include "scene/TSceneKit.h"
#include "scene/TSeparatorKit.h"
#include "kernel/run/InstanceNode.h"
#include "libraries/math/3D/Box3D.h"
#include "libraries/math/3D/Matrix4x4.h"
#include "libraries/math/3D/Transform.h"
#include "libraries/math/gcf.h"
#include "scene/TShapeKit.h"

SO_KIT_SOURCE(SunKitW)


void SunKitW::initClass()
{
    SO_KIT_INIT_CLASS(SunKitW, SoBaseKit, "BaseKit");

    SunPosition::initClass();
}

SunKitW::SunKitW()
{
    SO_KIT_CONSTRUCTOR(SunKitW);

    SO_KIT_ADD_CATALOG_ENTRY(iconMaterial, SoMaterial, FALSE, this, "", TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(iconTexture, SoTexture2, FALSE, this, "", TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(transform, SoTransform, FALSE, this, "", TRUE);

    SO_KIT_ADD_CATALOG_ENTRY(position, SunPosition, FALSE, this, "", TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(shape, SunPillbox, FALSE, this, "", TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(aperture, SunAperture, FALSE, this, "", TRUE);

    SO_KIT_INIT_INSTANCE();

//    SoDirectionalLight* light = static_cast<SoDirectionalLight*>(getPart("light", true) );
//    light->direction.setValue(SbVec3f(0, 0, 1));

    // temp
    updateTransform();
    SoTransform* transform = (SoTransform*) getPart("transform", true);
    SbMatrix mr;
    mr.setRotate(transform->rotation.getValue());
    SbVec3f res;
    mr.multVecMatrix(SbVec3f(0., 0., -10.), res);
    transform->translation = res;

    SoMaterial* material = static_cast<SoMaterial*>(getPart("iconMaterial", true) );
    material->transparency = 0.75f;
    setPart("iconMaterial", material);

    int sizeX = 10;
    int sizeY = 10;
    QVector<uchar> bitmap(sizeX*sizeY);
    bitmap.fill(255);

    SoTexture2* texture = new SoTexture2;
    texture->image.setValue(SbVec2s(sizeX, sizeY), 1, bitmap.data());
    texture->model = SoTexture2::BLEND;
    texture->blendColor.setValue(1.f, 1.f, 1.f);
    setPart("iconTexture", texture);

//    SunAperture* aperture = new SunAperture;
//    setPart("aperture", aperture);
//    setPart("shape", new SunPillbox);
    setName("SunW");
}

SunKitW::~SunKitW()
{
    qDebug() << QString("destroyrd") + getName().getString();
}

void SunKitW::updateTransform()
{
    SoTransform* transform = (SoTransform*) getPart("transform", true);
    SunPosition* sp = (SunPosition*) getPart("position", true);
    transform->rotation =
        SbRotation(SbVec3f(1., 0., 0.), (90. + sp->elevation.getValue())*gcf::degree)*
        SbRotation(SbVec3f(0., 0., 1.), -sp->azimuth.getValue()*gcf::degree);
}

void SunKitW::setBox(Box3D box)
{
    updateTransform();
    SoTransform* transform = (SoTransform*) getPart("transform", true);
    SbMatrix mr;
    mr.setRotate(transform->rotation.getValue());

    Transform tr = tgf::makeTransform(mr).inversed();
    box = tr(box);

    //box is global
    vec3d vA = box.min();
    vec3d vB = box.max();
    double distMax = vB.z + 10. - vA.z;
    double back = vA.z - 10.;

    if (-gcf::infinity == box.volume() )
    {
        vA.x = 0.;
        vB.x = 0.;
        vA.y = 0.;
        vB.y = 0.;
        distMax = 0.;
    }

    double delta = 0.01;
    SunShape* sunshape = static_cast<SunShape*>(getPart("shape", false));
    if (!sunshape) return;
    double thetaMax = sunshape->getThetaMax();
    if (thetaMax > 0.)
        delta = distMax*tan(thetaMax);

    SunAperture* shape = static_cast<SunAperture*>(getPart("aperture", false));
    if (!shape) return;
    shape->setSize(vA.x, vB.x, vA.y, vB.y, delta);

    if (!transform) return;
    SbVec3f res;
    mr.multVecMatrix(SbVec3f(0., 0., back), res);
    transform->translation = res;
}

void SunKitW::setBox(TSceneKit* scene)
{
    TSeparatorKit* separatorKit = scene->getLayout();
    if (!separatorKit) return;

    SoGetBoundingBoxAction* action = new SoGetBoundingBoxAction(SbViewportRegion() );
    separatorKit->getBoundingBox(action);
    SbBox3f box = action->getBoundingBox();
    delete action;

    if (!box.isEmpty() )
    {
        Box3D sceneBox(
            vec3d(box.getMin()[0], box.getMin()[1], box.getMin()[2]),
            vec3d(box.getMax()[0], box.getMax()[1], box.getMax()[2])
        );
        setBox(sceneBox);
    }
}

bool SunKitW::findTexture(int sizeX, int sizeY, InstanceNode* instanceRoot)
{
    SunAperture* aperture = static_cast<SunAperture*>(getPart("aperture", false));
    if (!aperture) return false;

    QStringList disabledList = QString(aperture->disabledNodes.getValue().getString()).split(";", QString::SkipEmptyParts);
    QVector< QPair<TShapeKit*, Transform> > surfacesList;
    instanceRoot->collectShapeTransforms(disabledList, surfacesList);
    if (surfacesList.isEmpty()) return false;

    SoTransform* sunTransform = static_cast<SoTransform*>(getPart("transform", false));
    SbMatrix mr;
    mr.setRotate(sunTransform->rotation.getValue());
    Transform tSun = tgf::makeTransform(mr).inversed();
    for (auto& s : surfacesList)
        s.second = tSun*s.second;

//    aperture->findTexture(sizeX, sizeY, surfacesList, this);
    aperture->findTexture(sizeX, sizeY, surfacesList, (SunKit*)0);
    return true;
}
