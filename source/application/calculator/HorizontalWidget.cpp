#include "HorizontalWidget.h"

#include <QLabel>
#include <QVBoxLayout>

#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoText3.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>

#include "libraries/math/gcf.h"
#include "libraries/math/3D/Ray.h"
#include "libraries/math/3D/vec3d.h"


HorizontalWidget::HorizontalWidget(QWidget* parent):
    QWidget(parent),
    sphereRadio(120.),
    m_azimuth(0.),
    m_zenith(0.)
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    // viewer
    QWidget* examinerWidget = new QWidget;
    mainLayout->addWidget(examinerWidget);
    mainLayout->setStretch(0, 1);

    m_rootNode = new SoSeparator;
    m_rootNode->addChild( Ejes( ) );
    m_rootNode->addChild( Text() );
    m_rootNode->addChild( Sphere() );
    m_rootNode->addChild( Horizon() );
    m_rootNode->addChild( AzimuthLine() );
    m_rootNode->addChild( ZenithLine() );
    m_rootNode->addChild( Star() );

    SoQtExaminerViewer* viewer = new SoQtExaminerViewer(examinerWidget);
    viewer->setTransparencyType(SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_BLEND);
    viewer->setSceneGraph(m_rootNode);
    viewer->setBackgroundColor(SbColor(0.86f, 0.86f, 0.86f));
    viewer->setDecoration(false);

    // labels
    QWidget* labelsWidget = new QWidget;
    mainLayout->addWidget(labelsWidget);
    QGridLayout* labelsLayout = new QGridLayout;
    labelsWidget->setLayout(labelsLayout);

    QLabel* azimuthLabel = new QLabel("Azimuth:");
    labelsLayout->addWidget(azimuthLabel, 0, 0, 1, 1);

    m_azimuthValue = new QLabel(QString::number(m_azimuth) );
    labelsLayout->addWidget(m_azimuthValue, 0, 1, 1, 3);

    QLabel* zenithLabel = new QLabel("Zenith:");
    labelsLayout->addWidget(zenithLabel, 1, 0, 1, 1);

    m_zenithValue = new QLabel(QString::number(m_zenith));
    labelsLayout->addWidget(m_zenithValue, 1, 1, 1, 3);
}

HorizontalWidget::~HorizontalWidget()
{

}

void HorizontalWidget::CoordinatesChanged(cSunCoordinates coordinates)
{
    m_azimuth = coordinates.dAzimuth;
    m_azimuthValue->setText(QString::number(m_azimuth));
    m_zenith = coordinates.dZenithAngle;
    m_zenithValue->setText(QString::number(m_zenith));

    SoSeparator* azimuth = AzimuthLine();
    m_rootNode->replaceChild(4, azimuth);

    SoSeparator* zenith = ZenithLine();
    m_rootNode->replaceChild(5, zenith);

    SoSeparator* star = Star();
    m_rootNode->replaceChild(6, star);
}

