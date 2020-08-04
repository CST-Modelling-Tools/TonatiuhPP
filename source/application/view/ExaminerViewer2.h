#pragma once

// This example shows how to put a permanent background image on
// your viewer canvas, below the 3D graphics, plus overlay
// foreground geometry.  Written by mortene.
// Copyright Kongsberg Oil & Gas Technologies 2002.
// *************************************************************************
#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoImage.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTranslation.h>
#include <GL/gl.h>


class ExaminerViewer2 : public SoQtExaminerViewer {
public:
  ExaminerViewer2(QWidget* parent);
  ~ExaminerViewer2();

protected:
  virtual void actualRedraw(void);

private:
void drawAxisCross();
void drawArrow();
};
