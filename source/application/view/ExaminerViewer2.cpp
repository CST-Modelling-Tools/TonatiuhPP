#include "ExaminerViewer2.h"


ExaminerViewer2::~ExaminerViewer2()
{

}

void ExaminerViewer2::actualRedraw()
{

    SoQtExaminerViewer::actualRedraw();
    drawAxisCross();

}

ExaminerViewer2::ExaminerViewer2(QWidget* parent)
    : SoQtExaminerViewer(parent)
{

}

void
ExaminerViewer2::drawAxisCross(void)
{
  // FIXME: convert this to a superimposition scenegraph instead of
  // OpenGL calls. 20020603 mortene.

  // Store GL state.
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  GLfloat depthrange[2];
  glGetFloatv(GL_DEPTH_RANGE, depthrange);
  GLdouble projectionmatrix[16];
  glGetDoublev(GL_PROJECTION_MATRIX, projectionmatrix);

  glDepthFunc(GL_ALWAYS);
  glDepthMask(GL_TRUE);
  glDepthRange(0, 0);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
//  glDisable(GL_BLEND); // Kills transparency.

  // Set the viewport in the OpenGL canvas. Dimensions are calculated
  // as a percentage of the total canvas size.
  SbVec2s view = getGLSize();
  const int pixelarea =
    int(float(30.)/100.0f * std::min(view[0], view[1]));
#if 0 // middle of canvas
  SbVec2s origin(view[0]/2 - pixelarea/2, view[1]/2 - pixelarea/2);
#endif // middle of canvas
#if 1 // lower right of canvas
  SbVec2s origin(view[0] - pixelarea, 0);
#endif // lower right of canvas
  glViewport(origin[0], origin[1], pixelarea, pixelarea);



  // Set up the projection matrix.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  const float NEARVAL = 0.1f;
  const float FARVAL = 10.0f;
  const float dim = NEARVAL * float(tan(M_PI / 8.0)); // FOV is 45° (45/360 = 1/8)
  glFrustum(-dim, dim, -dim, dim, NEARVAL, FARVAL);


  // Set up the model matrix.
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  SbMatrix mx;
  SoCamera * cam = getCamera();

  // If there is no camera (like for an empty scene, for instance),
  // just use an identity rotation.
  if (cam) { mx = cam->orientation.getValue(); }
  else { mx = SbMatrix::identity(); }

  mx = mx.inverse();
  mx[3][2] = -3.5; // Translate away from the projection point (along z axis).
  glLoadMatrixf((float *)mx);


  // Find unit vector end points.
  SbMatrix px;
  glGetFloatv(GL_PROJECTION_MATRIX, (float *)px);
  SbMatrix comb = mx.multRight(px);

  SbVec3f xpos;
  comb.multVecMatrix(SbVec3f(1,0,0), xpos);
  xpos[0] = (1 + xpos[0]) * view[0]/2;
  xpos[1] = (1 + xpos[1]) * view[1]/2;
  SbVec3f ypos;
  comb.multVecMatrix(SbVec3f(0,1,0), ypos);
  ypos[0] = (1 + ypos[0]) * view[0]/2;
  ypos[1] = (1 + ypos[1]) * view[1]/2;
  SbVec3f zpos;
  comb.multVecMatrix(SbVec3f(0,0,1), zpos);
  zpos[0] = (1 + zpos[0]) * view[0]/2;
  zpos[1] = (1 + zpos[1]) * view[1]/2;


  // Render the cross.
  {
    glLineWidth(2.0);

    enum { XAXIS, YAXIS, ZAXIS };
    int idx[3] = { XAXIS, YAXIS, ZAXIS };
    float val[3] = { xpos[2], ypos[2], zpos[2] };

    // Bubble sort.. :-}
    if (val[0] < val[1]) { std::swap(val[0], val[1]);std::swap(idx[0], idx[1]); }
    if (val[1] < val[2]) { std::swap(val[1], val[2]); std::swap(idx[1], idx[2]); }
    if (val[0] < val[1]) { std::swap(val[0], val[1]); std::swap(idx[0], idx[1]); }
    assert((val[0] >= val[1]) && (val[1] >= val[2])); // Just checking..

    for (int i=0; i < 3; i++) {
      glPushMatrix();
      if (idx[i] == XAXIS) {                       // X axis.
        glColor3f(0.500f, 0.125f, 0.125f);
      } else if (idx[i] == YAXIS) {                // Y axis.
        glRotatef(90, 0, 0, 1);
        glColor3f(0.125f, 0.500f, 0.125f);
      } else {                                     // Z axis.
        glRotatef(-90, 0, 1, 0);
        glColor3f(0.125f, 0.125f, 0.500f);
      }
      this->drawArrow();
      glPopMatrix();
    }
  }

  // Render axis notation letters ("X", "Y", "Z").
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, view[0], 0, view[1], -1, 1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  GLint unpack;
  glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glColor3fv(SbVec3f(0.8f, 0.8f, 0.0f).getValue());

  glRasterPos2d(xpos[0], xpos[1]);
//  glBitmap(8, 7, 0, 0, 0, 0, xbmp);
  glRasterPos2d(ypos[0], ypos[1]);
//  glBitmap(8, 7, 0, 0, 0, 0, ybmp);
  glRasterPos2d(zpos[0], zpos[1]);
//  glBitmap(8, 7, 0, 0, 0, 0, zbmp);

  glPixelStorei(GL_UNPACK_ALIGNMENT, unpack);
  glPopMatrix();

  // Reset original state.

  // FIXME: are these 3 lines really necessary, as we push
  // GL_ALL_ATTRIB_BITS at the start? 20000604 mortene.
  glDepthRange(depthrange[0], depthrange[1]);
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixd(projectionmatrix);

  glPopAttrib();
}

// Draw an arrow for the axis representation directly through OpenGL.
void
ExaminerViewer2::drawArrow(void)
{
  glBegin(GL_LINES);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(1.0f, 0.0f, 0.0f);
  glEnd();
  glDisable(GL_CULL_FACE);
  glBegin(GL_TRIANGLES);
  glVertex3f(1.0f, 0.0f, 0.0f);
  glVertex3f(1.0f - 1.0f / 3.0f, +0.5f / 4.0f, 0.0f);
  glVertex3f(1.0f - 1.0f / 3.0f, -0.5f / 4.0f, 0.0f);
  glVertex3f(1.0f, 0.0f, 0.0f);
  glVertex3f(1.0f - 1.0f / 3.0f, 0.0f, +0.5f / 4.0f);
  glVertex3f(1.0f - 1.0f / 3.0f, 0.0f, -0.5f / 4.0f);
  glEnd();
  glBegin(GL_QUADS);
  glVertex3f(1.0f - 1.0f / 3.0f, +0.5f / 4.0f, 0.0f);
  glVertex3f(1.0f - 1.0f / 3.0f, 0.0f, +0.5f / 4.0f);
  glVertex3f(1.0f - 1.0f / 3.0f, -0.5f / 4.0f, 0.0f);
  glVertex3f(1.0f - 1.0f / 3.0f, 0.0f, -0.5f / 4.0f);
  glEnd();
}