SoSeparator* HorizontalWidget::AzimuthLine()
{
    //Azimuth Line
    SoSeparator* azimuth = new SoSeparator;

    SoSeparator* azimuthLine = new SoSeparator;
    azimuth->addChild( azimuthLine );

    SoMaterial* azimuthMaterial = new SoMaterial;
    azimuthMaterial->diffuseColor.setValue( 0.0, 1.0, 0.0 );   // Green
    azimuthLine->addChild( azimuthMaterial );

    SoDrawStyle * drawstyle = new SoDrawStyle;
    drawstyle->lineWidth = 3;
    azimuthLine->addChild( drawstyle );


    float azimuthPoints[360][3];
    for( int fi = 0; fi < 360; ++fi )
    {
        double grad = ( m_azimuth / 360 ) * fi;
        azimuthPoints[fi][0] = sin( grad * (gcf::pi / 180) )* sphereRadio ;
        azimuthPoints[fi][1] = 0.;
        azimuthPoints[fi][2] = -cos( grad * (gcf::pi / 180) ) * sphereRadio ;
    }

    int azimuthLines[1] = {360};
    SoCoordinate3 * azimuthCoord3 = new SoCoordinate3;
    azimuthCoord3->point.setValues( 0, 360, azimuthPoints );
    azimuthLine->addChild( azimuthCoord3 );

    SoLineSet* line=new SoLineSet;
    line->numVertices.setValues( 0, 1, azimuthLines );
    azimuthLine->addChild( line );


    SoSeparator* curve = new SoSeparator;
    azimuth->addChild( curve );

    SoMaterial* curveMaterial = new SoMaterial;
    curveMaterial->diffuseColor.setValue( 0.0f, 1.0f, 0.0f );   // Green
    curve->addChild( curveMaterial );

    float curvePoints[270][3];
    vec3d center( 0, 0, 0 );

    int numPoints = 0;
    int indexes[360];
    for( int index = 1; index <= 90; ++index )
    {
        curvePoints[numPoints][0] = center.x;
        curvePoints[numPoints][1] = center.y;
        curvePoints[numPoints][2] = center.z;
        indexes[(index - 1)*4] = numPoints;
        numPoints++;

        double grad1 = (m_azimuth / 360) * 4 * (index - 1);
        curvePoints[numPoints][0] = sin( grad1 * (gcf::pi / 180) )* sphereRadio ;
        curvePoints[numPoints][1] =  0.0;
        curvePoints[numPoints][2] = -cos( grad1 * (gcf::pi / 180) ) * sphereRadio ;
        indexes[ ( ( index -1 ) * 4  ) +1 ] = numPoints;
        numPoints++;

        double grad2 = ( m_azimuth / 360 ) * 4 * index;
        curvePoints[numPoints][0] = sin( grad2 * (gcf::pi / 180) )* sphereRadio ;
        curvePoints[numPoints][1] =  0.0;
        curvePoints[numPoints][2] = -cos( grad2 * (gcf::pi / 180) ) * sphereRadio ;
        indexes[ ( ( index -1 ) * 4  ) +2 ] = numPoints;
        numPoints++;

        indexes[ ( ( index -1 ) * 4  ) + 3 ] = -1;
    }

    SoCoordinate3 * curveCoord3 = new SoCoordinate3;
    curveCoord3->point.setValues(0, 270, curvePoints);
    curve->addChild(curveCoord3);

    SoIndexedFaceSet* facet = new SoIndexedFaceSet;
    facet->coordIndex.setValues(0, 360, indexes);
    curve->addChild(facet);

    return azimuth;
}

SoSeparator* HorizontalWidget::Ejes() const
{
    SoSeparator* ejes = new SoSeparator;

    SoMaterial *myMaterial = new SoMaterial;
    myMaterial->diffuseColor.setValue(1.0, 0.0, 0.0);   // Red
    ejes->addChild(myMaterial);

    float p[6][3]={{0.0, 0.0, 0.0},
                   { float( sphereRadio+20 ), 0.0, 0.0},
                   {0.0, 0.0, 0.0},
                   {0.0, float( sphereRadio+20 ), 0.0},
                   {0.0, 0.0, 0.0},
                   {0.0, 0.0, float( sphereRadio+20 )}};

    SoCoordinate3* coord3 = new SoCoordinate3;
    coord3->point.setValues(0, 6, p);
    ejes->addChild(coord3);

    int lines[3]={2,2, 2};
    SoLineSet* lineset=new SoLineSet;
    lineset->numVertices.setValues(0,3,lines);
    ejes->addChild(lineset);

    return ejes;
}

SoSeparator* HorizontalWidget::Horizon() const
{
    float p[360][3];
    for( int fi = 0; fi<360; ++fi )
    {
        p[fi][0] = -sin( fi * (gcf::pi / 180) ) * sphereRadio ;
        p[fi][1] =  0.0;
        p[fi][2] = -cos( fi * (gcf::pi / 180) ) * sphereRadio ;
    }

    int lines[1]={360};
    SoCoordinate3 * coord3 = new SoCoordinate3;
    coord3->point.setValues(0, 360, p);
    SoSeparator* ecuador = new SoSeparator;
    ecuador->addChild(coord3);

    SoLineSet* lineset=new SoLineSet;
    lineset->numVertices.setValues(0,1,lines);
    ecuador->addChild(lineset);

    return ecuador;
}

