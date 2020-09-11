#include "TTransform.h"

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/nodes/SoSubNode.h>

SO_NODE_SOURCE(TTransform)


const SbRotation TTransform::gScaleOrientation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f);
const SbVec3f TTransform::gCenter(0.0f, 0.0f, 0.0f);

TTransform::TTransform()
{
    SO_NODE_CONSTRUCTOR(TTransform);
    isBuiltIn = TRUE;
    SO_NODE_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));
    SO_NODE_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
    SO_NODE_ADD_FIELD(scale, (1.0f, 1.0f, 1.0f));
}

TTransform::~TTransform()
{

}

void TTransform::initClass(void)
{
    SO_NODE_INIT_CLASS(TTransform,  SoTransformation, "Transformation");

    TSFRotation::initClass();
} 

/*!
  Sets the transformation to translate to \a frompoint, with a rotation
  so that the (0,0,-1) vector is rotated into the vector from \a frompoint
  to \a topoint.
*/
void TTransform::pointAt(const SbVec3f & frompoint, const SbVec3f & topoint)
{
  scale = SbVec3f(1.0f, 1.0f, 1.0f);
  translation = frompoint;
  SbVec3f dir = topoint - frompoint;
  if (dir.normalize() != 0.0f) {
    SbRotation rot(SbVec3f(0.0f, 0.0f, -1.0f), dir);
    this->rotation = rot;
  }
}

/*!
  Calculates the matrices to/from scale space.
*/
void TTransform::getScaleSpaceMatrix(SbMatrix & mat, SbMatrix & inv) const
{  
  SbMatrix tmp;
  mat.multRight(tmp);
  tmp.setScale(scale.getValue());
  mat.multRight(tmp);
  inv = mat.inverse();
}

/*!
  Calculates the matrices to/from rotation space.
*/
void
TTransform::getRotationSpaceMatrix(SbMatrix & mat, SbMatrix & inv) const
{
  SbMatrix tmp;
  tmp.setScale(scale.getValue());
  mat.multRight(tmp);
  tmp.setRotate(rotation.getValue());
  mat.multRight(tmp);
  inv = mat.inverse();
}

/*!
  Calculates the matrices to/from translation space.
*/
void
TTransform::getTranslationSpaceMatrix(SbMatrix & mat, SbMatrix & inv) const
{
  SbMatrix tmp;
  tmp.setScale(scale.getValue());
  mat.multRight(tmp);
  tmp.setRotate(rotation.getValue());
  mat.multRight(tmp);
  tmp.setTranslate(translation.getValue());
  mat.multRight(tmp);
  inv = mat.inverse();
}

/*!
  Premultiplies this transformation by \a mat.
*/
void TTransform::multLeft(const SbMatrix & mat)
{
  SbMatrix m;
  m.setTransform(translation.getValue(), rotation.getValue(), scale.getValue());
  m.multLeft(mat);
  setMatrix(m);
}

/*!
  Postmultiplies this transformation by \a mat.
*/
void TTransform::multRight(const SbMatrix & mat)
{
  SbMatrix m;
  m.setTransform(translation.getValue(), rotation.getValue(), scale.getValue());
  m.multRight(mat);
  setMatrix(m);
}

/*!
  Premultiplies this transformation by the transformation in \a nodeonright.
*/
void TTransform::combineLeft(SoTransformation * nodeonright)
{
  SoGetMatrixAction ma(SbViewportRegion(100,100));
  ma.apply(nodeonright);
  multLeft(ma.getMatrix());
}

/*!
  Postmultiplies this transformation by the transformation in \a nodeonleft.
*/
void TTransform::combineRight(SoTransformation * nodeonleft)
{
  SoGetMatrixAction ma(SbViewportRegion(100,100));
  ma.apply(nodeonleft);
  multRight(ma.getMatrix());
}

/*!
  Sets the fields to create a transformation equal to \a mat.
*/
void TTransform::setMatrix(const SbMatrix & mat)
{  
  SbVec3f t, s, c = gCenter;
  SbRotation r, so;
  mat.getTransform(t, r, s, so, c);
  
  translation = t;
  rotation = r;
  scale = s;
}

/*!  
  Sets the \e center field to \a newcenter. This might affect one
  or more of the other fields.  
*/
void TTransform::recenter(const SbVec3f & newcenter)
{
  SbMatrix matrix;
  matrix.setTransform(translation.getValue(), rotation.getValue(), scale.getValue());
  SbVec3f t, s;
  SbRotation r, so;
  matrix.getTransform(t, r, s, so, newcenter);
  translation = t;
  rotation = r;
  scale = s;
}


void TTransform::doAction(SoAction* action)
{
    SbMatrix m;
    m.setTransform(translation.getValue(), rotation.getValue(), scale.getValue());
    SoModelMatrixElement::mult(action->getState(), this, m);
}

void TTransform::GLRender(SoGLRenderAction* action)
{
    TTransform::doAction(action);
}

void TTransform::getBoundingBox(SoGetBoundingBoxAction* action)
{
    TTransform::doAction((SoAction*) action);
}

void TTransform::getMatrix(SoGetMatrixAction* action)
{
    SbMatrix m;
    m.setTransform(translation.getValue(), rotation.getValue(), scale.getValue());
    action->getMatrix().multLeft(m);

    SbMatrix mi = m.inverse();
    action->getInverse().multRight(mi);
}

void TTransform::callback(SoCallbackAction* action)
{
    TTransform::doAction((SoAction*) action);
}

void TTransform::pick(SoPickAction* action)
{
    TTransform::doAction((SoAction*) action);
}

void TTransform::getPrimitiveCount(SoGetPrimitiveCountAction* action)
{
    TTransform::doAction((SoAction*) action);
}
