#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QSplashScreen>
#include <QTime>

#include <Inventor/Qt/SoQt.h>

#include "gui/GraphicRootTracker.h"
#include "gui/MainWindow.h"
#include "kernel/raytracing/TCube.h"
#include "kernel/raytracing/TDefaultMaterial.h"
#include "kernel/raytracing/TDefaultSunShape.h"
#include "kernel/raytracing/TDefaultTracker.h"
#include "kernel/raytracing/TDefaultTransmissivity.h"
#include "kernel/raytracing/TLightKit.h"
#include "kernel/raytracing/TLightShape.h"
#include "kernel/raytracing/TSceneKit.h"
#include "kernel/raytracing/TSceneTracker.h"
#include "kernel/raytracing/TSeparatorKit.h"
#include "kernel/raytracing/TShapeKit.h"
#include "kernel/raytracing/TSquare.h"
#include "kernel/raytracing/TTrackerForAiming.h"
#include "kernel/raytracing/TTransmissivity.h"
#include "libraries/fields/UserMField.h"
#include "libraries/fields/UserSField.h"


#include <QScriptEngine>
#include <QTextStream>
#include "gui/PluginManager.h"
#include "raytracing/ScriptRayTracer.h"
#include "raytracing/tonatiuh_script.h"

/*!
   \mainpage
   The Tonatiuh project aims to create an open source, cutting-edge, accurate, and easy to use Monte Carlo ray tracer for the optical simulation of solar concentrating systems. It intends to advance the state-of-the-art of the simulation tools available for the design and analysis of solar concentrating systems, and to make those tools freely available to anyone interested in using and improving them. Some of the most relevant design goals of Tonatiuh are:
   <ol>
   <li>To develop a robust theoretical foundation that will facilitate the optical simulation of almost any type of solar concentrating systems.</li>
   <li>To exhibit a clean and flexible software architecture, that will allow the user to adapt, expand, increase, and modify its functionalities with ease.</li>
   <li>To achieve operating system independence at source level, and run on all major platforms with none, or minor, modifications to its source code.</li>
   <li>To provide the users with an advanced and easy-of-use Graphic User Interface (GUI).</li>
   </ol>

   <b>IMPORTANT NOTE: the online documentation for the Tonatiuh is still a work-in-progress.</b>
 */


//!  Application entry point.
/*!
   Tonatiuh's main() function. It starts SoQt and Coin3D. It also initializes the
   application specific Coin3D extension subclasses, and the application loop.
 */

Q_DECLARE_METATYPE(QVector<QVariant>)

int main(int argc, char** argv)
{
//    QApplication::setColorSpec(QApplication::CustomColor);

    QApplication a(argc, argv);
    a.setApplicationVersion(APP_VERSION);

    QSplashScreen* splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/icons/SplashScreen.png") );
    splash->show();

    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;

    splash->showMessage(QObject::tr("Loading libraries..."), topRight, Qt::black);


    QApplication::addLibraryPath(QApplication::applicationDirPath()
                                 + QDir::separator() + "marble");

    SoQt::init( (QWidget*) NULL);

    UserMField::initClass();
    UserSField::initClass();
    TSceneKit::initClass();
    TMaterial::initClass();
    TDefaultMaterial::initClass();
    TSeparatorKit::initClass();
    TShape::initClass();
    TCube::initClass();
    TLightShape::initClass();
    TShapeKit::initClass();
    TSquare::initClass();
    TLightKit::initClass();
    TSunShape::initClass();
    TDefaultSunShape::initClass();
    TTracker::initClass();
    TTrackerForAiming::initClass();
    TDefaultTracker::initClass();
    TSceneTracker::initClass();
    GraphicRootTracker::initClass();
    TTransmissivity::initClass();
    TDefaultTransmissivity::initClass();


    splash->showMessage(QObject::tr("Setting up the main window..."), topRight, Qt::black);


    QDir pluginsDirectory(qApp->applicationDirPath() );
    pluginsDirectory.cd("plugins");
    PluginManager pluginManager;
    pluginManager.LoadAvailablePlugins(pluginsDirectory);

    int exit;
    if (argc > 1)
    {
        QString tonatiuhFile = argv[1];

        QFileInfo fileInfo(tonatiuhFile);
        if (fileInfo.completeSuffix() == QLatin1String("tnhs") )
        {

            QString fileName(argv[1]);
            QFileInfo fileInfo(fileName);

            QDir testDirectory(fileInfo.absolutePath() );
            testDirectory.cd(".");

            QScriptEngine* interpreter = new QScriptEngine;
            qScriptRegisterSequenceMetaType<QVector<QVariant> >(interpreter);


            MainWindow* mw = new MainWindow(QLatin1String("") );
            mw->SetPluginManager(&pluginManager);
            QScriptValue tonatiuh = interpreter->newQObject(mw);
            interpreter->globalObject().setProperty("tonatiuh", tonatiuh);


            QFile scriptFile(fileName);
            if (!scriptFile.open(QIODevice::ReadOnly) )
            {
                QString errorMessage = QString("Cannot open file %1.").arg(fileName);
                std::cerr << errorMessage.toStdString() << std::endl;
            }


            QTextStream in(&scriptFile);
            QString program = in.readAll();
            scriptFile.close();


            QScriptSyntaxCheckResult checkResult = interpreter->checkSyntax(program);
            if (checkResult.state() != QScriptSyntaxCheckResult::Valid)
            {
                QString errorMessage = QString("Script Syntaxis Error.\n"
                                               "Line: %1. %2").arg(QString::number(checkResult.errorLineNumber() ), checkResult.errorMessage () );
                std::cerr << errorMessage.toStdString() << std::endl;
                return -1;
            }

            QScriptValue result = interpreter->evaluate(program);
            if (result.isError () )
            {
                QScriptValue lineNumber = result.property("lineNumber");

                QString errorMessage = QString("Script Execution Error.\nLine %1. %2").arg(QString::number(lineNumber.toNumber() ), result.toString() );
                std::cerr << errorMessage.toStdString() << std::endl;
                return -1;

            }

            delete mw;
            delete interpreter;
            exit = 0;
        }
        else
        {

            MainWindow* mw = new MainWindow(tonatiuhFile);
            mw->SetPluginManager(&pluginManager);


            mw->show();
            splash->finish(mw);
            delete splash;
            exit = a.exec();
            delete mw;
        }
    }
    else
    {
        MainWindow* mw = new MainWindow("");
        mw->SetPluginManager(&pluginManager);
        mw->show();
        splash->finish(mw);
        delete splash;
        exit = a.exec();
        delete mw;

    }

    return exit;
}
