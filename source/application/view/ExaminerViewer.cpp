#include "ExaminerViewer.h"


ExaminerViewer::~ExaminerViewer()
{
    this->bckgroundroot->unref();
    this->foregroundroot->unref();
}

void ExaminerViewer::actualRedraw()
{
    // Must set up the OpenGL viewport manually, as upon resize
    // operations, Coin won't set it up until the SoGLRenderAction is
    // applied again. And since we need to do glClear() before applying
    // the action..
    const SbViewportRegion vp = this->getViewportRegion();
    SbVec2s origin = vp.getViewportOriginPixels();
    SbVec2s size = vp.getViewportSizePixels();
    glViewport(origin[0], origin[1], size[0], size[1]);
    const SbColor col = this->getBackgroundColor();
    glClearColor(col[0], col[1], col[2], 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Render our scene graph with the image.
    SoGLRenderAction * glra = this->getGLRenderAction();
    glra->apply(this->bckgroundroot);
    // Render normal scene graph.
    SoQtExaminerViewer::actualRedraw();
    // Increase arrow angle with 1/1000  every frame.
    arrowrotation->angle = arrowrotation->angle.getValue() + (0.001 / M_PI * 180);
    // Render overlay front scene graph.
    glClear(GL_DEPTH_BUFFER_BIT);
    glra->apply(this->foregroundroot);
}

ExaminerViewer::ExaminerViewer(QWidget* parent, const char* filename)
    : SoQtExaminerViewer(parent)
{
    // Coin should not clear the pixel-buffer, so the background image
    // is not removed.
    this->setClearBeforeRender(FALSE, TRUE);
    // Set up background scene graph with image in it.
    this->bckgroundroot = new SoSeparator;
    this->bckgroundroot->ref();
    SoOrthographicCamera * cam = new SoOrthographicCamera;
    cam->position = SbVec3f(0, 0, 1);
    cam->height = 1;
    // SoImage will be at z==0.0.
    cam->nearDistance = 0.5;
    cam->farDistance = 1.5;
    SoImage * img = new SoImage;
    img->vertAlignment = SoImage::HALF;
    img->horAlignment = SoImage::CENTER;
    img->filename = filename;
    this->bckgroundroot->addChild(cam);
    this->bckgroundroot->addChild(img);
    // Set up foreground, overlaid scene graph.
    this->foregroundroot = new SoSeparator;
    this->foregroundroot->ref();
    SoLightModel * lm = new SoLightModel;
    lm->model = SoLightModel::BASE_COLOR;
    SoBaseColor * bc = new SoBaseColor;
    bc->rgb = SbColor(1, 1, 0);
    cam = new SoOrthographicCamera;
    cam->position = SbVec3f(0, 0, 5);
    cam->height = 10;
    cam->nearDistance = 0;
    cam->farDistance = 10;
    const double ARROWSIZE = 2.0;
    SoTranslation * posit = new SoTranslation;
    posit->translation = SbVec3f(-2.5 * ARROWSIZE, 1.5 * ARROWSIZE, 0);
    arrowrotation = new SoRotationXYZ;
    arrowrotation->axis = SoRotationXYZ::Z;
    SoTranslation * offset = new SoTranslation;
    offset->translation = SbVec3f(ARROWSIZE/2.0, 0, 0);
    SoCube * cube = new SoCube;
    cube->width = ARROWSIZE;
    cube->height = ARROWSIZE/15.0;
    this->foregroundroot->addChild(cam);
    this->foregroundroot->addChild(lm);
    this->foregroundroot->addChild(bc);
    this->foregroundroot->addChild(posit);
    this->foregroundroot->addChild(arrowrotation);
    this->foregroundroot->addChild(offset);
    this->foregroundroot->addChild(cube);
}
