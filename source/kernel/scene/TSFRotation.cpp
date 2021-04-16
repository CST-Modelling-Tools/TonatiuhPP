#include "TSFRotation.h"

#include <Inventor/fields/SoSFRotation.h>

#include <Inventor/SbVec3f.h>
#include <Inventor/errors/SoDebugError.h>

#include "libraries/math/gcf.h"


SbBool sosfrotation_read_valueU(SoInput* in, SbRotation & r)
{
  float f[4];
  for (int i = 0; i < 4; i++) {
    if (!in->read(f[i])) return FALSE;
  }
  SbVec3f axis(f[0], f[1], f[2]);
  const float angle = f[3]*gcf::degree;

  // vrml97 identity rotations are often specified with a null vector.
  // test for this and just set to z-axis.
  if (axis == SbVec3f(0.0f, 0.0f, 0.0f) && angle == 0.0f) {
    axis = SbVec3f(0.0f, 0.0f, 1.0f);
  }
  r.setValue(axis, angle);
  return TRUE;
}

void sosfrotation_write_valueU(SoOutput* out, const SbRotation & r)
{
  SbVec3f axis;
  float angle;
  r.getValue(axis, angle);

  if (axis.length() == 0.0f) {
    axis.setValue(0.0f, 0.0f, 1.0f);
    angle = 0.0f;
  }

  out->setFloatPrecision(6);
  out->write(axis[0]);
  if(!out->isBinary()) out->write(' ');
  out->write(axis[1]);
  if(!out->isBinary()) out->write(' ');
  out->write(axis[2]);
  if(!out->isBinary()) out->write("  ");
  out->write(float(angle/gcf::degree)); // less digits
}


// *************************************************************************

SO_SFIELD_SOURCE(TSFRotation, SbRotation, const SbRotation &)

// *************************************************************************

/*!
  \copydetails SoField::initClass(void)
*/
void TSFRotation::initClass(void)
{
  SO_SFIELD_INIT_CLASS(TSFRotation, SoSField);
}

SbBool TSFRotation::readValue(SoInput* in)
{
  SbRotation r;
  if (!sosfrotation_read_valueU(in, r)) return FALSE;
  setValue(r);
  return TRUE;
}

void TSFRotation::writeValue(SoOutput* out) const
{
  sosfrotation_write_valueU(out, this->getValue());
}


/*!
  Return value of rotation as an \a axis and an \a angle around this
  \a axis.
*/
void TSFRotation::getValue(SbVec3f& axis, float& angle) const
{
    getValue().getValue(axis, angle);
}

/*!
  Set the rotation from a set of quaternion values.
*/
void TSFRotation::setValue(const float q0, const float q1,
                             const float q2, const float q3)
{
    setValue(SbRotation(q0, q1, q2, q3));
}

/*!
  Set the rotation from a set of quaternion values.
*/
void TSFRotation::setValue(const float q[4])
{
    setValue(SbRotation(q));
}

/*!
  Set the rotation from an \a axis and an \a angle around
  this \a axis.
*/
void TSFRotation::setValue(const SbVec3f & axis, const float angle)
{
    setValue(SbRotation(axis, angle));
}

