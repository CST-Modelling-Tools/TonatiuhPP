#include <QApplication>
#include <QFileInfo>
#include <QSplashScreen>
#include <QScriptEngine>
#include <QTextStream>
#include <QStyleFactory>
#include <QDebug>

#include <Inventor/Qt/SoQt.h>

#include "MainWindow.h"

QTextStream cerr(stderr);

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

int main(int argc, char** argv)
{
//    QApplication::setColorSpec(QApplication::CustomColor);
//    QApplication::setAttribute(Qt::AA_DisableHighDpiScaling);

    QApplication a(argc, argv);
    a.setApplicationVersion(APP_VERSION);

//    qDebug() << QStyleFactory::keys();
//    a.setStyle(QStyleFactory::create("Fusion"));

    QString fileName = argc > 1 ? argv[1] : "";
    QFileInfo fileInfo(fileName);

    if (fileInfo.completeSuffix() != "tnhs")
    {
#ifdef CYE
        QPixmap pixmap(":/images/about/SplashScreenCy.png");
#else
        QPixmap pixmap(":/images/about/SplashScreen.png");
        pixmap = pixmap.scaledToHeight(256, Qt::SmoothTransformation);
#endif
        QSplashScreen splash(pixmap);
        int splashAlignment = Qt::AlignLeft | Qt::AlignBottom;
        splash.show();

        splash.showMessage("Launching Coin3D", splashAlignment);
        SoQt::init((QWidget*) NULL);

        splash.showMessage("Creating window", splashAlignment);
        MainWindow mw(fileName, &splash);
        mw.show();
        splash.finish(&mw);

        return a.exec();
    }
    else
    {
        SoQt::init((QWidget*) NULL);

        QScriptEngine* interpreter = new QScriptEngine;
        qScriptRegisterSequenceMetaType<QVector<QVariant> >(interpreter);

        MainWindow* mw = new MainWindow("");
//        mw->SetPluginManager(&pluginManager);
        QScriptValue tonatiuh = interpreter->newQObject(mw);
        interpreter->globalObject().setProperty("tonatiuh", tonatiuh);

        QFile scriptFile(fileName);
        if (!scriptFile.open(QIODevice::ReadOnly))
        {
            QString text = QString("Cannot open file %1.").arg(fileName);
            cerr << text << endl;
        }
        QTextStream in(&scriptFile);
        QString program = in.readAll();
        scriptFile.close();

        QScriptSyntaxCheckResult check = interpreter->checkSyntax(program);
        if (check.state() != QScriptSyntaxCheckResult::Valid)
        {
            QString text = QString("Script Syntaxis Error.\nLine: %1. %2").arg(
                QString::number(check.errorLineNumber()),
                check.errorMessage()
            );
            cerr << text << endl;
            return -1;
        }

        QScriptValue result = interpreter->evaluate(program);
        if (result.isError())
        {
            QScriptValue lineNumber = result.property("lineNumber");
            QString text = QString("Script Execution Error.\nLine %1. %2").arg(
                QString::number(lineNumber.toNumber()),
                result.toString()
            );
            cerr << text << endl;
            return -1;
        }

        delete mw;
        delete interpreter;
        return 0;
    }
}