SoSeparator* HorizontalWidget::Sphere() const
{
    SoSeparator* sph = new SoSeparator;

    SoMaterial* myMaterial = new SoMaterial;
    myMaterial->diffuseColor.setValue(0.0f, 0.0f, 1.0f);
    myMaterial->transparency.setValue(0.3f);
    sph->addChild(myMaterial);

    SoComplexity* complexity = new SoComplexity;
    complexity->value = 0.5;
    sph->addChild(complexity);

    SoSphere* sphere = new SoSphere;
    sphere->radius = sphereRadio;
    sph->addChild(sphere);

    return sph;
}

SoSeparator* HorizontalWidget::Star()
{
    //Star definition
    SoSeparator* star = new SoSeparator;

    SoMaterial *myMaterial = new SoMaterial;
    myMaterial->diffuseColor.setValue(1.0, 1.0, 0.0);   // yellow
    star->addChild(myMaterial);

    SoTranslation* starTransform = new SoTranslation;
    starTransform->translation.setValue( sin( m_zenith * (gcf::pi / 180) )* sin( m_azimuth * (gcf::pi / 180) ) * sphereRadio,
                                         cos( m_zenith * (gcf::pi / 180) ) * sphereRadio,
                                         -sin( m_zenith * (gcf::pi / 180) )* cos( m_azimuth * (gcf::pi / 180) )  * sphereRadio ) ;
    star->addChild( starTransform);

    SoSphere* sphere=new SoSphere;
    sphere->radius=10;
    star->addChild(sphere);

    return star;
}

SoSeparator* HorizontalWidget::Text()
{
    SoSeparator* text = new SoSeparator;

    SoMaterial* myMaterial = new SoMaterial;
    myMaterial->diffuseColor.setValue( 1.0, 0.0, 0.0 );   // Red
    text->addChild( myMaterial );

    SoSeparator* norte = new SoSeparator;
    SoTransform* norteTransf = new SoTransform;
    norteTransf->translation.setValue( SbVec3f( 0, 0, -( sphereRadio+30) ) );
    norte->addChild( norteTransf );

    SoText3* northText= new SoText3();
    northText->string.setValue("N");
    northText->justification = SoText3::CENTER;
    northText->parts = SoText3::ALL;
    norte->addChild( northText );
    text->addChild( norte );

    SoSeparator* south = new SoSeparator;
    SoTransform* southTransf = new SoTransform;
    southTransf->translation.setValue( SbVec3f( 0, 0, ( sphereRadio+30 ) ) );
    south->addChild( southTransf );

    SoText3* southText = new SoText3();
    southText->string.setValue("S");
    southText->justification = SoText3::CENTER;
    southText->parts = SoText3::ALL;
    south->addChild( southText );
    text->addChild( south );

    SoSeparator* east = new SoSeparator;
    SoTransform* eastTransf = new SoTransform;
    eastTransf->translation.setValue( SbVec3f( ( sphereRadio+30 ), 0.0, 0.0 ) );
    east->addChild( eastTransf );

    SoText3* eastText = new SoText3();
    eastText->string.setValue("E");
    eastText->justification = SoText3::CENTER;
    eastText->parts = SoText3::ALL;
    east->addChild( eastText );
    text->addChild( east );

    SoSeparator* west = new SoSeparator;
    SoTransform* westTransf = new SoTransform;
    westTransf->translation.setValue( SbVec3f( -( sphereRadio+30 ), 0.0, 0.0 ) );
    west->addChild( westTransf );

    SoText3* westText = new SoText3();
    westText->string.setValue("W");
    westText->justification = SoText3::CENTER;
    westText->parts = SoText3::ALL;
    west->addChild( westText );
    text->addChild( west );

    return text;
}

