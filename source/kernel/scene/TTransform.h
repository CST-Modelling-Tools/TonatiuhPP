#pragma once

#include "kernel/TonatiuhKernel.h"

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/nodes/SoTransformation.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbVec3f.h>

#include "kernel/scene/TSFRotation.h"


class TONATIUH_KERNEL TTransform: public SoTransformation
{
  typedef SoTransformation inherited;

  SO_NODE_HEADER(TTransform);

public:
  static void initClass(void);
  TTransform(void);

  SoSFVec3f translation;
  TSFRotation rotation;
  SoSFVec3f scale;
  static const SbRotation gScaleOrientation;
  static const SbVec3f gCenter;

  void pointAt(const SbVec3f & frompoint, const SbVec3f & topoint);
  void getScaleSpaceMatrix(SbMatrix & mat, SbMatrix & inv) const;
  void getRotationSpaceMatrix(SbMatrix & mat, SbMatrix & inv) const;
  void getTranslationSpaceMatrix(SbMatrix & mat, SbMatrix & inv) const;
  void multLeft(const SbMatrix & mat);
  void multRight(const SbMatrix & mat);
  void combineLeft(SoTransformation * nodeonright);
  void combineRight(SoTransformation * nodeonleft);
  void setMatrix(const SbMatrix & mat);
  void recenter(const SbVec3f & newcenter);

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void getMatrix(SoGetMatrixAction * action);
  virtual void pick(SoPickAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

protected:
  ~TTransform();
};
