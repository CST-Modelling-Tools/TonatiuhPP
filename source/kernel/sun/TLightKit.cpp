#include "TLightKit.h"

#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoLabel.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodekits/SoNodekitCatalog.h>

#include "SunAperture.h"
#include "kernel/TonatiuhFunctions.h"
#include "libraries/geometry/BoundingBox.h"
#include "libraries/geometry/Matrix4x4.h"
#include "libraries/geometry/Transform.h"
#include "libraries/geometry/gcf.h"
#include "scene/TShapeKit.h"
#include "sun/SunPillbox.h"


SO_KIT_SOURCE(TLightKit)

/**
 * Initializates TLightKit componets
 */
void TLightKit::initClass()
{
    SO_KIT_INIT_CLASS(TLightKit, SoLightKit, "LightKit");
}

/**
 * Creates a new TLightKit.
 */
TLightKit::TLightKit()
{
    SO_KIT_CONSTRUCTOR(TLightKit);

    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(iconMaterial, SoNode, SoMaterial, TRUE, iconSeparator, icon, TRUE);
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(iconTexture, SoNode, SoTexture2, TRUE, iconSeparator, iconMaterial, TRUE);
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(tsunshape, SunAbstract, SunPillbox, TRUE, transformGroup, "", TRUE);

    SO_NODE_ADD_FIELD( azimuth, (0.) );
    SO_NODE_ADD_FIELD( elevation, (90.*gcf::degree) );
    SO_NODE_ADD_FIELD( irradiance, (1000.) );
    SO_NODE_ADD_FIELD( disabledNodes, ("") );

    SO_KIT_INIT_INSTANCE();

    SoDirectionalLight* light = static_cast<SoDirectionalLight*>(getPart("light", true) );
    light->direction.setValue(SbVec3f(0, 0, 1) );

    SoTransform* transform = new SoTransform;
    setPart("transform", transform);

    SoMaterial* lightMaterial = static_cast<SoMaterial*>(getPart("iconMaterial", true) );
    lightMaterial->transparency = 0.75f;
    setPart("iconMaterial", lightMaterial);

    int widthPixeles = 10;
    int heightPixeles = 10;
    QVector<uchar> bitmap(widthPixeles*heightPixeles);
    bitmap.fill(255);

    SoTexture2* texture = new SoTexture2;
    texture->image.setValue(SbVec2s(heightPixeles, widthPixeles), 1, bitmap.data());
    texture->model = SoTexture2::BLEND;
    texture->blendColor.setValue(1.f, 1.f, 1.f);
    setPart("iconTexture", texture);

    SunAperture* iconShape = new SunAperture;
    setPart("icon", iconShape);

    setName("Light");
    updatePosition();
}

/**
 * Destructor.
 */
TLightKit::~TLightKit()
{
    //void ChangePosition( QDateTime time, double longitude, double latitude );
    //void SetDateTime( QDateTime time );
}

/*!
 * Changes the light position to \a azimuth and \a zenith from the scene centre.
 * Azimuth and Zenith are in radians.
 * \sa redo().
 */
void TLightKit::updatePosition()
{
    SoTransform* transform = (SoTransform*) getPart("transform", false);

    SbRotation elRotation(SbVec3f(1., 0., 0.), gcf::pi/2. + elevation.getValue());
    SbRotation azRotation(SbVec3f(0., 0., -1.), azimuth.getValue());

    transform->rotation = elRotation*azRotation;
}

void TLightKit::setBox(BoundingBox box)
{
    SoTransform* transform = (SoTransform*) getPart("transform", false);
    SbMatrix mr;
    mr.setRotate(transform->rotation.getValue());

    Transform tr = tgf::makeTransform(mr).inversed();
    box = tr(box);

    //box is global
    double xMin = box.pMin.x;
    double xMax = box.pMax.x;
    double yMin = box.pMin.y;
    double yMax = box.pMax.y;

    double distMax = box.pMax.z + 10. - box.pMin.z;
    double back = box.pMin.z - 10.;

    if (-gcf::infinity == box.volume() )
    {
        xMin = 0.;
        xMax = 0.;
        yMin = 0.;
        yMax = 0.;
        distMax = 0.;
    }

    double delta = 0.01;
    SunAbstract* sunshape = static_cast<SunAbstract*>(getPart("tsunshape", false));
    if (!sunshape) return;
    double thetaMax = sunshape->getThetaMax();
    if (thetaMax > 0.)
        delta = distMax*tan(thetaMax);

    SunAperture* shape = static_cast<SunAperture*>(getPart("icon", false));
    if (!shape) return;
    shape->setSize(xMin, xMax, yMin, yMax, delta);

    if (!transform) return;
    SbVec3f res;
    mr.multVecMatrix(SbVec3f(0., 0., back), res);
    transform->translation = res;
}

void TLightKit::findTexture(int xPixels, int yPixels, QVector< QPair<TShapeKit*, Transform> > surfacesList)
{
    SunAperture* shape = static_cast<SunAperture*>(getPart("icon", false));
    if (!shape) return;
    shape->findTexture(xPixels, yPixels, surfacesList);
}
