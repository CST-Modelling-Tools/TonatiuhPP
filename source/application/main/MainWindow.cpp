#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <iostream>

#include <QCloseEvent>
#include <QDir>
#include <QFileDialog>
#include <QFuture>
#include <QFutureWatcher>
#include <QMessageBox>
#include <QMutex>
#include <QPluginLoader>
#include <QProgressDialog>
#include <QSettings>
#include <QtConcurrentMap>
#include <QTime>
#include <QUndoStack>
#include <QUndoView>
#include <QSplashScreen>
#include <QElapsedTimer>
#include <QPushButton>

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/draggers/SoDragger.h>
#include <Inventor/manips/SoCenterballManip.h>
#include <Inventor/manips/SoHandleBoxManip.h>
#include <Inventor/manips/SoJackManip.h>
#include <Inventor/manips/SoTabBoxManip.h>
#include <Inventor/manips/SoTrackballManip.h>
#include <Inventor/manips/SoTransformBoxManip.h>
#include <Inventor/manips/SoTransformerManip.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodekits/SoSceneKit.h>

#include "libraries/math/gcf.h"
#include "libraries/math/gcf.h"
#include "libraries/math/3D/Ray.h"
#include "libraries/math/3D/Transform.h"

#include "commands/ActionInsert.h"
#include "commands/CmdChangeNodeName.h"
#include "commands/CmdCopy.h"
#include "commands/CmdCut.h"
#include "commands/CmdDelete.h"
#include "commands/CmdDeleteTracker.h"
#include "commands/CmdInsertNode.h"
#include "commands/CmdInsertSurface.h"
#include "commands/CmdInsertShape.h"
#include "commands/CmdInsertTracker.h"
#include "commands/CmdSunModified.h"
#include "commands/CmdLightPositionModified.h"
#include "commands/CmdModifyParameter.h"
#include "commands/CmdPaste.h"
#include "commands/CmdAirModified.h"

#include "PluginManager.h"
#include "calculator/SunCalculatorDialog.h"
#include "kernel/TonatiuhFunctions.h"
#include "kernel/air/Air.h"
#include "kernel/air/AirVacuum.h"
#include "kernel/component/ComponentFactory.h"
#include "kernel/material/MaterialRT.h"
#include "kernel/photons/PhotonsBuffer.h"
#include "kernel/photons/PhotonsAbstract.h"
#include "kernel/photons/PhotonsSettings.h"
#include "kernel/random/Random.h"
#include "kernel/run/InstanceNode.h"
#include "kernel/run/RayTracer.h"
#include "kernel/profiles/ProfileRT.h"
#include "kernel/scene/TSceneKit.h"
#include "kernel/scene/TSeparatorKit.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/ShapeRT.h"
#include "kernel/sun/SunAperture.h"
#include "kernel/sun/SunShape.h"
#include "kernel/sun/SunKit.h"
#include "kernel/trackers/Tracker.h"
#include "kernel/trf.h"
#include "main/Document.h"
#include "run/FluxAnalysis.h"
#include "run/FluxAnalysisDialog.h"
#include "run/RayTracingDialog.h"
#include "script/ScriptEditorDialog.h"
#include "tree/SceneModel.h"
#include "view/GraphicRoot.h"
#include "view/GraphicView.h"
#include "widgets/AboutDialog.h"
#include "widgets/AirDialog.h"
#include "widgets/GridDialog.h"
#include "widgets/NetworkConnectionsDialog.h"
#include "widgets/SunDialog.h"
#include "UndoView.h"
#include <QDebug>

/*!
 * Returns the \a fullFileName files name, without path.
 */
