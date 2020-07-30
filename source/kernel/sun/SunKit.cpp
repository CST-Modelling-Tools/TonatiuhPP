#include "SunKit.h"

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoLabel.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodekits/SoNodekitCatalog.h>

#include "SunAperture.h"
#include "SunPillbox.h"
#include "kernel/TonatiuhFunctions.h"
#include "scene/TSceneKit.h"
#include "kernel/run/InstanceNode.h"
#include "libraries/math/3D/Box3D.h"
#include "libraries/math/3D/Matrix4x4.h"
#include "libraries/math/3D/Transform.h"
#include "libraries/math/gcf.h"
#include "scene/TShapeKit.h"

SO_KIT_SOURCE(SunKit)

void SunKit::initClass()
{
    SO_KIT_INIT_CLASS(SunKit, SoLightKit, "LightKit");

    SunShape::initClass();
    SunAperture::initClass();
}

SunKit::SunKit()
{
    SO_KIT_CONSTRUCTOR(SunKit);

    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(iconMaterial, SoNode, SoMaterial, TRUE, iconSeparator, icon, TRUE);
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(iconTexture, SoNode, SoTexture2, TRUE, iconSeparator, iconMaterial, TRUE);
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(tsunshape, SunShape, SunPillbox, TRUE, transformGroup, "", TRUE);

    SO_NODE_ADD_FIELD( azimuth, (0.) );
    SO_NODE_ADD_FIELD( elevation, (90.*gcf::degree) );
    SO_NODE_ADD_FIELD( irradiance, (1000.) );

    SO_KIT_INIT_INSTANCE();

    SoDirectionalLight* light = static_cast<SoDirectionalLight*>(getPart("light", true) );
    light->direction.setValue(SbVec3f(0, 0, 1));

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

    SunAperture* aperture = new SunAperture;
    setPart("icon", aperture);

    setName("Sun");
    setPart("tsunshape", new SunPillbox);
}

SunKit::~SunKit()
{

}

/*!
 * Changes the light position to \a azimuth and \a zenith from the scene centre.
 * Azimuth and Zenith are in radians.
 * \sa redo().
 */
void SunKit::updateTransform()
{
    SoTransform* transform = (SoTransform*) getPart("transform", true);
    SbRotation elRotation(SbVec3f(1., 0., 0.), gcf::pi/2. + elevation.getValue());
    SbRotation azRotation(SbVec3f(0., 0., -1.), azimuth.getValue());
    transform->rotation = elRotation*azRotation;
}

void SunKit::setBox(Box3D box)
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
    SunShape* sunshape = static_cast<SunShape*>(getPart("tsunshape", false));
    if (!sunshape) return;
    double thetaMax = sunshape->getThetaMax();
    if (thetaMax > 0.)
        delta = distMax*tan(thetaMax);

    SunAperture* shape = static_cast<SunAperture*>(getPart("icon", false));
    if (!shape) return;
    shape->setSize(vA.x, vB.x, vA.y, vB.y, delta);

    if (!transform) return;
    SbVec3f res;
    mr.multVecMatrix(SbVec3f(0., 0., back), res);
    transform->translation = res;
}

void SunKit::setBox(TSceneKit* scene)
{
    SoGroup* separatorKit = static_cast<SoGroup*>(scene->getPart("group", false) );
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

bool SunKit::findTexture(int sizeX, int sizeY, InstanceNode* instanceRoot)
{
    SunAperture* aperture = static_cast<SunAperture*>(getPart("icon", false));
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

    aperture->findTexture(sizeX, sizeY, surfacesList, this);
    return true;
}
