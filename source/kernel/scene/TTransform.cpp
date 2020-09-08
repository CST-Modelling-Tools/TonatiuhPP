#include "TTransform.h"

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/nodes/SoSubNode.h>

//#include "nodes/SoSubNodeP.h"

SO_NODE_SOURCE(TTransform)


SbRotation TTransform::gScaleOrientation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f);
SbVec3f TTransform::gCenter(0.0f, 0.0f, 0.0f);

TTransform::TTransform()
{
  SO_NODE_CONSTRUCTOR(TTransform);

  SO_NODE_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_NODE_ADD_FIELD(scale, (1.0f, 1.0f, 1.0f));
}

TTransform::~TTransform()
{

}

void
TTransform::initClass(void)
{
  SO_NODE_INIT_CLASS(TTransform,  SoTransformation, "Transformation");
} 

/*!
  Sets the transformation to translate to \a frompoint, with a rotation
  so that the (0,0,-1) vector is rotated into the vector from \a frompoint
  to \a topoint.
*/
void TTransform::pointAt(const SbVec3f & frompoint, const SbVec3f & topoint)
{
  this->scale = SbVec3f(1.0f, 1.0f, 1.0f);
  this->translation = frompoint;
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
  tmp.setScale(this->scale.getValue());
  mat.multRight(tmp);
  tmp.setRotate(this->rotation.getValue());
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
  tmp.setScale(this->scale.getValue());
  mat.multRight(tmp);
  tmp.setRotate(this->rotation.getValue());
  mat.multRight(tmp);
  tmp.setTranslate(this->translation.getValue());
  mat.multRight(tmp);
  inv = mat.inverse();
}

/*!
  Premultiplies this transformation by \a mat.
*/
void
TTransform::multLeft(const SbMatrix & mat)
{
  SbMatrix matrix;
  matrix.setTransform(this->translation.getValue(),
                      this->rotation.getValue(),
                      this->scale.getValue(),
                      gScaleOrientation,
                      gCenter);

  matrix.multLeft(mat);
  this->setMatrix(matrix);
}

/*!
  Postmultiplies this transformation by \a mat.
*/
void
TTransform::multRight(const SbMatrix & mat)
{
  SbMatrix matrix;
  matrix.setTransform(this->translation.getValue(),
                      this->rotation.getValue(),
                      this->scale.getValue(),
                      gScaleOrientation,
                      gCenter);
  matrix.multRight(mat);
  this->setMatrix(matrix);
}

/*!
  Premultiplies this transformation by the transformation in \a nodeonright.
*/
void
TTransform::combineLeft(SoTransformation * nodeonright)
{
  SoGetMatrixAction ma(SbViewportRegion(100,100));
  ma.apply(nodeonright);
  this->multLeft(ma.getMatrix());
}

/*!
  Postmultiplies this transformation by the transformation in \a nodeonleft.
*/
void
TTransform::combineRight(SoTransformation * nodeonleft)
{
  SoGetMatrixAction ma(SbViewportRegion(100,100));
  ma.apply(nodeonleft);
  this->multRight(ma.getMatrix());
}

/*!
  Sets the fields to create a transformation equal to \a mat.
*/
void
TTransform::setMatrix(const SbMatrix & mat)
{  
  SbVec3f t,s,c = gCenter;
  SbRotation r, so;
  mat.getTransform(t,r,s,so,c);
  
  this->translation = t;
  this->rotation = r;
  this->scale = s;
}

/*!  
  Sets the \e center field to \a newcenter. This might affect one
  or more of the other fields.  
*/
void
TTransform::recenter(const SbVec3f & newcenter)
{
  SbMatrix matrix;
  matrix.setTransform(this->translation.getValue(),
                      this->rotation.getValue(),
                      this->scale.getValue(),
                      gScaleOrientation,
                      gCenter);
  SbVec3f t,s;
  SbRotation r, so;
  matrix.getTransform(t, r, s, so, newcenter);
  this->translation = t;
  this->rotation = r;
  this->scale = s;
}


void TTransform::doAction(SoAction * action)
{
  SbMatrix matrix;
  matrix.setTransform(this->translation.getValue(),
                      this->rotation.getValue(),
                      this->scale.getValue(),
                      gScaleOrientation,
                      gCenter);
  
  SoModelMatrixElement::mult(action->getState(), this, matrix);
}

void TTransform::GLRender(SoGLRenderAction * action)
{
  TTransform::doAction((SoAction *)action);
}

void TTransform::getBoundingBox(SoGetBoundingBoxAction * action)
{
  TTransform::doAction((SoAction *)action);
}

void TTransform::getMatrix(SoGetMatrixAction * action)
{
  SbMatrix m;
  m.setTransform(this->translation.getValue(),
                 this->rotation.getValue(),
                 this->scale.getValue(),
                 gScaleOrientation,
                 gCenter);
  action->getMatrix().multLeft(m);
  
  SbMatrix mi = m.inverse();
  action->getInverse().multRight(mi);
}

void TTransform::callback(SoCallbackAction * action)
{
  TTransform::doAction((SoAction *)action);
}

void TTransform::pick(SoPickAction * action)
{
  TTransform::doAction((SoAction *)action);
}

void TTransform::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  TTransform::doAction((SoAction *)action);
}
