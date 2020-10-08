TARGET = Tonatiuh-Application
DESTDIR = ..
include(../config.pri)

QT += concurrent # for multithreading
QT += script # for scripting
QT += printsupport # for customplot
QT += gui # QVector3D
#QT += opengl

LIBS += -lTonatiuh-Kernel -lTonatiuh-Libraries -lOpengl32

HEADERS += \
    calculator/CelestialWidget.h \
    calculator/HorizontalWidget.h \
    calculator/Image.h \
    calculator/SunCalculatorDialog.h \
    calculator/WorldMap.h \
    commands/ActionInsert.h \
    commands/CmdCopy.h \
    commands/CmdCut.h \
    commands/CmdDelete.h \
    commands/CmdInsertNode.h \
    commands/CmdPaste.h \
    commands/CmdRename.h \
    commands/CmdSetFieldNode.h \
    commands/CmdSetFieldText.h \
    main/CustomSplashScreen.h \
    main/Document.h \
    main/MainWindow.h \
    main/PluginManager.h \
    main/UndoView.h \
    parameters/ComboBoxDelegate.h \
    parameters/ParametersDelegate.h \
    parameters/ParametersEditor.h \
    parameters/ParametersItemField.h \
    parameters/ParametersItemNode.h \
    parameters/ParametersModel.h \
    parameters/ParametersTabs.h \
    parameters/ParametersView.h \
    run/FluxAnalysis.h \
    run/FluxAnalysisDialog.h \
    run/RayTracingDialog.h \
    run/SelectSurfaceDialog.h \
    script/AboutScriptDialog.h \
    script/CodeEditor.h \
    script/FileObject.h \
    script/LineNumbers.h \
    script/NodeObject.h \
    script/ScriptRayTracer.h \
    script/ScriptWindow.h \
    script/SyntaxHighlighter.h \
    script/tonatiuh_script.h \
    tree/SceneDelegate.h \
    tree/SceneFilter.h \
    tree/SceneTreeModel.h \
    tree/SceneTreeView.h \
    tree/SoPathVariant.h \
    view/GraphicRoot.h \
    view/GraphicView.h \
    view/GridNode3D.h \
    view/KeyFilter.h \
    view/KeyboardHook.h \
    view/MenuStyle.h \
    view/OverlayNode.h \
    view/SkyNode3D.h \
    view/SunNode3D.h \
    view/trf.h \
    widgets/AboutDialog.h \
    widgets/AirDialog.h \
    widgets/NetworkConnectionsDialog.h \
    widgets/SunDialog.h

SOURCES += \
    calculator/CelestialWidget.cpp \
    calculator/HorizontalWidget.cpp \
    calculator/Image.cpp \
    calculator/SunCalculatorDialog.cpp \
    calculator/WorldMap.cpp \
    commands/ActionInsert.cpp \
    commands/CmdCopy.cpp \
    commands/CmdCut.cpp \
    commands/CmdDelete.cpp \
    commands/CmdInsertNode.cpp \
    commands/CmdPaste.cpp \
    commands/CmdRename.cpp \
    commands/CmdSetFieldNode.cpp \
    commands/CmdSetFieldText.cpp \
    main/CustomSplashScreen.cpp \
    main/Document.cpp \
    main/MainWindow.cpp \
    main/PluginManager.cpp \
    main/UndoView.cpp \
    main/main.cpp \
    parameters/ComboBoxDelegate.cpp \
    parameters/ParametersDelegate.cpp \
    parameters/ParametersEditor.cpp \
    parameters/ParametersItemField.cpp \
    parameters/ParametersItemNode.cpp \
    parameters/ParametersModel.cpp \
    parameters/ParametersTabs.cpp \
    parameters/ParametersView.cpp \
    run/FluxAnalysis.cpp \
    run/FluxAnalysisDialog.cpp \
    run/RayTracingDialog.cpp \
    run/SelectSurfaceDialog.cpp \
    script/AboutScriptDialog.cpp \
    script/CodeEditor.cpp \
    script/FileObject.cpp \
    script/LineNumbers.cpp \
    script/NodeObject.cpp \
    script/ScriptRayTracer.cpp \
    script/ScriptWindow.cpp \
    script/SyntaxHighlighter.cpp \
    script/tonatiuh_script.cpp \
    tree/SceneDelegate.cpp \
    tree/SceneFilter.cpp \
    tree/SceneTreeModel.cpp \
    tree/SceneTreeView.cpp \
    tree/SoPathVariant.cpp \
    view/GraphicRoot.cpp \
    view/GraphicView.cpp \
    view/GridNode3D.cpp \
    view/KeyFilter.cpp \
    view/KeyboardHook.cpp \
    view/MenuStyle.cpp \
    view/OverlayNode.cpp \
    view/SkyNode3D.cpp \
    view/SunNode3D.cpp \
    view/trf.cpp \
    widgets/AboutDialog.cpp \
    widgets/AirDialog.cpp \
    widgets/NetworkConnectionsDialog.cpp \
    widgets/SunDialog.cpp

FORMS += \
    calculator/SunCalculatorDialog.ui \
    run/FluxAnalysisDialog.ui \
    run/RayTracingDialog.ui \
    run/SelectSurfaceDialog.ui \
    main/ContainerViewer.ui \
    main/MainWindow.ui \
    main/UndoView.ui \
    parameters/ParametersEditor.ui \
    script/AboutScriptDialog.ui \
    script/ScriptWindow.ui \
    widgets/AboutDialog.ui \
    widgets/AirDialog.ui \
    widgets/NetworkConnectionsDialog.ui \
    widgets/SunDialog.ui


RESOURCES += resources.qrc

win32 { # icons for windows
    RC_ICONS += images/about/TonatiuhCy.ico
    RC_ICONS += images/about/Tonatiuh.ico
    RC_ICONS += images/about/tnh.ico
    RC_ICONS += images/about/tnhs.ico
}

#DISTFILES += \
#    ../../examples/CPC.tnh
