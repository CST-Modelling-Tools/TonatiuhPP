#include "TArrayKit.h"

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoTransformSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoMultipleCopy.h>

#include "kernel/scene/MaterialGL.h"

//SO_KIT_SOURCE(TArrayKit)
SO_NODE_SOURCE(TArrayKit)



void TArrayKit::initClass()
{
//    SO_KIT_INIT_CLASS(TArrayKit, SoNode, "Node");
    SO_NODE_INIT_CLASS(TArrayKit, SoNode, "Node");
}

TArrayKit::TArrayKit()
{
//    SO_KIT_CONSTRUCTOR(TArrayKit);
    SO_NODE_CONSTRUCTOR(TArrayKit);

    float vs[][3] = {
        {1, 0, 0},
        {0, 1, 0},
    };
    positions.setValues(0, 2, vs);
    positions.setContainer(this);
    fieldData->addField(this, "positions", &positions);

    SO_NODE_ADD_FIELD(nMax, (5));

    m_root = new SoSeparator;
    m_root->ref();
    m_sensor = new SoFieldSensor(onSensor, this);
//    m_sensor->attach(&positions);
    m_sensor->attach(&nMax);
    m_parent = 0;
    onSensor();
     enableNotify(true);
}

TArrayKit::~TArrayKit()
{
    m_root->unref();
}

void TArrayKit::onSensor(void* data, SoSensor*)
{
    TArrayKit* kit = (TArrayKit*) data;
    kit->onSensor();
}

#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/nodes/SoMatrixTransform.h>
void TArrayKit::onSensor()
{
    m_root->removeAllChildren();

    MaterialGL* material = new MaterialGL;
    m_root->addChild(material);

    float sv[][3] = {
        {0.5, -0.5, 0},
        {0.5, 0.5, 0},
        {-0.5, 0.5, 0},
        {-0.5, -0.5, 0},
    };
    SoCoordinate3* sVertices = new SoCoordinate3;
    sVertices->point.setValues(0, 4, sv);
    m_root->addChild(sVertices);

    int faces[] = {
        0, 1, 2, -1,
        2, 3, 0, -1
    };
    SoIndexedFaceSet* sMesh = new SoIndexedFaceSet;
    sMesh->coordIndex.setValues(0, 2*4, faces);

    int kMax = nMax.getValue();
    int mMax = kMax;

//    for (int n = 0; n < kMax; ++n)
//        for (int m = 0; m < mMax; ++m) {
//            SoTransformSeparator* sep = new SoTransformSeparator;
//            SoTransform* transform = new SoTransform;
//            transform->translation = SbVec3f(n*3, m*3, 0);
//            sep->addChild(transform);
//            sep->addChild(sMesh);
//            m_root->addChild(sep);
//        }

    SoMultipleCopy* mc = new SoMultipleCopy;
    mc->addChild(sMesh);
    int counter = 0;
    for (int n = 0; n < kMax; ++n)
        for (int m = 0; m < mMax; ++m) {
            SbMatrix matrix;
            matrix.setTranslate(SbVec3f(n*3, m*3, 0));
            mc->matrix.set1Value(counter++, matrix);
        }
    m_root->addChild(mc);
}

#include <Inventor/actions/SoGetBoundingBoxAction.h>
void TArrayKit::getBoundingBox(SoGetBoundingBoxAction* action)
{
    int q = nMax.getValue()*3;
    SbBox3f box(-q, -q, -q, q, q, q);
    action->extendBy(box);
}

void TArrayKit::GLRender(SoGLRenderAction* action)
{
    m_root->GLRender(action);
}