QString StrippedName(const QString& fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void startManipulator(void* data, SoDragger* dragger)
{
    MainWindow* w = static_cast<MainWindow*>(data);
    w->StartManipulation(dragger);
}

void finishManipulator(void* data, SoDragger* /*dragger*/)
{
    MainWindow* w = static_cast<MainWindow*>(data);
    w->FinishManipulation();
}

/*!
 * Creates a new MainWindow object.
 */
MainWindow::MainWindow(QString tonatiuhFile, QSplashScreen* splash, QWidget* parent, Qt::WindowFlags flags):
    QMainWindow(parent, flags),
    ui(new Ui::MainWindow),
    m_recentFiles(""),
    m_currentFile(""),
    m_undoStack(0),
    m_undoView(0),
    m_document(0),
    m_graphicsRoot(0),
    m_modelScene(0),
    m_modelSelection(0),
    m_lastExportFileName(""),
    m_lastExportSurfaceUrl(""),
    m_lastExportInGlobal(true),
    m_coinNode_Buffer(0),
    m_manipulators_Buffer(0),

    m_raysNumber(10'000),
    m_raysScreen(1'000),
    m_raysRandomFactoryIndex(0),
    m_raysGridWidth(200),
    m_raysGridHeight(200),

    m_raysTracedTotal(0),
    m_rand(0),

    m_photonsBuffer(0),
    m_photonBufferSize(1'000'000),
    m_photonBufferAppend(false),
    m_photonsSettings(0),

    m_graphicView(0),
    m_focusView(0)
{
    ui->setupUi(this);

    int splashAlignment = Qt::AlignLeft | Qt::AlignBottom;

    if (splash) splash->showMessage("Loading plugins", splashAlignment);
    m_pluginManager = new PluginManager;
    QDir dir(qApp->applicationDirPath());
    dir.cd("plugins");
    m_pluginManager->load(dir);

    if (splash) splash->showMessage("Creating views", splashAlignment);
    SetupActions();
    SetupDocument();
    SetupViews();
    SetupPluginsManager();
    SetupTriggers();

    ReadSettings();


    qDebug() << "counter " << m_document->getSceneKit()->getRefCount();
    if (splash) splash->showMessage("Opening file", splashAlignment);
//    qDebug() << "layout " << m_modelScene->getInstance(m_modelScene->indexFromUrl("//Layout"))->getNode()->getRefCount();
    qDebug() << "counter " << m_document->getSceneKit()->getRefCount();

    if (!tonatiuhFile.isEmpty()) {
        StartOver(tonatiuhFile);
    } else {
        SetCurrentFile("");
//        ui->actionViewGrid->trigger();
//        ui->actionViewGrid->setChecked(true);
        ShowGrid();
        SoCamera* camera = m_graphicView[0]->getCamera();
        camera->focalDistance = 10.;
        SbVec3f target(0., 0., 0.);
        camera->position = target + SbVec3f(0, 0, camera->focalDistance.getValue());
        camera->pointAt(target, SbVec3f(0., 1., 0.));
    }

    qDebug() << "counter " << m_document->getSceneKit()->getRefCount();
    qDebug() << "layout " << m_modelScene->getInstance(m_modelScene->indexFromUrl("//Layout"))->getNode()->getRefCount();

    Select("//Layout");

    qDebug() << "layout " << m_modelScene->getInstance(m_modelScene->indexFromUrl("//Layout"))->getNode()->getRefCount();

//    m_graphicsRoot->deselectAll();
    qDebug() << "counter " << m_document->getSceneKit()->getRefCount();
    qDebug() << "layout " << m_modelScene->getInstance(m_modelScene->indexFromUrl("//Layout"))->getNode()->getRefCount();

    ui->toolbarFile->hide();
    ui->toolbarEdit->hide();

    setStyleSheet(R"(
QAbstractItemView {
outline: 0;
}

QAbstractItemView::item:selected {
color: black;
background-color: #c8dbe5;
}

QAbstractItemView::item:hover:selected {
background-color: #c8dbe5;
}

QAbstractItemView::item:hover:!selected {
background-color: #eeeeee;
}

QHeaderView::section
{
background-color: #d2dddb;
border-width: 0 1 1 0;
border-style: solid;
border-color: #abbaba;
padding-left: 8;
}

QHeaderView::section:last, QHeaderView::section:only-one
{
border-width: 0 0 1 0;
}

    )");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_undoView;
    delete m_pluginManager;
    delete m_modelScene;
    delete m_document;
    delete m_undoStack;
    delete m_rand;
    delete m_photonsBuffer;
}

/*!
 * Creates actions form recent files.
 */
void MainWindow::SetupActions()
{
    for (int i = 0; i < m_maxRecentFiles; ++i) {
        QAction* a = new QAction;
        a->setVisible(false);
        connect(
            a, SIGNAL(triggered()),
            this, SLOT(FileOpenRecent())
        );
        m_recentFileActions << a;
    }
    ui->menuFileRecent->addActions(m_recentFileActions);
}

/*!
 * Initializes tonatiuh document object.
 */
void MainWindow::SetupDocument()
{
    m_document = new Document;
    connect(
        m_document, SIGNAL(Warning(QString)),
        this, SLOT(ShowWarning(QString))
    );

    // graphic root
    m_graphicsRoot = new GraphicRoot;
    m_graphicsRoot->setDocument(m_document);
    m_graphicsRoot->grid()->addChild(CreateGrid());

    connect(
        m_graphicsRoot, SIGNAL(selectionChanged(SoSelection*)),
        this, SLOT(SelectionFinish(SoSelection*))
    );

    // models
    m_modelScene = new SceneModel;
    m_modelScene->setDocument(m_document);

    m_modelSelection = new QItemSelectionModel(m_modelScene);
}

/*!
 * Starts MainWindow views.
 */
void MainWindow::SetupViews()
{
    SetupUndoView();
    SetupGraphicView();
    SetupTreeView();
    SetupParametersView();
}

/*!
 * Creates a view for visualize user done last actions.
 */
void MainWindow::SetupUndoView()
{
    m_undoStack = new QUndoStack(this);
    m_undoStack->setUndoLimit(10);

    m_undoView = new UndoView(m_undoStack);
    m_undoView->setWindowTitle("Undo List");
    m_undoView->setAttribute(Qt::WA_QuitOnClose, false);
    int q = fontMetrics().height();
    m_undoView->resize(24*q, 16*q);

    connect(m_undoStack, SIGNAL(canRedoChanged(bool)),
            ui->actionEditRedo, SLOT(setEnabled(bool)) );
    connect(m_undoStack, SIGNAL(canUndoChanged(bool)),
            ui->actionEditUndo, SLOT(setEnabled(bool)) );
}

/*!
 * Initializates graphic view.
 */
void MainWindow::SetupGraphicView()
{
    QSplitter* splitter = findChild<QSplitter*>("horizontalSplitter");

    QSplitter* splitterV = new QSplitter();
    splitterV->setObjectName("graphicSplitterV");
    splitterV->setOrientation(Qt::Vertical);
    splitter->insertWidget(0, splitterV);

    QList<int> sizes;
    sizes << 500 << 200;
    splitter->setSizes(sizes);

    QSplitter* splitterH1 = new QSplitter(splitterV);
    splitterH1->setObjectName("graphicSplitterH1");
    splitterH1->setOrientation(Qt::Horizontal);

    QSplitter* splitterH2 = new QSplitter(splitterV);
    splitterH2->setObjectName("graphicSplitterH2");
    splitterH2->setOrientation(Qt::Horizontal);

    m_graphicView << new GraphicView(splitterH1);
    m_graphicView << new GraphicView(splitterH1);
    m_graphicView << new GraphicView(splitterH2);
    m_graphicView << new GraphicView(splitterH2);

    for (int n = 0; n < 4; n++){
        m_graphicView[n]->setSceneGraph(m_graphicsRoot);
        m_graphicView[n]->setModel(m_modelScene);
        m_graphicView[n]->setSelectionModel(m_modelSelection);
        if (n > 0) m_graphicView[n]->showDecoration(false);
    }

    m_focusView = 1;
    on_actionLookNorth_triggered();

    m_focusView = 2;
    on_actionLookEast_triggered();

    m_focusView = 3;
    on_actionLookWest_triggered();

    // 0 splitterH1 1              tree
    //   splitterV      splitter
    // 2 spliter H2 3              parameters
    on_actionQuadView_toggled();
}

/*!
 * Initializates Tonatiuh objectes tree view.
 */
void MainWindow::SetupTreeView()
{
    ui->sceneView->setModel(m_modelScene);
    ui->sceneView->setSelectionModel(m_modelSelection);
    ui->sceneView->setRootIndex(m_modelScene->indexFromUrl(""));

    connect(ui->sceneView, SIGNAL(dragAndDrop(const QModelIndex&,const QModelIndex&)),
            this, SLOT(ItemDragAndDrop(const QModelIndex&,const QModelIndex&)) );
    connect(ui->sceneView, SIGNAL(dragAndDropCopy(const QModelIndex&,const QModelIndex&)),
            this, SLOT(ItemDragAndDropCopy(const QModelIndex&,const QModelIndex&)) );
    connect(ui->sceneView, SIGNAL(showMenu(const QModelIndex&)),
            this, SLOT(ShowMenu(const QModelIndex&)) );
    connect(ui->sceneView, SIGNAL(nodeNameModificated(const QModelIndex&,const QString&)),
            this, SLOT(ChangeNodeName(const QModelIndex&,const QString&)) );
}

/*!
 * Initializes the tonatiuh objects parameters view.
 */
void MainWindow::SetupParametersView()
{
    connect(
        ui->parametersTabs, SIGNAL(valueModified(SoNode*, QString, QString)),
        this, SLOT(SetParameterValue(SoNode*, QString, QString))
    );

    connect(
        m_modelSelection, SIGNAL(currentChanged(const QModelIndex&,const QModelIndex&)),
        this, SLOT(ChangeSelection(const QModelIndex&))
    );
}

/*!
 * Initializes plugin manager and load available plugins.
 */
void MainWindow::SetupPluginsManager()
{
    if (!m_pluginManager) return;
    SetupActionsInsertComponent();
//    addToolBarBreak();
    SetupActionsInsertShape();
    SetupActionsInsertMaterial();
    SetupActionsInsertTracker();
}

/*!
 * Defines slots function for main window signals.
 */
void MainWindow::SetupTriggers()
{
    connect(this, SIGNAL(Abort(QString)), this, SLOT(onAbort(QString)));

    // file
    connect(ui->actionFileNew, SIGNAL(triggered()), this, SLOT(FileNew()) );
    connect(ui->actionFileOpen, SIGNAL(triggered()), this, SLOT(FileOpen()) );
    connect(ui->actionFileSave, SIGNAL(triggered()), this, SLOT(FileSave()) );
    connect(ui->actionFileSaveAs, SIGNAL(triggered()), this, SLOT(FileSaveAs()) );
    connect(ui->actionFileExit, SIGNAL(triggered()), this, SLOT(close()) );

    // edit
    connect(ui->actionEditUndo, SIGNAL(triggered()), this, SLOT(Undo()) );
    connect(ui->actionEditRedo, SIGNAL(triggered()), this, SLOT(Redo()) );
    connect(ui->actionEditUndoView, SIGNAL(triggered()), this, SLOT(ShowCommandView()) );
    connect(ui->actionEditCut, SIGNAL(triggered()), this, SLOT(Cut()) );
    connect(ui->actionEditCopy, SIGNAL(triggered()), this, SLOT(Copy()) );
    connect(ui->actionEditPaste, SIGNAL(triggered()), this, SLOT(PasteCopy()) );
    connect(ui->actionEditPasteLink, SIGNAL(triggered()), this, SLOT(PasteLink()) );
    connect(ui->actionEditDelete, SIGNAL(triggered()), this, SLOT(Delete()) );

    // insert
    connect(ui->actionInsertNode, SIGNAL(triggered()), this, SLOT(InsertNode()) );
    connect(ui->actionInsertShape, SIGNAL(triggered()), this, SLOT(InsertShape()) );
    connect(ui->actionSaveComponent, SIGNAL(triggered()), this, SLOT(SaveComponent()) );
    connect(ui->actionUserComponent, SIGNAL(triggered()), this, SLOT(InsertUserDefinedComponent()) );

    // scene
    connect(ui->actionSceneSun, SIGNAL(triggered()), this, SLOT(onSunDialog()) );
    connect(ui->actionSceneSunCalculator, SIGNAL(triggered()), this, SLOT(CalculateSunPosition()) );
    connect(ui->actionSceneAir, SIGNAL(triggered()), this, SLOT(onAirDialog()) );
    connect(ui->actionSceneGrid, SIGNAL(triggered()), this, SLOT(ChangeGridSettings())  );

    // run
    connect(ui->actionRun, SIGNAL(triggered()), this, SLOT(RunCompleteRayTracer()) );
    connect(ui->actionRunFlux, SIGNAL(triggered()), this, SLOT(RunFluxAnalysisDialog()) );

    // view
    connect(ui->actionViewGrid, SIGNAL(triggered()), this, SLOT(ShowGrid()));
    connect(ui->actionViewRays, SIGNAL(toggled(bool)), this, SLOT(ShowRays(bool)));
    connect(ui->actionViewPhotons, SIGNAL(toggled(bool)), this, SLOT(ShowPhotons(bool)));
}

/*!
 * Finish the manipulation of the current selected node.
 */
void MainWindow::FinishManipulation()
{
    QModelIndex currentIndex = ui->sceneView->currentIndex();
    SoBaseKit* coinNode = static_cast<SoBaseKit*>(m_modelScene->getInstance(currentIndex)->getNode() );

    SoTransform* nodeTransform = static_cast< SoTransform* >(coinNode->getPart("transform", true) );

    QUndoCommand* command = new QUndoCommand();

    QString translationValue = QString("%1 %2 %3").arg(
        QString::number(nodeTransform->translation.getValue()[0]),
        QString::number(nodeTransform->translation.getValue()[1]),
        QString::number(nodeTransform->translation.getValue()[2])
    );
    new CmdModifyParameter(nodeTransform, "translation", translationValue, m_modelScene, command);
    m_undoStack->push(command);

    UpdateLightSize();
    setDocumentModified(true);
}

void MainWindow::ExecuteScriptFile(QString fileName)
{
    ScriptEditorDialog dialog(m_pluginManager->getRandomFactories(), this);
    dialog.show();
    dialog.ExecuteScript(fileName);
    dialog.done(0);
}

void MainWindow::onAbort(QString error)
{
    statusBar()->showMessage(error, 2000);
}

/*!
 * Starts manipulating current selected node with \a dragger.
 */
void MainWindow::StartManipulation(SoDragger* dragger)
{
    SoSearchAction coinSearch;
    coinSearch.setNode(dragger);
    coinSearch.setInterest(SoSearchAction::FIRST);

    coinSearch.apply(m_graphicsRoot->getRoot() );

    SoPath* coinScenePath = coinSearch.getPath();
    if (!coinScenePath) gcf::SevereError("PathFromIndex Null coinScenePath.");

    SoNodeKitPath* nodePath = static_cast<SoNodeKitPath*> (coinScenePath);
    if (!nodePath) gcf::SevereError("PathFromIndex Null nodePath.");


    nodePath->truncate(nodePath->getLength() - 1);
    SoBaseKit* coinNode = static_cast<SoBaseKit*> (nodePath->getTail() );

    QModelIndex nodeIndex = m_modelScene->indexFromPath(*nodePath);
    m_modelSelection->setCurrentIndex(nodeIndex, QItemSelectionModel::ClearAndSelect);

    SoNode* manipulator = coinNode->getPart("transform", true);
    m_manipulators_Buffer = new QStringList();

    SoFieldList fieldList;
    int totalFields = manipulator->getFields(fieldList);

    SoField* pField = 0;
    SbString fieldValue = "null";

    for (int index = 0; index < totalFields; ++index)
    {
        pField = fieldList.get(index);
        if (pField)
        {
            pField->get(fieldValue);
            m_manipulators_Buffer->push_back(QString(fieldValue.getString() ) );
        }
    }
}

/*!
 * Defines Tonatiuh model light paramenters with a dilog window.
 */
void MainWindow::onSunDialog()
{
    TSceneKit* sceneKit = m_document->getSceneKit();
    if (!sceneKit) return;

    SunKit* sunKitOld = static_cast<SunKit*>(sceneKit->getPart("lightList[0]", false) );

    SunDialog dialog(m_modelScene, sunKitOld, m_pluginManager->getSunMap(), this);
    if (!dialog.exec()) return;

    SunKit* sunKit = dialog.getSunKit();
    if (!sunKit) return;

    CmdSunModified* cmd = new CmdSunModified(sunKit, sceneKit, m_modelScene);
    m_undoStack->push(cmd);

    sceneKit->updateTrackers();
    UpdateLightSize();

    ui->parametersTabs->UpdateView();
    setDocumentModified(true);

    m_graphicsRoot->rays()->removeAllChildren();
//    ui->actionViewRays->setEnabled(false);
//    ui->actionViewRays->setChecked(false);
}

/*!
 * Opens a dialog to define the scene transmissivity.
 */
void MainWindow::onAirDialog()
{
    AirDialog dialog(m_pluginManager->getAirMap(), this);

    TSceneKit* sceneKit = m_document->getSceneKit();
    if (!sceneKit) return;
    Air* airOld = static_cast<Air*>(sceneKit->getPart("air", false));
    if (airOld) dialog.setModel(airOld);

    if (!dialog.exec()) return;

    Air* air = dialog.getModel();
    CmdAirModified* cmd = new CmdAirModified(air, sceneKit);
    if (m_undoStack) m_undoStack->push(cmd);
    setDocumentModified(true);
}

/*!
 * If actionDisplay_rays is checked the 3D view shows rays representation. Otherwise the representation is hidden.
 */
void MainWindow::ShowRays(bool on)
{
    m_graphicsRoot->showRays(on);
}

void MainWindow::ShowPhotons(bool on)
{
    m_graphicsRoot->showPhotons(on);
}

/*!
 * Inserts an existing tonatiuh component into the tonatiuh model as a selected node child.
 *
 * A open file dialog is opened to select the file where the existing component is saved.
 */
void MainWindow::InsertUserDefinedComponent()
{
    QModelIndex parentIndex;
    if ((!ui->sceneView->currentIndex().isValid() ) || (ui->sceneView->currentIndex() == ui->sceneView->rootIndex() ) )
        parentIndex = m_modelScene->index (0,0,ui->sceneView->rootIndex());
    else
        parentIndex = ui->sceneView->currentIndex();

    SoNode* coinNode = m_modelScene->getInstance(parentIndex)->getNode();

    if (!coinNode->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId() ) ) return;

    QSettings settings("CyI", "Tonatiuh");
    QString openDirectory = settings.value("componentOpenDirectory", QString(".") ).toString();

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Tonatiuh document"), openDirectory,
                                                    tr("Tonatiuh component (*.tcmp)") );

    if (fileName.isEmpty() ) return;

    QFileInfo file(fileName);
    settings.setValue("componentOpenDirectory", file.absolutePath() );

    InsertFileComponent(fileName);
}

/*!
 * Moves the scene node with index \a node to the parent with index \a newParent.
 */
void MainWindow::ItemDragAndDrop(const QModelIndex& newParent, const QModelIndex& node)
{
    if (node == ui->sceneView->rootIndex() ) return;

    InstanceNode* nodeInstnace = m_modelScene->getInstance(node);
    if (nodeInstnace->getParent()&&nodeInstnace->getParent()->getNode()->getTypeId().isDerivedFrom(SoBaseKit::getClassTypeId() ) )
    {
        SoNode* coinNode = nodeInstnace->getNode();
        //if( coinNode->getTypeId().isDerivedFrom( TTracker::getClassTypeId() ) ) return;

        QUndoCommand* dragAndDrop = new QUndoCommand();
        dragAndDrop->setText("Drag and Drop node");
        new CmdCut(node, m_coinNode_Buffer, m_modelScene, dragAndDrop);

        new CmdPaste(tgc::Copied, newParent, coinNode, *m_modelScene, dragAndDrop);
        m_undoStack->push(dragAndDrop);

        UpdateLightSize();
        setDocumentModified(true);
    }
}

/*!
 * Inserts a copy of the node \a node as a \a newParent child.
 */
void MainWindow::ItemDragAndDropCopy(const QModelIndex& newParent, const QModelIndex& node)
{
    InstanceNode* nodeInstnace = m_modelScene->getInstance(node);
    SoNode* coinNode = nodeInstnace->getNode();
    //if( coinNode->getTypeId().isDerivedFrom( TTracker::getClassTypeId() ) ) return;

    QUndoCommand* dragAndDropCopy = new QUndoCommand();
    dragAndDropCopy->setText("Drag and Drop Copy");
    new CmdCopy(node, m_coinNode_Buffer, m_modelScene);
    new CmdPaste(tgc::Shared, newParent, coinNode, *m_modelScene, dragAndDropCopy);
    m_undoStack->push(dragAndDropCopy);

    UpdateLightSize();
    setDocumentModified(true);
}

/*!
 * Shows a dialog to the user to open a existing tonatiuh file.
 */
void MainWindow::FileOpen()
{
    if (!OkToContinue()) return;

    // HKEY_CURRENT_USER\Software\CyI\Tonatiuh
    QSettings settings("CyI", "Tonatiuh");
    QString dir = settings.value("openDirectory", "../examples").toString();

    QString file = QFileDialog::getOpenFileName(
                this, "Open", dir,
                "Tonatiuh files (*.tnh)"
    );
    if (file.isEmpty()) return;

    QFileInfo info(file);
    settings.setValue("openDirectory", info.path());

    StartOver(file);
}

/*!
 * Opens selected tonatiuh file from the recent file menu.
 */
void MainWindow::FileOpenRecent()
{
    if (!OkToContinue()) return;
    QAction* action = qobject_cast<QAction*>(sender());
    if (action)
    {
        QString fileName = action->data().toString();
        StartOver(fileName);
    }
}

/*!
 * Applies las reverted command action changes to Tonatiuh.
 */
void MainWindow::Redo()
{
    m_undoStack->redo();
    UpdateLightSize();
}

/*!
 * Reverts a change to Tonatiuh model. The model is returne to the previous state before the command is applied.
 */
void MainWindow::Undo()
{
    m_undoStack->undo();
    UpdateLightSize();
}

/*!
 * Runs complete ray tracer. First defines export settings if they are not defined and then runs ray tracer.
 */
void MainWindow::RunCompleteRayTracer()
{
    InstanceNode* instanceRoot = 0;
    InstanceNode* instanceSun = 0;
    SunShape* sunShape = 0;
    SunAperture* sunAperture = 0;
    Air* air = 0;

//    QElapsedTimer timer;
//    timer.start();

    if (!ReadyForRaytracing(instanceRoot, instanceSun, sunShape, sunAperture, air) ) // ? 2 times
        return;

//    std::cout << "Elapsed time (ReadyForRaytracing): " << timer.elapsed() << std::endl;


        QVector<PhotonsFactory*> exportFactories = m_pluginManager->getExportFactories();
        QVector<RandomFactory*> randomFactories = m_pluginManager->getRandomFactories();

        RayTracingDialog dialog;
        dialog.setParameters(m_raysNumber, m_raysScreen,
                             randomFactories, m_raysRandomFactoryIndex,
                             m_raysGridWidth, m_raysGridHeight,
                             m_photonBufferSize, m_photonBufferAppend);
        dialog.setPhotonSettings(m_modelScene, exportFactories, m_photonsSettings);
        if (!dialog.exec()) return;

        SetRaysNumber(dialog.raysNumber());
        SetRaysScreen(dialog.raysScreen());
        SetRaysRandomFactory(randomFactories[dialog.raysRandomFactory()]->name());
        SetRaysGrid(dialog.raysGridWidth(), dialog.raysGridHeight());
        SetPhotonBufferSize(dialog.photonBufferSize());
        SetPhotonBufferAppend(dialog.photonBufferAppend());

    if (!m_photonsBuffer->getExporter()) {
        if (m_photonsSettings) delete m_photonsSettings;
        m_photonsSettings = new PhotonsSettings;
        *m_photonsSettings = dialog.getPhotonSettings();
    }

    Run();
}

/*!
 * Runs the ray tracer for the analysis of the surface. A dialog will be opened to set the surface and flux calculation parameters.
 */
void MainWindow::RunFluxAnalysisDialog()
{
    TSceneKit* sceneKit = m_document->getSceneKit();
    if (!sceneKit) return;

    Random* rand = m_pluginManager->getRandomFactories()[m_raysRandomFactoryIndex]->create();

    FluxAnalysisDialog dialog(sceneKit, m_modelScene, m_raysGridWidth, m_raysGridHeight, rand, this);
    dialog.exec();
}

/*!
 * Returns \a true if the tonatiuh model is correctly saved in the current file. Otherwise, returns \a false.
 *
 * If a current file is not defined, it calls to SaveAs funtios.
 *
 * \sa SaveAs, SaveFile.
 */
bool MainWindow::FileSave()
{
    if (m_currentFile.isEmpty() ) return FileSaveAs();
    else return SaveFile(m_currentFile);
}

/*!
 * Saves current selected node as a component in the files named \a componentFileName.
 */
void MainWindow::SaveComponent(QString fileName)
{
    if (!m_modelSelection->hasSelection() ) return;
    if (m_modelSelection->currentIndex() == ui->sceneView->rootIndex() ) return;

    QModelIndex componentIndex = ui->sceneView->currentIndex();
    SoNode* coinNode = m_modelScene->getInstance(componentIndex)->getNode();

    if (!coinNode->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId() ) )
    {
        QMessageBox::warning(0, tr("Tonatiuh"),
                             tr("Selected node in not valid  for component node") );
        return;
    }

    TSeparatorKit* componentRoot = dynamic_cast< TSeparatorKit* > (coinNode);
    if (!componentRoot) return;

    if (fileName.isEmpty() ) return;

    QFileInfo componentFile(fileName);
    if (componentFile.completeSuffix().compare("tcmp") ) fileName.append(".tcmp");

    SoWriteAction SceneOuput;
    if (!SceneOuput.getOutput()->openFile(fileName.toLatin1().constData() ) )
    {
        QMessageBox::warning(0, tr("Tonatiuh"),
                             tr("Cannot open file %1. ")
                             .arg(fileName));
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    SceneOuput.getOutput()->setBinary(false);
    SceneOuput.apply(componentRoot);
    SceneOuput.getOutput()->closeFile();
    QApplication::restoreOverrideCursor();
    return;
}

/*!
 * Returns \a true if the tonatiuh model is correctly saved. Otherwise, returns \a false. A file dialog is created to select a file.
 *
 * \sa Save, SaveFile.
 */
bool MainWindow::FileSaveAs()
{
    QSettings settings("CyI", "Tonatiuh");
    QString dir = settings.value("saveDirectory", "../examples").toString();

    QString file = QFileDialog::getSaveFileName(
        this, "Save", dir,
        "Tonatiuh files (*.tnh);; Tonatiuh debug (*.tnhd)"
    );
    if (file.isEmpty() ) return false;

    QFileInfo info(file);
    settings.setValue("saveDirectory", info.path());

    return SaveFile(file);
}

/*!
 * Saves selected node subtree as a component in a file.
 * A dialog is opened to select a file name and its location.
 */
bool MainWindow::SaveComponent()
{
    if (!m_modelSelection->hasSelection() ) return false;
    if (m_modelSelection->currentIndex() == ui->sceneView->rootIndex() ) return false;

    QModelIndex componentIndex = ui->sceneView->currentIndex();

    SoNode* coinNode = m_modelScene->getInstance(componentIndex)->getNode();

    if (!coinNode->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId() ) )
    {
        QMessageBox::warning(0, tr("Tonatiuh"),
                             tr("Selected node in not valid  for component node") );
        return false;
    }

    TSeparatorKit* componentRoot = dynamic_cast< TSeparatorKit* > (coinNode);
    if (!componentRoot) return false;


    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Tonatiuh component"), ".",
                                                    tr("Tonatiuh component (*.tcmp)") );
    if (fileName.isEmpty() ) return false;

    QFileInfo componentFile(fileName);
    if (componentFile.completeSuffix().compare("tcmp") )
        fileName.append(".tcmp");

    SoWriteAction SceneOuput;
    if (!SceneOuput.getOutput()->openFile(fileName.toLatin1().constData() ) )
    {
        QMessageBox::warning(0, tr("Tonatiuh"),
                             tr("Cannot open file %1. ")
                             .arg(fileName));
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    SceneOuput.getOutput()->setBinary(false);
    SceneOuput.apply(componentRoot);
    SceneOuput.getOutput()->closeFile();
    QApplication::restoreOverrideCursor();
    return true;
}

