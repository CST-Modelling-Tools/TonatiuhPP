#include <QApplication>
#include <QStyleFactory>
#include <QCommandLineParser>
#include <QSettings>
#include "CustomSplashScreen.h"

#include <QFileInfo>
#include <QScriptEngine>
#include <QTextStream>

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
 */

#include "script/NodeObject.h"
#include "script/DataObject.h"
Q_SCRIPT_DECLARE_QMETAOBJECT(NodeObject, QObject*)
Q_SCRIPT_DECLARE_QMETAOBJECT(DataObject, QObject*)

int main(int argc, char** argv)
{  
    // application
    QApplication app(argc, argv);
    app.setApplicationName("Tonatiuh");
    app.setApplicationVersion(APP_VERSION);

    //    QApplication::setColorSpec(QApplication::CustomColor);
    //    QApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
    //    qDebug() << QStyleFactory::keys();
    //    app.setStyle(QStyleFactory::create("Fusion"));

    // parser
    QCommandLineParser parser;
    parser.setApplicationDescription("Description");
    parser.addHelpOption(); // -h
    parser.addVersionOption(); // -v

    // options
    QCommandLineOption optionInput( // -i=input.xml
        "i", "File with input parameters", // option name and description
        "file", "" // value type and default
    );
    parser.addOption(optionInput);

    QCommandLineOption optionTest( // -t
        "t", "Test mode"
    );
    parser.addOption(optionTest);

    QCommandLineOption optionWindow( // -w
        "w", "Window mode"
    );
    parser.addOption(optionWindow);

    // processing
    parser.process(app);
//    bool isTest = parser.isSet(optionTest);

    QSettings settings("Tonatiuh", "Cyprus");
    QString theme = settings.value("theme", "").toString();

    QString fileIcon;
    QString filePixmap;
    if (theme == "") {
        fileIcon = ":/images/about/Tonatiuh.ico";
        filePixmap = ":/images/about/SplashScreen.png";
    } else {
        fileIcon = ":/images/about/TonatiuhCy.ico";
        filePixmap = ":/images/about/SplashScreenCy.png";
    }
    app.setWindowIcon(QIcon(fileIcon));

//    QString fileName = parser.positionalArguments()[0];
    QString fileName = parser.value(optionInput);
    QFileInfo fileInfo(fileName);
    if (fileInfo.completeSuffix() != "tnhpps" || parser.isSet(optionWindow))
    {
        QPixmap pixmap(filePixmap);
        CustomSplashScreen splash(pixmap);
        splash.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        splash.show();

        splash.setMessage("Launching Coin3D");
        SoQt::init((QWidget*) NULL);

        splash.setMessage("Creating window");

        MainWindow mw(fileName, &splash);
        mw.show();
        splash.setFinishWindow();
        if (fileInfo.completeSuffix() == "tnhpps")
            mw.openFileScript(fileName);

        int code = app.exec();
        return code;
    }
    else
    {
        SoQt::init((QWidget*) 0);

        QScriptEngine* engine = new QScriptEngine;
        qScriptRegisterSequenceMetaType<QVector<QVariant>>(engine);

        MainWindow mw;
        QScriptValue tonatiuh = engine->newQObject(&mw);
        engine->globalObject().setProperty("tonatiuh", tonatiuh);
        engine->globalObject().setProperty("tn", tonatiuh);

        NodeObject::setMainWindow(&mw);
        NodeObject::setEngine(engine);
        QScriptValue nodeObjectClass = engine->scriptValueFromQMetaObject<NodeObject>();
        engine->globalObject().setProperty("NodeObject", nodeObjectClass);

        QScriptValue fileObjectClass = engine->scriptValueFromQMetaObject<DataObject>();
        engine->globalObject().setProperty("DataObject", fileObjectClass);

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))
        {
            QString text = QString("Cannot open file %1.").arg(fileName);
            cerr << text << endl;
        }
        QTextStream in(&file);
        QString program = in.readAll();
        file.close();

        QScriptSyntaxCheckResult check = engine->checkSyntax(program);
        if (check.state() != QScriptSyntaxCheckResult::Valid)
        {
            QString text = QString("Syntax error in line %1.\n%2")
                .arg(check.errorLineNumber())
                .arg(check.errorMessage());
            cerr << text << endl;
            return -1;
        }

        QScriptValue result = engine->evaluate(program);
        if (result.isError())
        {
            QString text = QString("Runtime error.\nLine %1. %2")
                .arg(result.property("lineNumber").toNumber())
                .arg(result.toString());
            cerr << text << endl;
            return -1;
        }

        delete engine;
        return 0;
    }
}
