TARGET = Tonatiuh
DESTDIR = ..
include(../config.pri)

QT += concurrent # for multithreading
QT += script # for scripting
QT += printsupport # for customplot
QT += gui # QVector3D

LIBS += -lTonatiuh-Kernel -lTonatiuh-Libraries -lOpengl32

HEADERS += \
    calculator/CelestialWidget.h \
    calculator/HorizontalWidget.h \
    calculator/Image.h \
    calculator/SunCalculatorDialog.h \
    calculator/WorldMap.h \
    commands/ActionInsert.h \
    commands/CmdAirModified.h \
    commands/CmdCopy.h \
    commands/CmdCut.h \
    commands/CmdDelete.h \
    commands/CmdDeleteTracker.h \
    commands/CmdInsertNode.h \
    commands/CmdInsertShape.h \
    commands/CmdInsertSurface.h \
    commands/CmdInsertTracker.h \
    commands/CmdLightPositionModified.h \
    commands/CmdParameterModified.h \
    commands/CmdPaste.h \
    commands/CmdRename.h \
    commands/CmdSunModified.h \
    commands/tgc.h \
    run/FluxAnalysis.h \
    run/FluxAnalysisDialog.h \
    run/RayTracingDialog.h \
    run/SelectSurfaceDialog.h \
    main/Document.h \
    main/MainWindow.h \
    main/PluginManager.h \
    main/UndoView.h \
    parameters/ParametersDelegate.h \
    parameters/ParametersEditor.h \
    parameters/ParametersItem.h \
    parameters/ParametersModel.h \
    parameters/ParametersTabs.h \
    parameters/ParametersView.h \
    script/CodeEditArea.h \
    script/CodeEditorWidget.h \
    script/LineNumberWidget.h \
    script/NodeObject.h \
    script/ScriptEditorDialog.h \
    script/ScriptRayTracer.h \
    script/tonatiuh_script.h \
    tree/SceneDelegate.h \
    tree/SceneFilter.h \
    tree/SceneModel.h \
    tree/SceneView.h \
    tree/SoPathVariant.h \
    view/ExaminerViewer.h \
    view/ExaminerViewer2.h \
    view/GraphicRoot.h \
    view/GraphicView.h \
    view/GridNode3D.h \
    view/SkyNode3D.h \
    view/SunNode3D.h \
    widgets/AboutDialog.h \
    widgets/AirDialog.h \
    widgets/GridDialog.h \
    widgets/NetworkConnectionsDialog.h \
    widgets/SunDialog.h

SOURCES += \
    calculator/CelestialWidget.cpp \
    calculator/HorizontalWidget.cpp \
    calculator/Image.cpp \
    calculator/SunCalculatorDialog.cpp \
    calculator/WorldMap.cpp \
    commands/ActionInsert.cpp \
    commands/CmdAirModified.cpp \
    commands/CmdCopy.cpp \
    commands/CmdCut.cpp \
    commands/CmdDelete.cpp \
    commands/CmdDeleteTracker.cpp \
    commands/CmdInsertNode.cpp \
    commands/CmdInsertShape.cpp \
    commands/CmdInsertSurface.cpp \
    commands/CmdInsertTracker.cpp \
    commands/CmdLightPositionModified.cpp \
    commands/CmdParameterModified.cpp \
    commands/CmdPaste.cpp \
    commands/CmdRename.cpp \
    commands/CmdSunModified.cpp \
    run/FluxAnalysis.cpp \
    run/FluxAnalysisDialog.cpp \
    run/RayTracingDialog.cpp \
    run/SelectSurfaceDialog.cpp \
    main/Document.cpp \
    main/main.cpp \
    main/MainWindow.cpp \
    main/PluginManager.cpp \
    main/UndoView.cpp \
    parameters/ParametersDelegate.cpp \
    parameters/ParametersEditor.cpp \
    parameters/ParametersItem.cpp \
    parameters/ParametersModel.cpp \
    parameters/ParametersTabs.cpp \
    parameters/ParametersView.cpp \
    script/CodeEditArea.cpp \
    script/CodeEditorWidget.cpp \
    script/LineNumberWidget.cpp \
    script/NodeObject.cpp \
    script/ScriptEditorDialog.cpp \
    script/ScriptRayTracer.cpp \
    script/tonatiuh_script.cpp \
    tree/SceneDelegate.cpp \
    tree/SceneFilter.cpp \
    tree/SceneModel.cpp \
    tree/SceneView.cpp \
    tree/SoPathVariant.cpp \
    view/ExaminerViewer.cpp \
    view/ExaminerViewer2.cpp \
    view/GraphicRoot.cpp \
    view/GraphicView.cpp \
    view/GridNode3D.cpp \
    view/SkyNode3D.cpp \
    view/SunNode3D.cpp \
    widgets/AboutDialog.cpp \
    widgets/AirDialog.cpp \
    widgets/GridDialog.cpp \
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
    script/CodeEditorWidget.ui \
    script/ScriptEditorDialog.ui \
    widgets/AboutDialog.ui \
    widgets/AirDialog.ui \
    widgets/GridDialog.ui \
    widgets/NetworkConnectionsDialog.ui \
    widgets/SunDialog.ui


RESOURCES += resources.qrc

win32 { # icons for windows explorer
    RC_ICONS += images/about/Tonatiuh.ico
    RC_ICONS += images/about/TonatiuhCy.ico
}

#DISTFILES += \
#    ../../examples/CPC.tnh