/*!
 * Changes the selected elements to the nodes into \a selection.
 */
void MainWindow::SelectionFinish(SoSelection* selection)
{
    if (selection->getNumSelected() == 0) return;

    SoPath* path = selection->getPath(0);
    if (!path) return;
    if (!path->containsNode(m_document->getSceneKit())) return;

    SoNodeKitPath* nodeKitPath = static_cast<SoNodeKitPath*>(path);
    if (nodeKitPath->getTail()->getTypeId().isDerivedFrom(SunKit::getClassTypeId() ) )
    {
        selection->deselectAll();
        QModelIndex currentIndex = m_modelSelection->currentIndex();
        m_modelSelection->setCurrentIndex(currentIndex, QItemSelectionModel::ClearAndSelect);
        return;
    }
    if (nodeKitPath->getTail()->getTypeId().isDerivedFrom(SoDragger::getClassTypeId() ) )
        return;

    QModelIndex index = m_modelScene->indexFromPath(*nodeKitPath);
    if (!index.isValid()) return;
    m_modelSelection->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
//    m_selectionModel->select(index, QItemSelectionModel::ClearAndSelect);
}

/*!
 * Changes to the node defined by \a node the value of the \a parameter to \a value.
 */
void MainWindow::SetParameterValue(SoNode* node, QString name, QString value)
{
    CmdModifyParameter* cmd = new CmdModifyParameter(node, name, value, m_modelScene);
    if (m_undoStack) m_undoStack->push(cmd);
//    UpdateLightSize();
    setDocumentModified(true);
}

/*!
 * This property holds whether the SunPositionCalculator action is enabled.
 * If the action is disabled, it does not disappear from menu, but it is displayed
 * in a way which indicates that they are unavailable.
 */
void MainWindow::SetSunPositionCalculatorEnabled(int enabled)
{
    ui->actionSceneSunCalculator->setEnabled(enabled);
}

/*!
 * Shows a windows with the applied commands.
 * The most recently executed command is always selected.
 * When a different command is selected the model returns to the state after selected command was applied.
 */
void MainWindow::ShowCommandView()
{
    m_undoView->show();
}

/*!
 * Shows selected node right menu.
 */
void MainWindow::ShowMenu(const QModelIndex& index)
{
    if (!index.isValid()) return;
    m_modelSelection->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);

    InstanceNode* instanceNode = m_modelScene->getInstance(index);
    SoNode* coinNode = instanceNode->getNode();
    SoType type = coinNode->getTypeId();

    QMenu popupmenu(this);

    if (instanceNode->getParent() && instanceNode->getParent()->getNode()->getTypeId().isDerivedFrom(SoBaseKit::getClassTypeId() ) )
    {
        popupmenu.addAction(ui->actionEditCut);
        popupmenu.addAction(ui->actionEditCopy);
        popupmenu.addAction(ui->actionEditPaste);
        popupmenu.addAction(ui->actionEditPasteLink);
        popupmenu.addAction(ui->actionEditDelete);
        popupmenu.addAction("Collapse all", ui->sceneView, SLOT(collapseAll()));
    }

    if (type.isDerivedFrom(TSeparatorKit::getClassTypeId() ) )
    {
        QMenu* transformMenu = popupmenu.addMenu("Convert to");
        TSeparatorKit* coinKit = dynamic_cast<TSeparatorKit*> (coinNode);
        SoTransform* transform = static_cast<SoTransform*>(coinKit->getPart("transform", true));
        SoType transformType = transform->getTypeId();

        if (!transformType.isDerivedFrom(SoCenterballManip::getClassTypeId()) )
            transformMenu->addAction("SoCenterballManip", this, SLOT(SoTransform_to_SoCenterballManip()));
        if (!transformType.isDerivedFrom(SoHandleBoxManip::getClassTypeId()) )
            transformMenu->addAction("SoHandleBoxManip", this, SLOT(SoTransform_to_SoHandleBoxManip()));
        if (!transformType.isDerivedFrom(SoJackManip::getClassTypeId()) )
            transformMenu->addAction("SoJackManip", this, SLOT(SoTransform_to_SoJackManip()));
        if (!transformType.isDerivedFrom(SoTabBoxManip::getClassTypeId()) )
            transformMenu->addAction("SoTabBoxManip", this, SLOT(SoTransform_to_SoTabBoxManip()));
        if (!transformType.isDerivedFrom(SoTrackballManip::getClassTypeId()) )
            transformMenu->addAction("SoTrackballManip", this, SLOT(SoTransform_to_SoTrackballManip()));
        if (!transformType.isDerivedFrom(SoTransformBoxManip::getClassTypeId()) )
            transformMenu->addAction("SoTransformBoxManip", this, SLOT(SoTransform_to_SoTransformBoxManip()));
        if (!transformType.isDerivedFrom(SoTransformerManip::getClassTypeId()) )
            transformMenu->addAction("SoTransformerManip", this, SLOT(SoTransform_to_SoTransformerManip()));
        if (transformType.isDerivedFrom(SoTransformManip::getClassTypeId()) )
            transformMenu->addAction("SoTransform", this, SLOT(SoManip_to_SoTransform()) );
    }

    popupmenu.exec(QCursor::pos());
}

void MainWindow::ShowWarning(QString message)
{
    QMessageBox::warning(this, "Tonatiuh", message);
}

//View menu actions
void MainWindow::on_actionViewAxes_toggled()
{
    m_graphicView[0]->showAxes(ui->actionViewAxes->isChecked() );
    m_graphicView[1]->showAxes(ui->actionViewAxes->isChecked() );
    m_graphicView[2]->showAxes(ui->actionViewAxes->isChecked() );
    m_graphicView[3]->showAxes(ui->actionViewAxes->isChecked() );
}

void MainWindow::on_actionQuadView_toggled()
{
    if (!ui->actionQuadView->isChecked()) {
        m_graphicView[1]->hide();
        m_graphicView[2]->hide();
        m_graphicView[3]->hide();
        m_focusView = 0;
        QSplitter* splitter = findChild<QSplitter*>("graphicSplitterH2");
        splitter->hide();
    } else {       
        m_graphicView[1]->show();
        m_graphicView[2]->show();
        m_graphicView[3]->show();
        QSplitter* splitter = findChild<QSplitter*>("graphicSplitterH2");
        splitter->show();
    }
}

SbVec3f MainWindow::getTarget(SoCamera* camera)
{ 
    SbRotation rotation = camera->orientation.getValue();
    SbVec3f target;
    rotation.multVec(SbVec3f(0., 0., -camera->focalDistance.getValue()), target);
    target += camera->position.getValue();
    return target;
}

void MainWindow::on_actionRunScript_triggered()
{
//    m_undoStack->setActive(false);
    m_undoStack->beginMacro("Script");
    ScriptEditorDialog dialog(m_pluginManager->getRandomFactories(), this);
    dialog.exec();
//    m_undoStack->setActive(true);
    m_undoStack->endMacro();
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dialog;
    dialog.exec();
}

/*!
 * Changes the number of the grid cells and grid cell dimensions.
 */
void MainWindow::ChangeGridSettings()
{
//    GridDialog dialog();
//    if (!dialog.exec()) return;

//    m_graphicsRoot->RemoveGrid();

//    m_gridXElements = dialog.GetXDimension();
//    m_gridZElements = dialog.GetZDimension();

//    if (dialog.IsSizeDefined() )
//    {
//        m_gridXSpacing = dialog.GetXSpacing();
//        m_gridZSpacing = dialog.GetZSpacing();
//    }
//    else
//    {
//        InstanceNode* sceneInstance = m_sceneModel->NodeFromIndex(ui->sceneView->rootIndex() );
//        if (!sceneInstance) return;
//        InstanceNode* rootInstance = sceneInstance->children[sceneInstance->children.size() - 1 ];
//        if (!rootInstance) return;

//        SoGetBoundingBoxAction* bbAction = new SoGetBoundingBoxAction(SbViewportRegion() );
//        rootInstance->GetNode()->getBoundingBox(bbAction);

//        SbBox3f box = bbAction->getXfBoundingBox().project();
//        delete bbAction;

//        m_gridXSpacing = 10;
//        m_gridZSpacing = 10;

//        if (!box.isEmpty() )
//        {
//            SbVec3f min, max;
//            box.getBounds(min, max);

//            m_gridXSpacing = (2 *  std::max(fabs(max[0]), fabs(min[0]) ) + 5) / m_gridXElements;
//            m_gridZSpacing = (2 *  std::max(fabs(max[2]), fabs(min[2]) ) + 5) / m_gridZElements;
//        }
//    }

////    m_graphicsRoot->AddGrid(CreateGrid(m_gridXElements, m_gridZElements, m_gridXSpacing, m_gridZSpacing) );
//    m_graphicsRoot->AddGrid(CreateGrid() );
//    m_graphicsRoot->ShowGrid(true);
}

void MainWindow::ChangeNodeName(const QModelIndex& index, const QString& name)
{
    if (!index.isValid()) return;

    InstanceNode* instance = m_modelScene->getInstance(index);
    if (!instance) return;
    if (!instance->getNode()) return;

    CmdChangeNodeName* cmd = new CmdChangeNodeName(index, name, m_modelScene);
    m_undoStack->push(cmd);

    setDocumentModified(true);
}

/*!
 * Adds a surface to the export sufaces list.
 */
void MainWindow::AddExportSurfaceURL(QString nodeURL)
{
    if (!m_photonsSettings) return;
    m_photonsSettings->surfaces.push_back(nodeURL);
}

/*!
 * Changes the light position to the position defined by \a azimuth and \a elevation.
 * The parameters are defined in degree.
 */
void MainWindow::ChangeSunPosition(double azimuth, double elevation)
{
    SoSceneKit* coinScene = m_document->getSceneKit();
    SunKit* lightKit = static_cast<SunKit*>(coinScene->getPart("lightList[0]", false) );
    if (!lightKit)
    {
        QMessageBox::warning(this, "Tonatiuh warning", tr("ChangeSunPosition:: Sun not defined in scene") );
        return;
    }
    CmdLightPositionModified* command = new CmdLightPositionModified(lightKit, azimuth * gcf::degree, (90 - elevation) * gcf::degree);
    m_undoStack->push(command);

    //UpdateLightDimensions();
    UpdateLightSize();
    setDocumentModified(true);

//    ui->actionViewRays->setEnabled(false);
//    ui->actionViewRays->setChecked(false);
}