SoSeparator* HorizontalWidget::ZenithLine()
{
    SoSeparator* zenith = new SoSeparator;

    SoMaterial* zenithMaterial = new SoMaterial;
    zenithMaterial->diffuseColor.setValue( 0.0, 1.0, 0.0 );   // Green
    zenith->addChild( zenithMaterial );

    //Zenith Line
    SoSeparator* zenithLine = new SoSeparator;
    zenith->addChild( zenithLine );

    SoDrawStyle * drawstyle = new SoDrawStyle;
    drawstyle->lineWidth = 4;
    zenithLine->addChild( drawstyle );

    float zenithPoints[360][3];

    double fi = m_azimuth;
    for( int theta = 0; theta< 360; ++theta )
    {
        double grad = ( m_zenith / 360 ) * theta;
        zenithPoints[theta][0] = sin( grad * (gcf::pi / 180) ) * sin( fi * (gcf::pi / 180) )* sphereRadio;
        zenithPoints[theta][1] = cos( grad  * (gcf::pi / 180) ) * sphereRadio;
        zenithPoints[theta][2] = -sin( grad * (gcf::pi / 180) ) * cos( fi * (gcf::pi / 180) ) * sphereRadio ;
    }


    int zenithLines[1] = {360};
    SoCoordinate3 * zenithCoord3 = new SoCoordinate3;
    zenithCoord3->point.setValues( 0, 360, zenithPoints );
    zenithLine->addChild( zenithCoord3 );

    SoLineSet* line=new SoLineSet;
    line->numVertices.setValues( 0, 1, zenithLines );
    zenithLine->addChild( line );

    //Surface
    SoSeparator* curve = new SoSeparator;
    zenith->addChild( curve );

    float curvePoints[270][3];
    vec3d center( 0, 0, 0 );

    int numPoints = 0;
    int indexes[360];
    for( int index = 1; index <= 90; ++index )
    {
        curvePoints[numPoints][0]= center.x;
        curvePoints[numPoints][1]= center.y;
        curvePoints[numPoints][2]= center.z;
        indexes[ ( index -1 ) * 4 ] = numPoints;
        numPoints++;

        double grad1 = ( m_zenith / 360 ) * 4 * (index -1 );
        curvePoints[numPoints][0] = sin( grad1 * (gcf::pi / 180) ) * sin( m_azimuth * (gcf::pi / 180) )* sphereRadio;
        curvePoints[numPoints][1] = cos( grad1  * (gcf::pi / 180) ) * sphereRadio;
        curvePoints[numPoints][2] = -sin( grad1 * (gcf::pi / 180) ) * cos( m_azimuth * (gcf::pi / 180) ) * sphereRadio ;
        indexes[ ( ( index -1 ) * 4  ) +1 ] = numPoints;
        numPoints++;

        double grad2 = ( m_zenith / 360 ) * 4 * index;
        curvePoints[numPoints][0] = sin( grad2 * (gcf::pi / 180) ) * sin( m_azimuth * (gcf::pi / 180) )* sphereRadio;
        curvePoints[numPoints][1] = cos( grad2 * (gcf::pi / 180) ) * sphereRadio;
        curvePoints[numPoints][2] = -sin( grad2 * (gcf::pi / 180) ) * cos( m_azimuth * (gcf::pi / 180) ) * sphereRadio ;
        indexes[ ( ( index -1 ) * 4  ) +2 ] = numPoints;
        numPoints++;

        indexes[ ( ( index -1 ) * 4  ) + 3 ] = -1;
    }


    SoCoordinate3 * curveCoord3 = new SoCoordinate3;
    curveCoord3->point.setValues( 0, 270, curvePoints );
    curve->addChild( curveCoord3 );

    SoIndexedFaceSet* facet = new SoIndexedFaceSet;
    facet->coordIndex.setValues( 0, 360, indexes );
    curve->addChild( facet );

    return zenith;
}
