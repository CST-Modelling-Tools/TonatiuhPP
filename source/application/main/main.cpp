#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QSplashScreen>

#include <iostream>

#include <Inventor/Qt/SoQt.h>

#include "MainWindow.h"

#include <QScriptEngine>
#include <QTextStream>
#include "PluginManager.h"
#include "script/ScriptRayTracer.h"
#include "script/tonatiuh_script.h"

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

//Q_DECLARE_METATYPE(QVector<QVariant>)

#include <QStyleFactory>
#include <QDebug>




int main(int argc, char** argv)
{
//    QApplication::setColorSpec(QApplication::CustomColor);

//    QApplication::setAttribute(Qt::AA_DisableHighDpiScaling);

    QApplication a(argc, argv);
    a.setApplicationVersion(APP_VERSION);

//    qDebug() << QStyleFactory::keys();
//    a.setStyle(QStyleFactory::create("Fusion"));

    QSplashScreen* splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/images/about/SplashScreen.png") );
    splash->show();
    int splashAlignment = Qt::AlignLeft | Qt::AlignBottom;

    splash->showMessage("Loading Coin3D...", splashAlignment);
    SoQt::init( (QWidget*) NULL);


    splash->showMessage("Loading plugins...", splashAlignment);

    QDir pluginsDirectory(qApp->applicationDirPath() );
    pluginsDirectory.cd("plugins");
    PluginManager pluginManager;
    pluginManager.load(pluginsDirectory);

    splash->showMessage("Opening scene...", splashAlignment);
    int exit;
    if (argc > 1)
    {
        QString tonatiuhFile = argv[1];

        QFileInfo fileInfo(tonatiuhFile);
        if (fileInfo.completeSuffix() == "tnhs")
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