/*!
 * Sets the sun position for the location given by \a latitude and \a longitude coordinates in degrees.
 * Latitude is positive for North and positive longitude for east.
 * The time  defined by \a year, \a month and \a day, \a hours, \a minustes and \a seconds is ut time.
 */
void MainWindow::ChangeSunPosition(int year, int month, int day, double hours, double minutes, double seconds, double latitude, double longitude)
{
    if ( (month < 0) || (month> 12) ||
         (day < 0) || (day > 31) ||
         (hours < 0) || (hours > 23) ||
         (minutes < 0) || (minutes > 59) ||
         (seconds < 0) || (seconds > 59) ||
         (longitude < -180.) || (longitude > 180.) ||
         (latitude < -90.) || (latitude > 90.) )
    {
        QMessageBox::warning(this, "Tonatiuh warning", tr("ChangeSunPosition:: Not valid value define to new sun position.") );
        return;
    }

    cTime myTime = {year, month, day, hours, minutes, seconds};
    cLocation myLocation = {longitude, latitude};
    cSunCoordinates results;
    sunpos(myTime, myLocation, &results);
    ChangeSunPosition(results.dAzimuth, (90 - results.dZenithAngle) );

//    ui->actionViewRays->setEnabled(false);
//    ui->actionViewRays->setChecked(false);
}

/*!
 * Clears current design in Tonatiuh without verify if the changes have been saved.
 */
void MainWindow::Clear()
{
    StartOver("");
}

/*!
 * Copies current node to the clipboard.
 * The current node cannot be the model root node or concentrator node.
 */
void MainWindow::Copy()
{
    if (!m_modelSelection->hasSelection() )
    {
        emit Abort(tr("Copy: No node seleted to copy.") );
        return;
    }
    if (m_modelSelection->currentIndex() == ui->sceneView->rootIndex() )
    {
        emit Abort(tr("Copy: The root node can not bee copied.") );
        return;
    }
    if (m_modelSelection->currentIndex().parent() == ui->sceneView->rootIndex() )
    {
        emit Abort(tr("Copy: The root node can not bee copied.") );
        return;
    }

    CmdCopy* command = new CmdCopy(m_modelSelection->currentIndex(), m_coinNode_Buffer, m_modelScene);
    m_undoStack->push(command);

    setDocumentModified(true);
}

/*!
 * Copies the node defined with the \a nodeURL to the clipboard.
 *
 * The current node cannot be the model root node or concentrator node.
 */
void MainWindow::Copy(QString url)
{
    QModelIndex nodeIndex = m_modelScene->indexFromUrl(url);

    if (!nodeIndex.isValid() )
    {
        emit Abort(tr("Copy: There is no node with defined url.") );
        return;
    }
    if (nodeIndex == ui->sceneView->rootIndex() || nodeIndex.parent() == ui->sceneView->rootIndex() )
    {
        emit Abort(tr("Copy: The root node can not bee copied.") );
        return;
    }

    CmdCopy* command = new CmdCopy(nodeIndex, m_coinNode_Buffer, m_modelScene);
    m_undoStack->push(command);

    setDocumentModified(true);
}

/*!
 * If there is a node selected and this node is not the root node, cuts current selected node from the model. Otherwise, nothing is done.
 */
void MainWindow::Cut()
{
    int validNode = 1;
    if (!m_modelSelection->hasSelection() ) validNode = 0;
    if (m_modelSelection->currentIndex() == ui->sceneView->rootIndex() ) validNode = 0;
    if (m_modelSelection->currentIndex().parent() == ui->sceneView->rootIndex() ) validNode = 0;
    if (!validNode)
    {
        emit Abort(tr("Cut: No valid node selected to cut.") );
        return;
    }
    CmdCut* cmd = new CmdCut(m_modelSelection->currentIndex(), m_coinNode_Buffer, m_modelScene);
    m_undoStack->push(cmd);

    UpdateLightSize();
    setDocumentModified(true);
}

/*!
 * If there \a nodeURL is a valid node url and this node is not the root node, cuts current selected node from the model. Otherwise, nothing is done.
 */
void MainWindow::Cut(QString url)
{
    if (url.isEmpty() )
    {
        emit Abort(tr("Cut: There is no node with defined url.") );
        return;
    }

    QModelIndex nodeIndex = m_modelScene->indexFromUrl(url);

    if (!nodeIndex.isValid() )
    {
        emit Abort(tr("Cut: There is no node with defined url.") );
        return;
    }

    if (nodeIndex == ui->sceneView->rootIndex() )
    {
        emit Abort(tr("Cut: Selected node is not valid node to cut.") );
        return;
    }
    if (nodeIndex.parent() == ui->sceneView->rootIndex() )
    {
        emit Abort(tr("Cut: Selected node is not valid node to cut.") );
        return;
    }

    CmdCut* command = new CmdCut(nodeIndex, m_coinNode_Buffer, m_modelScene);
    m_undoStack->push(command);

    UpdateLightSize();
    setDocumentModified(true);
}

/*!
 * * Deletes selected node if there is a valid node to delete.
 */
void MainWindow::Delete()
{
    if (!m_modelSelection->hasSelection() )
    {
        emit Abort("Delete: There is no node selected to delete");
        return;
    }

    QModelIndex index = m_modelSelection->currentIndex();
    m_modelSelection->clearSelection();

    InstanceNode* instance = m_modelScene->getInstance(index);
    m_modelSelection->setCurrentIndex(m_modelScene->indexFromUrl(instance->getParent()->getURL()), QItemSelectionModel::ClearAndSelect);

    Delete(index);
}

/*!
 * Creates a new delete command, where the node with the \a nodeURL was deleted.
 *
 * If \a nodeURL is not a valid node url, nothing is done.
 */
void MainWindow::Delete(QString url)
{
    QModelIndex index = m_modelScene->indexFromUrl(url);
    if (!index.isValid())
    {
        emit Abort(tr("Delete: There is no node with defined url.") );
        return;
    }

    Delete(index);
    if (m_modelSelection->isSelected(index)) m_modelSelection->clearSelection();
}

/*!
 * Creates a new delete command, where the \a index node was deleted.
 *
 * Returns \a true if the node was successfully deleted, otherwise returns \a false.
 */
bool MainWindow::Delete(QModelIndex index)
{
    if (!index.isValid()) return false;
    if (index == ui->sceneView->rootIndex() ) return false;
    if (index.parent() == ui->sceneView->rootIndex() ) return false;

    InstanceNode* instance = m_modelScene->getInstance(index);
    SoNode* node = instance->getNode();

    if (node->getTypeId().isDerivedFrom(Tracker::getClassTypeId()))
    {
        CmdDeleteTracker* cmd = new CmdDeleteTracker(index, m_document->getSceneKit(), *m_modelScene);
        m_undoStack->push(cmd);
    }
    else if (node->getTypeId().isDerivedFrom(SunKit::getClassTypeId()))
        return false;
    else
    {
        CmdDelete* cmd = new CmdDelete(index, m_modelScene);
        m_undoStack->push(cmd);
    }

//    UpdateLightSize();
    setDocumentModified(true);
    return true;
}

/*!
 * Inserts a new node as a \a nodeURL node child. The new node is a copy to node saved into the clipboard.
 * The \a pasteType take "Shared" or "Copied" values.
 */
void MainWindow::Paste(QString url, QString pasteType)
{
    if (!m_coinNode_Buffer)
    {
        emit Abort(tr("Paste: There is not node copied.") );
        return;
    }

    QModelIndex nodeIndex = m_modelScene->indexFromUrl(url);
    if (!nodeIndex.isValid() )
    {
        emit Abort(tr("Paste: The node url is not valid.") );
        return;
    }

    if (pasteType == "Shared")
        Paste(nodeIndex, tgc::Shared);
    else
        Paste(nodeIndex, tgc::Copied);
}

/*!
 * Inserts a new node as a current node child. The new node is a copy to node saved into the clipboard.
 */
void MainWindow::PasteCopy()
{
    if (!m_modelSelection->hasSelection())
    {
        emit Abort("PasteCopy: There is not node copied.");
        return;
    }
    Paste(m_modelSelection->currentIndex(), tgc::Copied);
}

/*!
 * Inserts a new node as a current node child. The new node is a link to node saved into the clipboard.
 */
void MainWindow::PasteLink()
{
    if (!m_modelSelection->hasSelection())
    {
        emit Abort("PasteCopy: There is not node copied.");
        return;
    }
    Paste(m_modelSelection->currentIndex(), tgc::Shared);
}

/*!
 * Creates a new group node as a selected node child.
 */
void MainWindow::InsertNode()
{
    QModelIndex index = ui->sceneView->currentIndex();
    if (!index.isValid()) return;
    ui->sceneView->expand(index);

    InstanceNode* instance = m_modelScene->getInstance(index);
    if (!instance)
    {
        emit Abort(tr("CreateGroupNode: Error creating new group node.") );
        return;
    }
    SoNode* node = instance->getNode();
    if (!node)
    {
        emit Abort(tr("CreateGroupNode: Error creating new group node.") );
        return;
    }
    if (!node->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId() ) )
        return;

    TSeparatorKit* kit = new TSeparatorKit;
    CmdInsertNode* cmd = new CmdInsertNode(kit, QPersistentModelIndex(index), m_modelScene);
    m_undoStack->push(cmd);

    QString name("Node");
    int count = 0;
    while (!m_modelScene->SetNodeName(kit, name))
        name = QString("Node_%1").arg(++count);

    setDocumentModified(true);
}

/*!
 * Creates a surface node as selected node child.
 */
void MainWindow::InsertShape()
{
    QModelIndex index = ui->sceneView->currentIndex();
    if (!index.isValid()) return;
    ui->sceneView->expand(index);

    InstanceNode* instance = m_modelScene->getInstance(index);
    if (!instance) return;
    SoNode* node = instance->getNode();
    if (!node) return;
    if (!node->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId()))
        return;

    TShapeKit* kit = new TShapeKit;
    CmdInsertShape* cmd = new CmdInsertShape(kit, index, m_modelScene);
    m_undoStack->push(cmd);

    QString name("Shape");
    int count = 0;
    while (!m_modelScene->SetNodeName(kit, name))
        name = QString("Shape_%1").arg(++count);

    Select(instance->getURL() + "/" + name);
    setDocumentModified(true);
}

/*!
 * Creates a \a shapeType shape node from the as current selected node child.
 *
 * If the current node is not a valid parent node or \a shapeType is not a valid type, the shape node will not be created.
 *
 */
void MainWindow::InsertSurface(QString name)
{
    ShapeFactory* f = m_pluginManager->getShapeMap().value(name, 0);
    if (f)
        InsertSurface(f);
    else
        emit Abort("CreateShape: Shape not found");
}

/*!
 * Creates a \a shapeType shape node from the as current selected node child with the parameters defined in \a parametersList. \a numberOfParameters is the
 * number of parametners in the vector \a numberOfParameters
 *
 * If the current node is not a valid parent node or \a shapeType is not a valid type, the shape node will not be created.
 *
 */
void MainWindow::InsertSurface(QString name, int numberOfParameters, QVector<QVariant> parametersList)
{
    ShapeFactory* f = m_pluginManager->getShapeMap().value(name, 0);
    if (f)
        InsertSurface(f, numberOfParameters, parametersList);
    else
        emit Abort("CreateShape: Selected shape type is not valid.");
}

void MainWindow::InsertProfile(QString name)
{
    ProfileFactory* f = m_pluginManager->getProfileMap().value(name, 0);
    if (f)
        InsertProfile(f);
    else
        emit Abort("InsertProfile: Profile not found");
}

/*!
 * Creates a \a materialType material node from the as current selected node child.
 *
 * If the current node is not a surface type node or \a materialType is not a valid type, the material node will not be created.
 */
void MainWindow::InsertMaterial(QString name)
{
    MaterialFactory* f = m_pluginManager->getMaterialMap().value(name, 0);
    if (f)
        InsertMaterial(f);
    else
        emit Abort("CreateMaterial: Material not found");
}

/*!
 * Creates a \a trackerType shape node from the as current selected node child.
 *
 * If the current node is not a valid parent node or \a trackerType is not a valid type, the shape node will not be created.
 *
 */
void MainWindow::InsertTracker(QString name)
{
    TrackerFactory* f = m_pluginManager->getTrackerMap().value(name, 0);
    if (f)
        InsertTracker(f);
    else
        emit Abort("CreateTracker: Selected tracker type is not valid.");
}

/*!
 * Creates a \a componentType component node with the name \a nodeName.
 */
void MainWindow::CreateComponentNode(QString componentType, QString nodeName, int numberofParameters, QVector<QVariant> parametersList)
{
    QModelIndex parentIndex = ui->sceneView->currentIndex();
    if (!parentIndex.isValid()) return;

    InstanceNode* parentInstance = m_modelScene->getInstance(parentIndex);
    SoNode* parentNode = parentInstance->getNode();
    if (!parentNode->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId() ) ) return;

    QVector<ComponentFactory*> factoryList = m_pluginManager->getComponentFactories();
    if (factoryList.size() == 0) return;

    QVector<QString> componentNames;
    for (int i = 0; i < factoryList.size(); i++)
        componentNames << factoryList[i]->name();

    int selectedCompoent = componentNames.indexOf(componentType);
    if (selectedCompoent < 0)
    {
        emit Abort(tr("CreateComponentNode: Selected component type is not valid.") );
        return;
    }
    setDocumentModified(true);

    ComponentFactory* pComponentFactory = factoryList[selectedCompoent];
    if (!pComponentFactory) return;

    //CreateComponent( pComponentFactory );

    TSeparatorKit* componentLayout = pComponentFactory->CreateTComponent(m_pluginManager, numberofParameters, parametersList);
    if (!componentLayout) return;

//    QString typeName = pComponentFactory->name();
    componentLayout->setName(nodeName.toStdString().c_str() );

    CmdInsertNode* cmd = new CmdInsertNode(componentLayout, QPersistentModelIndex(parentIndex), m_modelScene);
    QString commandText = QString("Create Component: %1").arg(pComponentFactory->name().toLatin1().constData() );
    cmd->setText(commandText);
    m_undoStack->push(cmd);

    UpdateLightSize();
    setDocumentModified(true);
}

/*!
 *
 * Inserts an existing tonatiuh component into the tonatiuh model as a selected node child.
 *
 * The component is saved into \a componentFileName file.
 */
void MainWindow::InsertFileComponent(QString componentFileName)
{
    QModelIndex parentIndex;
    if ((!ui->sceneView->currentIndex().isValid() ) || (ui->sceneView->currentIndex() == ui->sceneView->rootIndex() ) )
        parentIndex = m_modelScene->index (0,0,ui->sceneView->rootIndex());
    else
        parentIndex = ui->sceneView->currentIndex();

    SoNode* coinNode = m_modelScene->getInstance(parentIndex)->getNode();
    if (!coinNode->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId() ) )
    {
        emit Abort(tr("InsertFileComponent: Parent node is not valid node type.") );
        return;
    }

    if (componentFileName.isEmpty() )
    {
        emit Abort(tr("InsertFileComponent: Cannot open file:\n%1.").arg(componentFileName) );
        return;
    }

    SoInput componentInput;
    if (!componentInput.openFile(componentFileName.toLatin1().constData() ) )
    {
        emit Abort(tr("Cannot open file:\n%1.").arg(componentFileName) );
        return;
    }

    SoSeparator* componentSeparator = SoDB::readAll(&componentInput);
    componentInput.closeFile();

    if (!componentSeparator)
    {
        emit Abort(tr("Error reading file: \n%1.").arg(componentFileName) );
        return;
    }

    TSeparatorKit* componentLayout = static_cast< TSeparatorKit* >(componentSeparator->getChild(0) );

    CmdInsertNode* cmdInsertSeparatorKit = new CmdInsertNode(componentLayout, QPersistentModelIndex(parentIndex), m_modelScene);
    cmdInsertSeparatorKit->setText("Insert SeparatorKit node");
    m_undoStack->push(cmdInsertSeparatorKit);

    UpdateLightSize();
    setDocumentModified(true);
}

#include "script/NodeObject.h"

void MainWindow::InsertScene(QScriptValue v)
{
    QModelIndex parentIndex;
    if ((!ui->sceneView->currentIndex().isValid() ) || (ui->sceneView->currentIndex() == ui->sceneView->rootIndex() ) )
        parentIndex = m_modelScene->index (0,0,ui->sceneView->rootIndex());
    else
        parentIndex = ui->sceneView->currentIndex();

    SoNode* coinNode = m_modelScene->getInstance(parentIndex)->getNode();
    if (!coinNode->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId() ) )
    {
        emit Abort(tr("InsertFileComponent: Parent node is not valid node type.") );
        return;
    }
    NodeObject* no = (NodeObject*)v.toQObject();
    CmdInsertNode* cmdInsertSeparatorKit = new CmdInsertNode((TSeparatorKit*)no->getNode(), QPersistentModelIndex(parentIndex), m_modelScene);
    cmdInsertSeparatorKit->setText("Insert SeparatorKit node");
    m_undoStack->push(cmdInsertSeparatorKit);

    UpdateLightSize();
    setDocumentModified(true);
}

/*!
 * Changes selected node to the node with \a nodeUrl. If model does not contains a node with defined url,
 * the selection will be null.
 */
void MainWindow::Select(QString url)
{
    QModelIndex index = m_modelScene->indexFromUrl(url);
    m_modelSelection->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
}

/*!
 * Sets \a nodeName as the current node name.
 */
void MainWindow::SetName(QString name)
{
    if (!m_modelSelection->hasSelection())
    {
        emit Abort(tr("SetNodeName: No node selected.") );
        return;
    }

    if (m_modelSelection->currentIndex() == ui->sceneView->rootIndex())
    {
        emit Abort(tr("SetNodeName: Cannot change the name of the current selected node cannot.") );
        return;
    }

    ChangeNodeName(m_modelSelection->currentIndex(), name);
}

/*!
 * Sets to the \a parameter of the node \a nodeUrl the value defined at \a value.
 */
void MainWindow::SetValue(QString url, QString parameter, QString value)
{
    if (url.isEmpty() || parameter.isEmpty() || value.isEmpty() )
    {
        emit Abort(tr("SetValue: You must define a node url, a parameter name and a value.") );
        return;
    }

    QModelIndex nodeIndex = m_modelScene->indexFromUrl(url);
    if (!nodeIndex.isValid() )
    {
        emit Abort(tr("SetValue: Defined node url is not a valid url.") );
        return;
    }

    InstanceNode* nodeInstance = m_modelScene->getInstance(nodeIndex);
    if (!nodeInstance)
    {
        emit Abort(tr("SetValue: Defined node url is not a valid url.") );
        return;
    }

    SoNode* node = nodeInstance->getNode();
    if (!node) return;
    if (node->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId() ) )
    {
        TSeparatorKit* separatorNode = static_cast< TSeparatorKit* >(node);
        SoTransform* nodeTransform = static_cast< SoTransform* >(separatorNode->getPart("transform", true) );
        node = nodeTransform;
    }

    SoField* parameterField = node->getField(parameter.toStdString().c_str() );
    if (!parameterField)
    {
        QMessageBox::information(this, "Tonatiuh",
            QString("Defined node has not contains the parameter %1.").arg(parameter), 1);
        return;
    }

    SetParameterValue(node, parameter, value);
}

/*!
 * Starts new tonatiuh empty model.
 */
void MainWindow::FileNew()
{
    if (!OkToContinue())
    {
        emit Abort(tr("Current Tonatiuh model cannot be closed.") );
        return;
    }
    StartOver("");
}

/*!
 * Opens \a fileName file is there is a valid tonatiuh file.
 */
void MainWindow::FileOpen(QString fileName)
{
    if (fileName.isEmpty() )
    {
        QMessageBox::warning(this, "Tonatiuh",
                             tr("Open: Cannot open file:\n%1.").arg(fileName) );
        emit Abort(tr("Open: Cannot open file:\n%1.").arg(fileName) );
        return;
    }

    QFileInfo file(fileName);
    if (!file.exists() || !file.isFile() || file.suffix()!=QString("tnh") )
    {
        QMessageBox::warning(this, "Tonatiuh",
                             tr("Open: Cannot open file:\n%1.").arg(fileName) );
        emit Abort(tr("Open: Cannot open file:\n%1.").arg(fileName) );
        return;
    }

    StartOver(fileName);
}

/*!
 * Runs ray tracer to defined model and paramenters.
 */
void MainWindow::Run()
{
    InstanceNode* instanceRoot = 0;
    InstanceNode* instanceSun = 0;
    SunShape* sunShape = 0;
    SunAperture* sunAperture = 0;
    Air* air = 0;

    QElapsedTimer timer;
    timer.start();

    UpdateLightSize();

    if (ReadyForRaytracing(instanceRoot, instanceSun, sunShape, sunAperture, air) )
    {
        if (!m_photonsBuffer->getExporter() )
        {
            if (!m_photonsSettings) return;
            PhotonsAbstract* photonsExporter = CreatePhotonMapExport();
            if (!photonsExporter) return;
            if (!m_photonsBuffer->setExporter(photonsExporter)) return;
        }

        QVector<InstanceNode*> exportSuraceList;
        for (QString s : m_photonsSettings->surfaces)
        {
            m_modelScene->indexFromUrl(s);
            InstanceNode* node = m_modelScene->getInstance(m_modelScene->indexFromUrl(s));
            exportSuraceList << node;
        }

        instanceRoot->updateTree(Transform::Identity);

        SunKit* sunKit = static_cast<SunKit*> (instanceSun->getNode() );
        if (!sunKit->findTexture(m_raysGridWidth, m_raysGridHeight, instanceRoot))
        {
            emit Abort(tr("There are no surfaces defined for ray tracing") );
            ShowRaysIn3DView(); // cleaning?
            return;
        }

        QVector<long> raysPerThread;
        int progressMax = 100;
        ulong t1 = m_raysNumber/progressMax;
        for (int progress = 0; progress < progressMax; ++progress)
            raysPerThread << t1;

        if (t1*progressMax < m_raysNumber)
            raysPerThread << m_raysNumber - t1*progressMax;



        // single thread for gprof
//        QThreadPool::globalInstance()->setMaxThreadCount(1);
// change ideal

        // Create a progress dialog.
        QProgressDialog progressDialog;
        progressDialog.setWindowFlag(Qt::WindowContextHelpButtonHint, false);
        progressDialog.setLabelText(QString("Progressing using %1 thread(s)...").arg(QThread::idealThreadCount() ) );

        QFutureWatcher<void> futureWatcher;
        QObject::connect(&futureWatcher, SIGNAL(finished()), &progressDialog, SLOT(reset()));
        QObject::connect(&progressDialog, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));
        QObject::connect(&futureWatcher, SIGNAL(progressRangeChanged(int, int)), &progressDialog, SLOT(setRange(int, int)));
        QObject::connect(&futureWatcher, SIGNAL(progressValueChanged(int)), &progressDialog, SLOT(setValue(int)));

        std::cout << "QtConcurrent started: " << timer.elapsed() << std::endl;
        QMutex mutex;
        QMutex mutexPhotonMap;
        QFuture<void> photonMap;
        Air* airTemp = 0;
        if (!dynamic_cast<AirVacuum*>(air))
            airTemp = air;

        photonMap = QtConcurrent::map(raysPerThread, RayTracer(instanceRoot,
            instanceSun, sunAperture, sunShape, airTemp,
            *m_rand,
            &mutex, m_photonsBuffer, &mutexPhotonMap,
            exportSuraceList) );
        futureWatcher.setFuture(photonMap);

        // Display the dialog and start the event loop.
        progressDialog.exec();
        futureWatcher.waitForFinished();
        std::cout << "QtConcurrent finished: " << timer.elapsed() << std::endl;

        m_raysTracedTotal += m_raysNumber;

        if (exportSuraceList.empty())
            ShowRaysIn3DView(); // all photons must be stored
        else {
//            ui->actionViewRays->setEnabled(false);
//            ui->actionViewRays->setChecked(false);
        }

        double area = sunAperture->getArea();
        double irradiance = sunKit->irradiance.getValue();
        double power = area*irradiance/m_raysTracedTotal;
        m_photonsBuffer->endExport(power);
    }

    std::cout << "Elapsed time (Run): " << timer.elapsed() << std::endl;
}

/*
 * Runs ray trace to calculate a flux distribution map in the surface of the node \a nodeURL related to the side \a surfaceSide.
 * The map will be calculated with the parameters \a nOfRays, \a heightDivisions and \a heightDivisions.
 * The results will save in a file \a directory \a QString fileName, the coordinates of the cells depending on the variable \a saveCoord.
 */
void MainWindow::RunFluxAnalysis(QString nodeURL, QString surfaceSide, uint nOfRays, int heightDivisions, int widthDivisions, QString dirName, QString fileName, bool saveCoords)
{
    TSceneKit* sceneKit = m_document->getSceneKit();
    if (!sceneKit) return;

    if (!m_rand) m_rand = m_pluginManager->getRandomFactories()[m_raysRandomFactoryIndex]->create();

    FluxAnalysis fa(sceneKit, m_modelScene, m_raysGridWidth, m_raysGridHeight, m_rand);
    fa.run(nodeURL, surfaceSide, nOfRays, false, heightDivisions, widthDivisions); //?
    fa.write(dirName, fileName, saveCoords);
}

/*!
 * Saves current tonatiuh model into \a fileName file.
 */
void MainWindow::FileSaveAs(QString fileName)
{
    if (fileName.isEmpty() )
    {
        emit Abort(tr("SaveAs: There is no file defined.") );
        return;
    }
    QFileInfo fileInfo (fileName);
    if (fileInfo.completeSuffix() != QLatin1String("tnh") )
    {
        emit Abort(tr("SaveAs: The file defined is not a tonatiuh file. The suffix must be tnh.") );
        return;
    }
    SaveFile(fileName);
}

/*!
 * Sets to export all surfaces photons.
 */
void MainWindow::SetExportAllPhotonMap()
{
    if (!m_photonsSettings) return;
    m_photonsSettings->surfaces.clear();
}

/*!
 * If \a enabled is true, sets to export photons coordinates. Otherwise, the photons will not be exported.
 * If \a global is true, the coordinates will be exported in global coordinate system. Otherwise, into local to surface.
 */
void MainWindow::SetExportCoordinates(bool enabled, bool global)
{
    if (!m_photonsSettings) return;
    m_photonsSettings->saveCoordinates = enabled;
    m_photonsSettings->saveCoordinatesGlobal = global;
}

/*!
 * If \a enabled is true, sets to export intersection surface.
 * Otherwise, the intersection surface will not be exported.
 */
void MainWindow::SetExportIntersectionSurface(bool enabled)
{
    if (!m_photonsSettings) return;
    m_photonsSettings->saveSurfaceID = enabled;
}

/*!
 * If \a enabled is true, sets to export intersection surface side.
 * Otherwise, the intersection surface side will not be exported.
 */
void MainWindow::SetExportIntersectionSurfaceSide(bool enabled)
{
    if (!m_photonsSettings) return;
    m_photonsSettings->saveSurfaceSide = enabled;
}

/*!
 * Sets the export photon mode type to \a exportModeType, for photon map.
 */
void MainWindow::SetExportPhotonMapType(QString name)
{
    QVector<PhotonsFactory*> factoryList = m_pluginManager->getExportFactories();
    if (factoryList.size() == 0) return;

    QVector<QString> exportPMModeNames;
    for (int i = 0; i < factoryList.size(); i++)
        exportPMModeNames << factoryList[i]->name();

    if (exportPMModeNames.indexOf(name) < 0)
    {
        emit Abort(tr("exportModeType: Defined export mode is not valid type."));
        return;
    }

    if (!m_photonsSettings)
        m_photonsSettings = new PhotonsSettings;

    m_photonsSettings->name = name;
}

/*!
 * If \a enabled is true, sets to export for each photon the prevous and next photoan id.
 * Otherwise, this information will not be exported.
 */
void MainWindow::SetExportPreviousNextPhotonID(bool enabled)
{
    if (!m_photonsSettings) return;
    m_photonsSettings->savePhotonsID = enabled;
}

/*!
 * Sets to selected export type parameter named \a parameterName the value \a parameterValue.
 */
void MainWindow::SetExportTypeParameterValue(QString name, QString value)
{
    if (!m_photonsSettings) return;
    m_photonsSettings->parameters.insert(name, value);
}

/*!
 * Sets the random number generator type, \a typeName, for ray tracing.
 */
void MainWindow::SetRaysRandomFactory(QString name)
{
//    RandomFactory* f = m_pluginManager->getRandomMap().value(name, 0);
//    if (!f) return;

    QVector<RandomFactory*> factories = m_pluginManager->getRandomFactories();
    if (factories.size() == 0) return;

    QVector< QString > randomNames;
    for (int i = 0; i < factories.size(); i++)
        randomNames << factories[i]->name();

    int oldSelectedRandomDeviate = m_raysRandomFactoryIndex;

    if (randomNames.indexOf(name) < 0)
    {
        emit Abort(tr("SetRandomDeviateType: Defined random generator is not valid type.") );
        return;
    }
    m_raysRandomFactoryIndex = randomNames.indexOf(name);
    if (oldSelectedRandomDeviate != m_raysRandomFactoryIndex)
    {
        delete m_rand;
        m_rand = 0;
    }
}

/*!
 * Sets the ray casting surface grid elemets to \a widthDivisions x \a heightDivisions.
 */
void MainWindow::SetRaysGrid(int width, int height)
{
    m_raysGridWidth = width;
    m_raysGridHeight = height;
}

/*!
 *    Set selected sunshape, \a sunshapeType, to the sun.
 */
void MainWindow::SetSunshape(QString name)
{
    SunFactory* f = m_pluginManager->getSunMap().value(name, 0);
    if (!f)
    {
        emit Abort(tr("SetSunshape: Defined sunshape is not valid type.") );
        return;
    }

    TSceneKit* sceneKit = m_document->getSceneKit();
    SunKit* sunKit = 0;
    if (sceneKit->getPart("lightList[0]", false))
    {
        sunKit = static_cast<SunKit*>(sceneKit->getPart("lightList[0]", false) );
    }
    else
        sunKit = new SunKit;

    sunKit->setPart("tsunshape", f->create() );

    CmdSunModified* command = new CmdSunModified(sunKit, sceneKit, m_modelScene);
    m_undoStack->push(command);

    //UpdateLightDimensions();
    UpdateLightSize();

    ui->parametersTabs->UpdateView();
    setDocumentModified(true);

    ui->actionSceneSunCalculator->setEnabled(true);
}

/*!
 * Set the \a value for the sunshape parameter \a parameter.
 */
void MainWindow::SetSunshapeParameter(QString parameter, QString value)
{
    TSceneKit* sceneKit = m_document->getSceneKit();
    SunKit* sunKit = static_cast<SunKit*>(sceneKit->getPart("lightList[0]", false) );
    if (!sunKit)
    {
        emit Abort(tr("SetSunshapeParameter: There is not light defined.") );
        return;
    }

    SunShape* sunshape = static_cast<SunShape*>(sunKit->getPart("tsunshape", false));
    if (!sunshape)
    {
        emit Abort(tr("SetSunshapeParameter: There is not sunshape defined.") );
        return;
    }

    CmdModifyParameter* cmd = new CmdModifyParameter(sunshape, parameter, value, m_modelScene);
    if (m_undoStack) m_undoStack->push(cmd);
    setDocumentModified(true);
}

/*!
 *    Set selected transmissivity, \a transmissivityType, to the scene.
 */
void MainWindow::SetAir(QString name)
{
    AirFactory* f = m_pluginManager->getAirMap().value(name, 0);
    if (!f)
    {
        emit Abort("SetTransmissivity: Error defining transmissivity.");
        return;
    }

    Air* air = f->create();

    TSceneKit* sceneKit = m_document->getSceneKit();

    CmdAirModified* cmd = new CmdAirModified(air, sceneKit);
    m_undoStack->push(cmd);
    setDocumentModified(true);
}

/*!
 * Set the \a value for the transmissivity parameter \a parameter.
 */
void MainWindow::SetAirParameter(QString parameter, QString value)
{
    TSceneKit* sceneKit = m_document->getSceneKit();
    Air* air = static_cast<Air*>(sceneKit->getPart("air", false));
    if (!air)
    {
        emit Abort("SetTransmissivity: No transmissivity type defined.");
        return;
    }

    CmdModifyParameter* cmd = new CmdModifyParameter(air, parameter, value, m_modelScene);
    if (m_undoStack) m_undoStack->push(cmd);
    setDocumentModified(true);
}

//Manipulators actions
void MainWindow::SoTransform_to_SoCenterballManip()
{
    //Transform to a SoCenterballManip manipulator
    QModelIndex currentIndex = ui->sceneView->currentIndex();

    InstanceNode* instanceNode = m_modelScene->getInstance(currentIndex);
    SoBaseKit* coinNode = static_cast< SoBaseKit* >(instanceNode->getNode() );
    SoTransform* transform = static_cast< SoTransform* >(coinNode->getPart("transform", false) );

    SoCenterballManip* manipulator = new SoCenterballManip;
    manipulator->rotation.setValue(transform->rotation.getValue());
    manipulator->translation.setValue(transform->translation.getValue());
    manipulator->scaleFactor.setValue(transform->scaleFactor.getValue());
    manipulator->scaleOrientation.setValue(transform->scaleOrientation.getValue());
    manipulator->center.setValue(transform->center.getValue());

    coinNode->setPart("transform", manipulator);
    ChangeSelection(currentIndex);

    SoDragger* dragger = manipulator->getDragger();
    dragger->addStartCallback (startManipulator, static_cast< void*>(this) );
    dragger->addFinishCallback(finishManipulator, static_cast< void*>(this) );

    setDocumentModified(true);
}

void MainWindow::SoTransform_to_SoJackManip()
{
    //Transform to a SoJackManip manipulator
    QModelIndex currentIndex = ui->sceneView->currentIndex();

    InstanceNode* instanceNode = m_modelScene->getInstance(currentIndex);
    SoBaseKit* coinNode = static_cast< SoBaseKit* >(instanceNode->getNode() );
    SoTransform* transform = static_cast< SoTransform* >(coinNode->getPart("transform", false) );

    SoJackManip* manipulator = new SoJackManip;
    manipulator->rotation.setValue(transform->rotation.getValue());
    manipulator->translation.setValue(transform->translation.getValue());
    manipulator->scaleFactor.setValue(transform->scaleFactor.getValue());
    manipulator->scaleOrientation.setValue(transform->scaleOrientation.getValue());
    manipulator->center.setValue(transform->center.getValue());

    coinNode->setPart("transform", manipulator);
    ChangeSelection(currentIndex);

    SoDragger* dragger = manipulator->getDragger();
    dragger->addStartCallback (startManipulator, static_cast< void*>(this) );
    dragger->addFinishCallback(finishManipulator, static_cast< void*>(this) );

    setDocumentModified(true);
}

void MainWindow::SoTransform_to_SoHandleBoxManip()
{
    //Transform to a SoHandleBoxManip manipulator
    QModelIndex currentIndex = ui->sceneView->currentIndex();

    InstanceNode* instanceNode = m_modelScene->getInstance(currentIndex);
    SoBaseKit* coinNode = static_cast< SoBaseKit* >(instanceNode->getNode() );
    SoTransform* transform = static_cast< SoTransform* >(coinNode->getPart("transform", false) );

    SoHandleBoxManip* manipulator = new SoHandleBoxManip;
    manipulator->rotation.setValue(transform->rotation.getValue());
    manipulator->translation.setValue(transform->translation.getValue());
    manipulator->scaleFactor.setValue(transform->scaleFactor.getValue());
    manipulator->scaleOrientation.setValue(transform->scaleOrientation.getValue());
    manipulator->center.setValue(transform->center.getValue());

    coinNode->setPart("transform", manipulator);
    ChangeSelection(currentIndex);

    SoDragger* dragger = manipulator->getDragger();
    dragger->addStartCallback (startManipulator, static_cast< void*>(this) );
    dragger->addFinishCallback(finishManipulator, static_cast< void*>(this) );

    setDocumentModified(true);
}

void MainWindow::SoTransform_to_SoTabBoxManip()
{
    //Transform to a SoTabBoxManip manipulator
    QModelIndex currentIndex = ui->sceneView->currentIndex();

    InstanceNode* instanceNode = m_modelScene->getInstance(currentIndex);
    SoBaseKit* coinNode = static_cast< SoBaseKit* >(instanceNode->getNode() );
    SoTransform* transform = static_cast< SoTransform* >(coinNode->getPart("transform", false) );

    SoTabBoxManip* manipulator = new SoTabBoxManip;
    manipulator->rotation.setValue(transform->rotation.getValue());
    manipulator->translation.setValue(transform->translation.getValue());
    manipulator->scaleFactor.setValue(transform->scaleFactor.getValue());
    manipulator->scaleOrientation.setValue(transform->scaleOrientation.getValue());
    manipulator->center.setValue(transform->center.getValue());

    coinNode->setPart("transform", manipulator);
    ChangeSelection(currentIndex);

    SoDragger* dragger = manipulator->getDragger();
    dragger->addStartCallback (startManipulator, static_cast< void*>(this) );
    dragger->addFinishCallback(finishManipulator, static_cast< void*>(this) );

    setDocumentModified(true);
}

void MainWindow::SoTransform_to_SoTrackballManip()
{
    //Transform to a SoTrackballManip manipulator
    QModelIndex currentIndex = ui->sceneView->currentIndex();

    InstanceNode* instanceNode = m_modelScene->getInstance(currentIndex);
    SoBaseKit* coinNode = static_cast< SoBaseKit* >(instanceNode->getNode() );
    SoTransform* transform = static_cast< SoTransform* >(coinNode->getPart("transform", false) );

    SoTrackballManip* manipulator = new SoTrackballManip;
    manipulator->rotation.setValue(transform->rotation.getValue());
    manipulator->translation.setValue(transform->translation.getValue());
    manipulator->scaleFactor.setValue(transform->scaleFactor.getValue());
    manipulator->scaleOrientation.setValue(transform->scaleOrientation.getValue());
    manipulator->center.setValue(transform->center.getValue());

    coinNode->setPart("transform", manipulator);
    ChangeSelection(currentIndex);

    SoDragger* dragger = manipulator->getDragger();
    dragger->addStartCallback (startManipulator, static_cast< void*>(this) );
    dragger->addFinishCallback(finishManipulator, static_cast< void*>(this) );

    setDocumentModified(true);
}

void MainWindow::SoTransform_to_SoTransformBoxManip()
{
    //Transform to a SoTransformBoxManip manipulator
    QModelIndex currentIndex = ui->sceneView->currentIndex();

    InstanceNode* instanceNode = m_modelScene->getInstance(currentIndex);
    SoBaseKit* coinNode = static_cast< SoBaseKit* >(instanceNode->getNode() );
    SoTransform* transform = static_cast< SoTransform* >(coinNode->getPart("transform", false) );

    SoTransformBoxManip* manipulator = new SoTransformBoxManip;
    manipulator->rotation.setValue(transform->rotation.getValue());
    manipulator->translation.setValue(transform->translation.getValue());
    manipulator->scaleFactor.setValue(transform->scaleFactor.getValue());
    manipulator->scaleOrientation.setValue(transform->scaleOrientation.getValue());
    manipulator->center.setValue(transform->center.getValue());

    coinNode->setPart("transform", manipulator);

    ChangeSelection(currentIndex);

    SoDragger* dragger = manipulator->getDragger();
    dragger->addStartCallback (startManipulator, static_cast< void*>(this) );
    dragger->addFinishCallback(finishManipulator, static_cast< void*>(this) );

    setDocumentModified(true);
}

void MainWindow::SoTransform_to_SoTransformerManip()
{
    //Transform to a SoTransformerManip manipulator
    QModelIndex currentIndex = ui->sceneView->currentIndex();

    InstanceNode* instanceNode = m_modelScene->getInstance(currentIndex);
    SoBaseKit* coinNode = static_cast< SoBaseKit* >(instanceNode->getNode() );
    SoTransform* transform = static_cast< SoTransform* >(coinNode->getPart("transform", false) );

    SoTransformerManip* manipulator = new SoTransformerManip;
    manipulator->rotation.setValue(transform->rotation.getValue());
    manipulator->translation.setValue(transform->translation.getValue());
    manipulator->scaleFactor.setValue(transform->scaleFactor.getValue());
    manipulator->scaleOrientation.setValue(transform->scaleOrientation.getValue());
    manipulator->center.setValue(transform->center.getValue());


    coinNode->setPart("transform", manipulator);
    ChangeSelection(currentIndex);

    SoDragger* dragger = manipulator->getDragger();
    dragger->addStartCallback (startManipulator, static_cast< void*>(this) );
    dragger->addFinishCallback(finishManipulator, static_cast< void*>(this) );

    setDocumentModified(true);
}

void MainWindow::SoManip_to_SoTransform()
{
    //Transform manipulator to a SoTransform
    QModelIndex currentIndex = ui->sceneView->currentIndex();

    InstanceNode* instanceNode = m_modelScene->getInstance(currentIndex);
    SoBaseKit* coinNode = static_cast< SoBaseKit* >(instanceNode->getNode() );
    SoTransformManip* manipulator = static_cast< SoTransformManip* >(coinNode->getPart("transform", false) );
    if (!manipulator) return;

    SoTransform* transform = new SoTransform;
    transform->rotation.setValue(manipulator->rotation.getValue());
    transform->translation.setValue(manipulator->translation.getValue());
    transform->scaleFactor.setValue(manipulator->scaleFactor.getValue());
    transform->scaleOrientation.setValue(manipulator->scaleOrientation.getValue());
    transform->center.setValue(manipulator->center.getValue());

    coinNode->setPart("transform", transform);
    ChangeSelection(currentIndex);

    setDocumentModified(true);
}

void MainWindow::ChangeSelection(const QModelIndex& index)
{
    InstanceNode* instance = m_modelScene->getInstance(index);
    SoNode* node = instance->getNode();
    ui->parametersTabs->SelectionChanged(node);
}

/*!
 * Creates a componet subtree from the \a pComponentFactory as current selected node child.
 *
 * If the current node is not a group type node, the component subtree will not be created.
 */
void MainWindow::CreateComponent(ComponentFactory* factory)
{
    QModelIndex parentIndex = ( (!ui->sceneView->currentIndex().isValid() ) || (ui->sceneView->currentIndex() == ui->sceneView->rootIndex() ) ) ?
                m_modelScene->index(0, 0, ui->sceneView->rootIndex()) :
                ui->sceneView->currentIndex();

    InstanceNode* parentInstance = m_modelScene->getInstance(parentIndex);
    SoNode* parentNode = parentInstance->getNode();
    if (!parentNode->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId() ) ) return;

    TSeparatorKit* componentLayout = factory->CreateTComponent(m_pluginManager);
    if (!componentLayout) return;

    QString typeName = factory->name();
    componentLayout->setName(typeName.toStdString().c_str() );

    CmdInsertNode* cmd = new CmdInsertNode(componentLayout, QPersistentModelIndex(parentIndex), m_modelScene);
    QString text = QString("Create Component: %1").arg(factory->name().toLatin1().constData() );
    cmd->setText(text);
    m_undoStack->push(cmd);

    UpdateLightSize();
    setDocumentModified(true);
}

/*!
 * Creates a shape node from the \a pShapeFactory as current selected node child.
 *
 * If the current node is not a surface type node, the shape node will not be created.
 */
void MainWindow::InsertSurface(ShapeFactory* factory)
{
    QModelIndex index = ui->sceneView->currentIndex();
    if (!index.isValid()) return;
    ui->sceneView->expand(index);

    InstanceNode* instance = m_modelScene->getInstance(index);
    SoNode* node = instance->getNode();
    TShapeKit* kit = dynamic_cast<TShapeKit*>(node);
    if (!kit) return;

    ShapeRT* shape = factory->create();
    shape->setName(factory->name().toStdString().c_str());

    CmdInsertSurface* cmd = new CmdInsertSurface(kit, shape, m_modelScene);
    m_undoStack->push(cmd);

//    UpdateLightSize();
    setDocumentModified(true);
}

/*!
 * Creates a shape node from the \a pShapeFactory as current selected node child.
 *
 * If the current node is not a surface type node, the shape node will not be created.
 */
void MainWindow::InsertSurface(ShapeFactory* factory, int /*numberofParameters*/, QVector<QVariant> parametersList)
{
    QModelIndex index = ui->sceneView->currentIndex();
    if (!index.isValid()) return;

    InstanceNode* instance = m_modelScene->getInstance(index);
    SoNode* node = instance->getNode();
    TShapeKit* kit = dynamic_cast<TShapeKit*>(node);
    if (!kit) return;

    ShapeRT* shape = factory->create(parametersList);
    shape->setName(factory->name().toStdString().c_str() );

    CmdInsertSurface* cmd = new CmdInsertSurface(kit, shape, m_modelScene);
    m_undoStack->push(cmd);

//    UpdateLightSize();
    setDocumentModified(true);
}

/*!
 * Creates a material node from the \a pMaterialFactory as current selected node child.
 *
 * If the current node is not a surface type node, the material node will not be created.
 */
void MainWindow::InsertMaterial(MaterialFactory* factory)
{
    QModelIndex index = ui->sceneView->currentIndex();
    if (!index.isValid()) return;
    ui->sceneView->expand(index);

    InstanceNode* instance = m_modelScene->getInstance(index);
    SoNode* node = instance->getNode();
    TShapeKit* kit = dynamic_cast<TShapeKit*>(node);
    if (!kit) return;

    MaterialRT* material = factory->create();
    material->setName(factory->name().toStdString().c_str());

    CmdInsertSurface* cmd = new CmdInsertSurface(kit, material, m_modelScene);
    m_undoStack->push(cmd);

    setDocumentModified(true);
}

void MainWindow::InsertProfile(ProfileFactory* factory)
{
    QModelIndex index = ui->sceneView->currentIndex();
    if (!index.isValid()) return;
    ui->sceneView->expand(index);

    InstanceNode* instance = m_modelScene->getInstance(index);
    SoNode* node = instance->getNode();
    TShapeKit* kit = dynamic_cast<TShapeKit*>(node);
    if (!kit) return;

    ProfileRT* profile = factory->create();
    profile->setName(factory->name().toStdString().c_str());

    CmdInsertSurface* cmd = new CmdInsertSurface(kit, profile, m_modelScene);
    m_undoStack->push(cmd);

//    UpdateLightSize();
    setDocumentModified(true);
}

/*!
 * Creates a shape node from the \a pTrackerFactory as current selected node child.
 *
 */
void MainWindow::InsertTracker(TrackerFactory* factory)
{
    QModelIndex parentIndex = ui->sceneView->currentIndex();
    if (!parentIndex.isValid()) return;

    InstanceNode* parentInstance = m_modelScene->getInstance(parentIndex);
    SoNode* parentNode = parentInstance->getNode();
    TSeparatorKit* kit = dynamic_cast<TSeparatorKit*>(parentNode);
    if (!kit) return;

    Tracker* tracker = (Tracker*) kit->getPart("tracker", false);
    if (tracker)
    {
        ShowWarning("This TSeparatorKit already contains a tracker node");
        return;
    }
    tracker = factory->create();
    tracker->setName(factory->name().toStdString().c_str());

    CmdInsertTracker* cmd = new CmdInsertTracker(tracker, parentIndex, m_modelScene);
    m_undoStack->push(cmd);

    setDocumentModified(true);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (OkToContinue())
    {
        WriteSettings();
        event->accept();
    }
    else
        event->ignore();
}

/*!
 * Receives mouse press event \a e, an sets as active 3D view the view that contains the mouse press position.
 */
void MainWindow::mousePressEvent(QMouseEvent* e)
{
    QPoint pos = e->pos();
    int x = pos.x();
    int y = pos.y() - 64;

    QSplitter* pSplitter = findChild<QSplitter*>("graphicSplitterV");
    QRect mainViewRect = pSplitter->geometry();

    if (mainViewRect.contains(x, y))
    {
        QSplitter* pvSplitter1 = findChild<QSplitter*>("graphicSplitterH1");
        QSplitter* pvSplitter2 = findChild<QSplitter*>("graphicSplitterH1");
        QRect vViewRect1 = pvSplitter1->geometry();
        if (vViewRect1.contains(x,y))
        {
            QList<int> size(pvSplitter2->sizes());
            if (x < size[0])
                m_focusView = 0;
            else
                m_focusView = 1;
        }
        else
        {
            QList<int> size(pvSplitter1->sizes());
            if (x < size[0])
                m_focusView = 2;
            else
                m_focusView = 3;
        }
    }
}

/*!
 * Set current document coin scene into Tonatiuh Models and views.
 */
void MainWindow::ChangeModelScene()
{
    m_graphicsRoot->setDocument(m_document);
    m_modelScene->setDocument(m_document);

    QModelIndex viewLayoutIndex = m_modelScene->indexFromUrl("");
    InstanceNode* viewLayout = m_modelScene->getInstance(viewLayoutIndex);
    ui->sceneView->setRootIndex(viewLayoutIndex);

    InstanceNode* concentratorRoot = viewLayout->children[0];

    m_modelSelection->setCurrentIndex(m_modelScene->indexFromUrl(concentratorRoot->getURL() ), QItemSelectionModel::ClearAndSelect);
}

/*!
 * Shows a dialog to allow define current light position with a position calculator.
 */
void MainWindow::CalculateSunPosition()
{
#ifndef NO_MARBLE

    SoSceneKit* coinScene = m_document->getSceneKit();
    if (!coinScene->getPart("lightList[0]", false) ) return;

    SunCalculatorDialog sunposDialog;
    connect(&sunposDialog, SIGNAL(changeSunLight(double,double)), this, SLOT(ChangeSunPosition(double,double)) );

    sunposDialog.exec();

#endif /* NO_MARBLE*/
}

/*!
 * Creates a \a xDimension by \a zDimension grid and shows in the 3D view. Each grid cell is a \a xSpacing x \a zSpacing.
 */
#include "view/GroundGrid.h"
SoGroup* MainWindow::CreateGrid()
{
    GroundGrid grid;
// todo
    return grid.makeGrid();
}


/*!
 * Creates a export mode object form export mode settings.
 */
PhotonsAbstract* MainWindow::CreatePhotonMapExport() const
{
    PhotonsFactory* f = m_pluginManager->getExportMap().value(m_photonsSettings->name, 0);
    if (!f) return 0;
    PhotonsAbstract* photonExport = f->create();
    if (!photonExport) return 0;

    photonExport->setSceneModel(*m_modelScene);
    photonExport->setPhotonSettings(m_photonsSettings);
    return photonExport;
}

/*!
 * Returns \a true if the application is ready to start with other model. Otherwise,
 * returns \a false.
 */
bool MainWindow::OkToContinue()
{
    if (m_document->isModified() )
    {
        int answer = QMessageBox::warning(this, tr("Tonatiuh"),
                                          tr("The document has been modified.\n"
                                             "Do you want to save your changes?"),
                                          QMessageBox::Yes | QMessageBox::Default,
                                          QMessageBox::No,
                                          QMessageBox::Cancel | QMessageBox::Escape);

        if (answer == QMessageBox::Yes) return FileSave();
        else if (answer == QMessageBox::Cancel) return false;
    }
    return true;
}

/*!
 * Creates a new \a type paste command. The clipboard node is inserted as \a nodeIndex node
 * child.
 *
 * Returns \a true if the node was successfully pasted, otherwise returns \a false.
 */
bool MainWindow::Paste(QModelIndex index, tgc::PasteType type)
{
    if (!index.isValid() ) return false;
    if (!m_coinNode_Buffer) return false;

    InstanceNode* ancestor = m_modelScene->getInstance(index);
    SoNode* selectedCoinNode = ancestor->getNode();

    if (!selectedCoinNode->getTypeId().isDerivedFrom(SoBaseKit::getClassTypeId() ) ) return false;
    if ( (selectedCoinNode->getTypeId().isDerivedFrom(TShapeKit::getClassTypeId() ) ) &&
         (m_coinNode_Buffer->getTypeId().isDerivedFrom(SoBaseKit::getClassTypeId() ) ) ) return false;

    if (ancestor->getNode() == m_coinNode_Buffer) return false;
    while (ancestor->getParent() )
    {
        if (ancestor->getParent()->getNode() == m_coinNode_Buffer) return false;
        ancestor = ancestor->getParent();
    }

    CmdPaste* cmd = new CmdPaste(type, m_modelSelection->currentIndex(), m_coinNode_Buffer, *m_modelScene);
    m_undoStack->push(cmd);

    UpdateLightSize();
    setDocumentModified(true);
    return true;
}

/**
 * Restores application settings.
 **/
void MainWindow::ReadSettings()
{
    QSettings settings("CyI", "Tonatiuh");
    QRect rect = settings.value("geometry", QRect(200, 200, 400, 400)).toRect();
    move(rect.topLeft());
    resize(rect.size());

    setWindowState(Qt::WindowNoState);
    if (settings.value("windowNoState", false).toBool() ) setWindowState(windowState() ^ Qt::WindowNoState);
    if (settings.value("windowMinimized", false).toBool() ) setWindowState(windowState() ^ Qt::WindowMinimized);
    if (settings.value("windowMaximized", true).toBool() ) setWindowState(windowState() ^ Qt::WindowMaximized);
    if (settings.value("windowFullScreen", false).toBool() ) setWindowState(windowState() ^ Qt::WindowFullScreen);
    if (settings.value("windowActive", false).toBool() ) setWindowState(windowState() ^ Qt::WindowActive);

    m_recentFiles = settings.value("recentFiles").toStringList();

    UpdateRecentFileActions();
}

/*!
 * Checks whether a ray tracing can be started with the current light and model.
 */
bool MainWindow::ReadyForRaytracing(InstanceNode*& instanceLayout,
                                    InstanceNode*& instanceSun,
                                    SunShape*& sunShape,
                                    SunAperture*& sunAperture,
                                    Air*& air)
{
    TSceneKit* sceneKit = m_document->getSceneKit();
    if (!sceneKit) return false;

    air = dynamic_cast<Air*>(sceneKit->getPart("air", false));

    InstanceNode* instanceScene = m_modelScene->getInstance(QModelIndex());
    if (!instanceScene) return false;

    instanceSun = instanceScene->children[0];
    if (!instanceSun) return false;

    instanceLayout = instanceScene->children[1];
    if (!instanceLayout) return false;

    if (!sceneKit->getPart("lightList[0]", false)) return false;
    SunKit* sunKit = (SunKit*) sceneKit->getPart("lightList[0]", false);
//    sunKit->updateTransform();

    if (!sunKit->getPart("tsunshape", false)) return false;
    sunShape = (SunShape*) sunKit->getPart("tsunshape", false);

    if (!sunKit->getPart("icon", false)) return false;
    sunAperture = (SunAperture*) sunKit->getPart("icon", false);

    if (!sunKit->getPart("transform", false)) return false;
    SoTransform* sunTransform = (SoTransform*) sunKit->getPart("transform", false);
    instanceSun->setTransform(tgf::makeTransform(sunTransform));

    QVector<RandomFactory*> randomFactories = m_pluginManager->getRandomFactories();
    if (!m_rand) m_rand = randomFactories[m_raysRandomFactoryIndex]->create();


    //Create the photon map where photons are going to be stored
    if (!m_photonBufferAppend)
    {
        delete m_photonsBuffer;
        m_photonsBuffer = new PhotonsBuffer(m_photonBufferSize, m_photonBufferSize);
        m_raysTracedTotal = 0;
    }

    if (!m_photonsBuffer)
    {
        m_photonsBuffer = new PhotonsBuffer(m_photonBufferSize, m_photonBufferSize);
        m_raysTracedTotal = 0;
    }

    return true;
}

/*!
 * Returns \a true if the tonatiuh model is correctly saved into the the given \a fileName. Otherwise, returns \a false.
 *
 * \sa Save, SaveAs.
 */
bool MainWindow::SaveFile(const QString& fileName)
{
    if (!m_document->WriteFile(fileName))
    {
        statusBar()->showMessage(tr("Saving canceled"), 2000);
        return false;
    }

    SetCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

/*!
 * Sets \a fileName files as current file.
 */
void MainWindow::SetCurrentFile(const QString& fileName)
{
    m_currentFile = fileName;
    setDocumentModified(false);

    QString title = "Untitled";
    if (!m_currentFile.isEmpty() )
    {
        title = StrippedName(m_currentFile);
        m_recentFiles.removeAll(m_currentFile);
        m_recentFiles.prepend(m_currentFile);
        UpdateRecentFileActions();
    }

    setWindowTitle(tr("%1[*] - Tonatiuh").arg(title));
}

void MainWindow::SetupActionsInsertComponent()
{
    QMenu* menu = ui->menuInsert->findChild<QMenu*>("menuComponent");
    if (!menu) return;
    if (menu->isEmpty() )
        menu->setEnabled(true);

    if (!m_pluginManager) return;
    QVector<ComponentFactory*> componentFactoryList = m_pluginManager->getComponentFactories();
    if (!(componentFactoryList.size() > 0) ) return;

    for (int i = 0; i < componentFactoryList.size(); ++i)
    {
        ComponentFactory* pComponentFactory = componentFactoryList[i];
        ActionInsert* a = new ActionInsert(pComponentFactory, this);
        menu->addAction(a);
        //m_materialsToolBar->addAction( actionInsertComponent );
        //m_materialsToolBar->addSeparator();
        connect(a, SIGNAL(triggered()),
                a, SLOT(OnActionInsertComponentTriggered()) );
        connect(a, SIGNAL(CreateComponent(ComponentFactory*)),
                this, SLOT(CreateComponent(ComponentFactory*)) );
    }
}

void MainWindow::SetupActionsInsertMaterial()
{
    QMenu* menu = ui->menuInsert->findChild<QMenu*>("menuMaterial");

    for (MaterialFactory* f : m_pluginManager->getMaterialFactories()) {
        ActionInsert* a = new ActionInsert(f, this);
        menu->addAction(a);
        connect(
            a, SIGNAL(InsertMaterial(MaterialFactory*)),
            this, SLOT(InsertMaterial(MaterialFactory*))
        );
    }

    QPushButton* button = new QPushButton;
    button->setIcon(QIcon(":/images/scene/nodeMaterial.png")); // can be static
    button->setToolTip("Materials");
    button->setMenu(menu);
    findChild<QToolBar*>("insertToolBar")->addWidget(button);
}

/**
 * Creates an action for the /a pShapeFactory and adds to shape insert menu and toolbar.
 */
void MainWindow::SetupActionsInsertShape()
{
    QMenu* menu = ui->menuInsert->findChild<QMenu*>("menuShape");

//    QToolBar* toolbar = findChild<QToolBar*>("shapeToolBar");
//    if (!toolbar) {
//        toolbar = new QToolBar(menu);
//        toolbar->setOrientation(Qt::Horizontal);
//        toolbar->setWindowTitle("Shapes");
//        toolbar->setObjectName("shapeToolBar");
//        addToolBar(toolbar);
//    }

    for (ShapeFactory* f : m_pluginManager->getShapeFactories()) {
        if (f) {
            ActionInsert* a = new ActionInsert(f, this);
            menu->addAction(a);
//            toolbar->addAction(a);
            connect(
                a, SIGNAL(InsertSurface(ShapeFactory*)),
                this, SLOT(InsertSurface(ShapeFactory*))
            );
        } else {
            menu->addSeparator();
//            toolbar->addSeparator();
        }
    }

    QPushButton* button = new QPushButton;
    button->setIcon(QIcon(":/images/scene/nodeSurface.png"));
    button->setToolTip("Shapes");
    button->setMenu(menu);
    findChild<QToolBar*>("insertToolBar")->addWidget(button);

    // profiles
    menu = ui->menuInsert->findChild<QMenu*>("menuProfile");

    for (ProfileFactory* f : m_pluginManager->getProfileFactories()) {
        ActionInsert* a = new ActionInsert(f, this);
        menu->addAction(a);
        connect(
            a, SIGNAL(InsertProfile(ProfileFactory*)),
            this, SLOT(InsertProfile(ProfileFactory*))
        );
    }

    button = new QPushButton;
    button->setIcon(QIcon(":/images/scene/nodeProfile.png"));
    button->setToolTip("Profiles");
    button->setMenu(menu);
    findChild<QToolBar*>("insertToolBar")->addWidget(button);
}

void MainWindow::SetupActionsInsertTracker()
{
    QMenu* menu = ui->menuInsert->findChild<QMenu*>("menuTracker");

    for (TrackerFactory* f : m_pluginManager->getTrackerFactories())
    {
        ActionInsert* a = new ActionInsert(f, this);
        menu->addAction(a);
        connect(
            a, SIGNAL(InsertTracker(TrackerFactory*)),
            this, SLOT(InsertTracker(TrackerFactory*))
        );
    }

    QPushButton* button = new QPushButton;
    button->setIcon(QIcon(":/images/scene/nodeTracker.png"));
    button->setToolTip("Trackers");
    button->setMenu(menu);
    findChild<QToolBar*>("insertToolBar")->addWidget(button);
}

/**
 * Action slot to show/hide a grid with the scene dimensions.
 */
void MainWindow::ShowGrid()
{
    m_graphicsRoot->showGrid(ui->actionViewGrid->isChecked());

    /*if( ui->actionViewGrid->isChecked() )
           {
            InstanceNode* sceneInstance = m_sceneModel->NodeFromIndex( ui->sceneView->rootIndex() );
            if ( !sceneInstance )  return;
            SoNode* rootNode = sceneInstance->GetNode();
            SoPath* nodePath = new SoPath( rootNode );
            nodePath->ref();

            SbViewportRegion region = m_graphicView[m_focusView]->GetViewportRegion();
            SoGetBoundingBoxAction* bbAction = new SoGetBoundingBoxAction( region ) ;
            if(nodePath)    bbAction->apply(nodePath);

            SbXfBox3f box= bbAction->getXfBoundingBox();
            delete bbAction;
            nodePath->unref();

            m_gridXElements = 10;
            m_gridZElements = 10;
            m_gridXSpacing = 10;
            m_gridZSpacing = 10;

            if( !box.isEmpty() )
            {
                SbVec3f min, max;
                box.getBounds(min, max);

                m_gridXSpacing = ( 2 *  std::max( fabs( max[0] ), fabs( min[0] ) ) + 5  ) / m_gridXElements;
                m_gridZSpacing = ( 2 *  std::max( fabs( max[2] ), fabs( min[2] ) ) + 5 ) / m_gridZElements;

            }

            m_pGrid->addChild( CreateGrid( m_gridXElements, m_gridZElements, m_gridXSpacing, m_gridZSpacing ) );

           }
           else
            m_pGrid->removeAllChildren();*/

}

/*!
 * Shows the rays and photons stored at the photon map in the 3D view.
 */
void MainWindow::ShowRaysIn3DView()
{
    if (ui->actionViewRays->isChecked() || ui->actionViewPhotons->isChecked())
    {
        trf::DrawRays(m_graphicsRoot->rays(), *m_photonsBuffer, m_raysScreen);
        m_graphicsRoot->showRays(ui->actionViewRays->isChecked());
        m_graphicsRoot->showPhotons(ui->actionViewPhotons->isChecked());
    } else {
        m_graphicsRoot->rays()->removeAllChildren();
    }
}

/*!
 * Returns to the start origin state and starts with a new model defined in \a fileName.
 * If the file name is not defined, it starts with an empty scene.
 */
bool MainWindow::StartOver(const QString& fileName)
{
    InstanceNode* sceneInstance = m_modelScene->getInstance(ui->sceneView->rootIndex() );

    InstanceNode* concentratorRoot = sceneInstance->children[sceneInstance->children.size() - 1];
    m_modelSelection->setCurrentIndex(m_modelScene->indexFromUrl(concentratorRoot->getURL() ), QItemSelectionModel::ClearAndSelect);

//    ui->actionViewRays->setEnabled(false);
//    ui->actionViewRays->setChecked(false);

    m_graphicsRoot->removeScene();
    m_undoStack->clear();
    m_modelScene->clear();

//    SetSunPositionCalculatorEnabled(0);

    if (!fileName.isEmpty() && m_document->ReadFile(fileName) )
    {
        statusBar()->showMessage("File loaded", 2000);
        SetCurrentFile(fileName);
    }
    else
    {
        m_document->New();
        statusBar()->showMessage("New file", 2000);
        SetCurrentFile("");
    }

    ChangeModelScene();
    ui->sceneView->expandToDepth(0);
    Select("//Layout");
    on_actionViewAll_triggered(); // discard sun
    return true;
}

void MainWindow::setDocumentModified(bool value)
{
    m_document->setModified(value);
    setWindowModified(value);
}

/*!
 * Computes the new light size to the scene current dimensions.
 */
void MainWindow::UpdateLightSize()
{
    TSceneKit* sceneKit = m_document->getSceneKit();
    SunKit* sunKit = (SunKit*) sceneKit->getPart("lightList[0]", false);
    if (!sunKit) return;

    sunKit->setBox(sceneKit);
    m_modelScene->UpdateSceneModel();
}

/*!
 * Updates the recently opened files actions list.
 */
void MainWindow::UpdateRecentFileActions()
{
    QMutableStringListIterator iterator(m_recentFiles);
    while (iterator.hasNext())
    {
        if (!QFile::exists(iterator.next()))
            iterator.remove();
    }

    for (int n = 0; n < m_maxRecentFiles; ++n) {
        if (n < m_recentFiles.count()) {
            QString text = tr("&%1 |   %2")
                    .arg(n + 1)
                    .arg(StrippedName(m_recentFiles[n]));
            m_recentFileActions[n]->setText(text);
            m_recentFileActions[n]->setData(m_recentFiles[n]);
            m_recentFileActions[n]->setVisible(true);
        } else
            m_recentFileActions[n]->setVisible(false);
    }
}

/*!
 * Saves application settings.
 */
void MainWindow::WriteSettings()
{
    QSettings settings("CyI", "Tonatiuh");
    settings.setValue("geometry", geometry());

    Qt::WindowStates states = windowState();
    if (states.testFlag(Qt::WindowNoState) ) settings.setValue("windowNoState", true);
    else settings.setValue("windowNoState", false);
    if (states.testFlag(Qt::WindowMinimized) ) settings.setValue("windowMinimized", true);
    else settings.setValue("windowMinimized", false);
    if (states.testFlag(Qt::WindowMaximized) ) settings.setValue("windowMaximized", true);
    else settings.setValue("windowMaximized", false);
    if (states.testFlag(Qt::WindowFullScreen) ) settings.setValue("windowFullScreen", true);
    else settings.setValue("windowFullScreen", false);
    if (states.testFlag(Qt::WindowActive) ) settings.setValue("windowActive", true);
    else settings.setValue("windowActive", false);

    settings.setValue("recentFiles", m_recentFiles);
}

void MainWindow::on_actionViewAll_triggered()
{
    SbViewportRegion vpr = m_graphicView[m_focusView]->GetViewportRegion();
    SoCamera* camera = m_graphicView[m_focusView]->getCamera();
    camera->viewAll(m_graphicsRoot->getRoot(), vpr);
}

void MainWindow::on_actionViewSelected_triggered()
{
    QModelIndex index = m_modelSelection->currentIndex();
    InstanceNode* node = m_modelScene->getInstance(index);

    SoGetBoundingBoxAction* action = new SoGetBoundingBoxAction( SbViewportRegion() ) ;
    node->getNode()->getBoundingBox(action);
    SbBox3f box = action->getBoundingBox();
    delete action;
    if ( box.isEmpty() ) return;

    SoCamera* camera = m_graphicView[m_focusView]->getCamera();
    camera->pointAt(box.getCenter(), SbVec3f(0., 0., 1.));
}

void MainWindow::on_actionViewTop_triggered()
{
    SoCamera* camera = m_graphicView[m_focusView]->getCamera();
    SbVec3f target = getTarget(camera);

    camera->position = target + SbVec3f(0, 0, camera->focalDistance.getValue());
    camera->pointAt(target, SbVec3f(0., 1., 0.) );
}

void MainWindow::on_actionLookNorth_triggered()
{
    SoCamera* camera = m_graphicView[m_focusView]->getCamera();
    SbVec3f target = getTarget(camera);

    camera->position = target + SbVec3f(0., -camera->focalDistance.getValue(), 0.);
    camera->pointAt(target, SbVec3f(0., 0., 1.));
}

void MainWindow::on_actionLookEast_triggered()
{
    SoCamera* camera = m_graphicView[m_focusView]->getCamera();
    SbVec3f target = getTarget(camera);

    camera->position = target + SbVec3f(-camera->focalDistance.getValue(), 0., 0.);
    camera->pointAt(target, SbVec3f(0., 0., 1.));
}

void MainWindow::on_actionLookSouth_triggered()
{
    SoCamera* camera = m_graphicView[m_focusView]->getCamera();
    SbVec3f target = getTarget(camera);

    camera->position = target + SbVec3f(0., camera->focalDistance.getValue(), 0.);
    camera->pointAt(target, SbVec3f(0., 0., 1.));
}

void MainWindow::on_actionLookWest_triggered()
{
    SoCamera* camera = m_graphicView[m_focusView]->getCamera();
    SbVec3f target = getTarget(camera);

    camera->position = target + SbVec3f(camera->focalDistance.getValue(), 0., 0.);
    camera->pointAt(target, SbVec3f(0., 0., 1.));
}

void MainWindow::on_actionViewSun_triggered()
{
    SoSceneKit* sceneKit = m_document->getSceneKit();
    if (!sceneKit) return;
    SunKit* lightKit = static_cast<SunKit*>(sceneKit->getPart("lightList[0]", false));
    if (!lightKit) return;
    SoTransform* transform = (SoTransform*)lightKit->getPart("transform", false);

    SoCamera* camera = m_graphicView[m_focusView]->getCamera();
    SbVec3f target = getTarget(camera);

    SbVec3f shift;
    transform->rotation.getValue().multVec(SbVec3f(0., 0., -camera->focalDistance.getValue()), shift);

    camera->position = target + shift;
    camera->pointAt(target, SbVec3f(0., 0., 1.));
}
