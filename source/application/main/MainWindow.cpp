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

#include "libraries/geometry/gcf.h"
#include "libraries/geometry/gcf.h"
#include "libraries/geometry/Ray.h"
#include "libraries/geometry/Transform.h"

#include "commands/ActionInsert.h"
#include "commands/CmdChangeNodeName.h"
#include "commands/CmdCopy.h"
#include "commands/CmdCut.h"
#include "commands/CmdDelete.h"
#include "commands/CmdDeleteTracker.h"
#include "commands/CmdInsertSeparatorKit.h"
#include "commands/CmdInsertShape.h"
#include "commands/CmdInsertShapeKit.h"
#include "commands/CmdInsertTracker.h"
#include "commands/CmdLightKitModified.h"
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
#include "kernel/photons/Photons.h"
#include "kernel/photons/PhotonsAbstract.h"
#include "kernel/photons/PhotonsFactory.h"
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
#include "run/RayExportDialog.h"
#include "run/RayOptionsDialog.h"
#include "script/ScriptEditorDialog.h"
#include "tree/SceneModel.h"
#include "view/GraphicRoot.h"
#include "view/GraphicView.h"
#include "widgets/AboutDialog.h"
#include "widgets/AirDialog.h"
#include "widgets/GridDialog.h"
#include "widgets/NetworkConnectionsDialog.h"
#include "widgets/SunDialog.h"

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
    m_commandStack(0),
    m_commandView(0),
    m_currentFile(""),
    m_document(0),
    m_sceneModel(0),
    m_selectionModel(0),
    m_rand(0),
    m_selectedRandomDeviate(-1),
    m_bufferPhotons(1'000'000),
    m_increasePhotonMap(false),
    m_photonsSettings(0),
    m_photons(0),
    m_lastExportFileName(""),
    m_lastExportSurfaceUrl(""),
    m_lastExportInGlobal(true),
    m_graphicsRoot(0),
    m_coinNode_Buffer(0),
    m_manipulators_Buffer(0),
    m_raysTracedTotal(0),
    m_raysTraced(10000),
    m_widthDivisions(200),
    m_heightDivisions(200),
    m_drawPhotons(false),
    m_drawRays(true),
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
    SetupGraphicsRoot();
    SetupModels();
    SetupViews();
    SetupPluginsManager();
    SetupTriggers();

    ReadSettings();

    if (splash) splash->showMessage("Opening file", splashAlignment);
    if (!tonatiuhFile.isEmpty() )
        StartOver(tonatiuhFile);
    else
        SetCurrentFile("");

    SelectNode("//Layout");

    ui->fileToolBar->hide();
    ui->editToolBar->hide();

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

/*!
 * Destroys MainWindow object.
 */
MainWindow::~MainWindow()
{
    delete ui;
    delete m_commandView;
    delete m_pluginManager;
    delete m_sceneModel;
    delete m_document;
    delete m_commandStack;
    delete m_rand;
    delete m_photons;
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
            this, SLOT(OpenRecentFile())
        );
        m_recentFileActions << a;
        ui->menuRecent->addAction(a);
    }
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
}

/*!
 * Defines 3D view background.
 */
void MainWindow::SetupGraphicsRoot()
{
    m_graphicsRoot = new GraphicRoot;
    m_graphicsRoot->AddModel(m_document->getSceneKit());

    connect(
        m_graphicsRoot, SIGNAL(ChangeSelection(SoSelection*)),
        this, SLOT(SelectionFinish(SoSelection*))
    );

    m_graphicsRoot->AddGrid(CreateGrid());
}

/*!
 * Initializes Tonatiuh models.
 */
void MainWindow::SetupModels()
{
    m_sceneModel = new SceneModel();
    m_sceneModel->setSceneRoot(*m_graphicsRoot->GetNode() );
    m_sceneModel->setSceneKit(*m_document->getSceneKit() );

    m_selectionModel = new QItemSelectionModel(m_sceneModel);
}

/*!
 * Starts MainWindow views.
 */
void MainWindow::SetupViews()
{
    SetupCommandView();
    SetupGraphicView();
    SetupTreeView();
    SetupParametersView();
}

/*!
 * Creates a view for visualize user done last actions.
 */
void MainWindow::SetupCommandView()
{
    m_commandStack = new QUndoStack(this);

    m_commandView = new QUndoView(m_commandStack);
    m_commandView->setWindowTitle("Command List");
    m_commandView->setAttribute(Qt::WA_QuitOnClose, false);
    int q = fontMetrics().height();
    m_commandView->resize(24*q, 16*q);

    connect(m_commandStack, SIGNAL(canRedoChanged(bool)),
            ui->actionRedo, SLOT(setEnabled(bool)) );
    connect(m_commandStack, SIGNAL(canUndoChanged(bool)),
            ui->actionUndo, SLOT(setEnabled(bool)) );
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
        m_graphicView[n]->SetSceneGraph(m_graphicsRoot);
        m_graphicView[n]->setModel(m_sceneModel);
        m_graphicView[n]->setSelectionModel(m_selectionModel);
        if (n > 0) m_graphicView[n]->ViewDecoration(false);
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
    ui->sceneModelView->setModel(m_sceneModel);
    ui->sceneModelView->setSelectionModel(m_selectionModel);
    ui->sceneModelView->setRootIndex(m_sceneModel->IndexFromUrl(""));

    connect(ui->sceneModelView, SIGNAL(dragAndDrop(const QModelIndex&,const QModelIndex&)),
            this, SLOT(ItemDragAndDrop(const QModelIndex&,const QModelIndex&)) );
    connect(ui->sceneModelView, SIGNAL(dragAndDropCopy(const QModelIndex&,const QModelIndex&)),
            this, SLOT(ItemDragAndDropCopy(const QModelIndex&,const QModelIndex&)) );
    connect(ui->sceneModelView, SIGNAL(showMenu(const QModelIndex&)),
            this, SLOT(ShowMenu(const QModelIndex&)) );
    connect(ui->sceneModelView, SIGNAL(nodeNameModificated(const QModelIndex&,const QString&)),
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
        m_selectionModel, SIGNAL(currentChanged (const QModelIndex&,const QModelIndex&)),
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
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(New()) );
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(Open()) );
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(Save()) );
    connect(ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(SaveAs()) );
    connect(ui->actionSaveComponent, SIGNAL(triggered()), this, SLOT(SaveComponent()) );
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(close()) );

    // edit
    connect(ui->actionUndo, SIGNAL(triggered()), this, SLOT(Undo()) );
    connect(ui->actionRedo, SIGNAL(triggered()), this, SLOT(Redo()) );
    connect(ui->actionUndoView, SIGNAL(triggered()), this, SLOT(ShowCommandView()) );
    connect(ui->actionCut, SIGNAL(triggered()), this, SLOT(Cut()) );
    connect(ui->actionCopy, SIGNAL(triggered()), this, SLOT(Copy()) );
    connect(ui->actionPasteCopy, SIGNAL(triggered()), this, SLOT(PasteCopy()) );
    connect(ui->actionPasteLink, SIGNAL(triggered()), this, SLOT(PasteLink()) );
    connect(ui->actionDelete, SIGNAL(triggered()), this, SLOT(Delete()) );

    // insert
    connect(ui->actionNode, SIGNAL(triggered()), this, SLOT(CreateGroupNode()) );
    connect(ui->actionSurfaceNode, SIGNAL(triggered()), this, SLOT(CreateSurfaceNode()) );
    connect(ui->actionUserComponent, SIGNAL(triggered()), this, SLOT(InsertUserDefinedComponent()) );

    // scene
    connect(ui->actionDefineSunLight, SIGNAL(triggered()), this, SLOT(onSunDialog()) );
    connect(ui->actionCalculateSunPosition, SIGNAL(triggered()), this, SLOT(CalculateSunPosition()) );
    connect(ui->actionDefineTransmissivity, SIGNAL(triggered()), this, SLOT(onAirDialog()) );

    // run
    connect(ui->actionRun, SIGNAL(triggered()), this, SLOT(RunCompleteRayTracer()) );
    connect(ui->actionRunFluxAnalysis, SIGNAL(triggered()), this, SLOT(RunFluxAnalysisRayTracer()) );
    connect(ui->actionRayTraceOptions, SIGNAL(triggered()), this, SLOT(onRayOptionsDialog())  );

    // view
    connect(ui->actionViewRays, SIGNAL(toggled(bool)), this, SLOT(DisplayRays(bool)) );
    connect(ui->actionViewGrid, SIGNAL(triggered()), this, SLOT(ShowGrid())  );
    connect(ui->actionGridSettings, SIGNAL(triggered()), this, SLOT(ChangeGridSettings())  );
    connect(ui->actionViewBackground, SIGNAL(triggered()), this, SLOT(ShowBackground())  );
}

/*!
 * Finish the manipulation of the current selected node.
 */
void MainWindow::FinishManipulation()
{
    QModelIndex currentIndex = ui->sceneModelView->currentIndex();
    SoBaseKit* coinNode = static_cast< SoBaseKit* >(m_sceneModel->getInstance(currentIndex)->getNode() );

    SoTransform* nodeTransform = static_cast< SoTransform* >(coinNode->getPart("transform", true) );

    QUndoCommand* command = new QUndoCommand();

    QString translationValue = QString("%1 %2 %3").arg(
        QString::number(nodeTransform->translation.getValue()[0]),
        QString::number(nodeTransform->translation.getValue()[1]),
        QString::number(nodeTransform->translation.getValue()[2])
    );
    new CmdModifyParameter(nodeTransform, "translation", translationValue, m_sceneModel, command);
    m_commandStack->push(command);

    UpdateLightSize();
    setDocumentModified(true);
}

void MainWindow::ExecuteScriptFile(QString tonatiuhScriptFile)
{
    QVector<RandomFactory*> randomDeviateFactoryList = m_pluginManager->getRandomFactories();
    ScriptEditorDialog editor(randomDeviateFactoryList, this);
    editor.show();
    editor.ExecuteScript(tonatiuhScriptFile);
    editor.done(0);
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

    coinSearch.apply(m_graphicsRoot->GetNode() );

    SoPath* coinScenePath = coinSearch.getPath();
    if (!coinScenePath) gcf::SevereError("PathFromIndex Null coinScenePath.");

    SoNodeKitPath* nodePath = static_cast< SoNodeKitPath* > (coinScenePath);
    if (!nodePath) gcf::SevereError("PathFromIndex Null nodePath.");


    nodePath->truncate(nodePath->getLength() - 1);
    SoBaseKit* coinNode = static_cast<SoBaseKit*> (nodePath->getTail() );

    QModelIndex nodeIndex = m_sceneModel->IndexFromPath(*nodePath);
    m_selectionModel->setCurrentIndex(nodeIndex, QItemSelectionModel::ClearAndSelect);

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

    SunDialog dialog(m_sceneModel, sunKitOld, m_pluginManager->getSunMap(), this);
    if (!dialog.exec()) return;

    SunKit* sunKit = dialog.getSunKit();
    if (!sunKit) return;

    CmdSunKitModified* cmd = new CmdSunKitModified(sunKit, sceneKit, m_sceneModel);
    m_commandStack->push(cmd);

    sceneKit->updateTrackers();
    UpdateLightSize();

    ui->parametersTabs->UpdateView();
    setDocumentModified(true);

    ui->actionViewRays->setEnabled(false);
    ui->actionViewRays->setChecked(false);
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
    if (m_commandStack) m_commandStack->push(cmd);
    setDocumentModified(true);
}

/*!
 * If actionDisplay_rays is checked the 3D view shows rays representation. Otherwise the representation is hidden.
 */
void MainWindow::DisplayRays(bool display)
{
    m_graphicsRoot->ShowRays(display);
    /*if( display && ( m_pRays ) )
            m_graphicsRoot->insertChild( m_pRays, 0 );
           else if( !display )
            if ( m_pRays->getRefCount( ) > 0 )    m_graphicsRoot->removeChild( 0 );*/
}

/*!
 * Inserts an existing tonatiuh component into the tonatiuh model as a selected node child.
 *
 * A open file dialog is opened to select the file where the existing component is saved.
 */
void MainWindow::InsertUserDefinedComponent()
{
    QModelIndex parentIndex;
    if ((!ui->sceneModelView->currentIndex().isValid() ) || (ui->sceneModelView->currentIndex() == ui->sceneModelView->rootIndex() ) )
        parentIndex = m_sceneModel->index (0,0,ui->sceneModelView->rootIndex());
    else
        parentIndex = ui->sceneModelView->currentIndex();

    SoNode* coinNode = m_sceneModel->getInstance(parentIndex)->getNode();

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
    if (node == ui->sceneModelView->rootIndex() ) return;

    InstanceNode* nodeInstnace = m_sceneModel->getInstance(node);
    if (nodeInstnace->getParent()&&nodeInstnace->getParent()->getNode()->getTypeId().isDerivedFrom(SoBaseKit::getClassTypeId() ) )
    {
        SoNode* coinNode = nodeInstnace->getNode();
        //if( coinNode->getTypeId().isDerivedFrom( TTracker::getClassTypeId() ) ) return;

        QUndoCommand* dragAndDrop = new QUndoCommand();
        dragAndDrop->setText("Drag and Drop node");
        new CmdCut(node, m_coinNode_Buffer, m_sceneModel, dragAndDrop);

        new CmdPaste(tgc::Copied, newParent, coinNode, *m_sceneModel, dragAndDrop);
        m_commandStack->push(dragAndDrop);

        UpdateLightSize();
        setDocumentModified(true);
    }
}

/*!
 * Inserts a copy of the node \a node as a \a newParent child.
 */
void MainWindow::ItemDragAndDropCopy(const QModelIndex& newParent, const QModelIndex& node)
{
    InstanceNode* nodeInstnace = m_sceneModel->getInstance(node);
    SoNode* coinNode = nodeInstnace->getNode();
    //if( coinNode->getTypeId().isDerivedFrom( TTracker::getClassTypeId() ) ) return;

    QUndoCommand* dragAndDropCopy = new QUndoCommand();
    dragAndDropCopy->setText("Drag and Drop Copy");
    new CmdCopy(node, m_coinNode_Buffer, m_sceneModel);
    new CmdPaste(tgc::Shared, newParent, coinNode, *m_sceneModel, dragAndDropCopy);
    m_commandStack->push(dragAndDropCopy);

    UpdateLightSize();
    setDocumentModified(true);
}

/*!
 * Shows a dialog to the user to open a existing tonatiuh file.
 */
void MainWindow::Open()
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
void MainWindow::OpenRecentFile()
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
    m_commandStack->redo();
    UpdateLightSize();
}

/*!
 * Reverts a change to Tonatiuh model. The model is returne to the previous state before the command is applied.
 */
void MainWindow::Undo()
{
    m_commandStack->undo();
    UpdateLightSize();
}

/*!
 * Runs complete ray tracer. First defines export settings if they are not defined and then runs ray tracer.
 */
void MainWindow::RunCompleteRayTracer()
{
    InstanceNode* rootSeparatorInstance = 0;
    InstanceNode* lightInstance = 0;
    SunShape* sunShape = 0;
    SunAperture* sunAperture = 0;
    Air* transmissivity = 0;

//    QElapsedTimer timer;
//    timer.start();

    if (!ReadyForRaytracing(rootSeparatorInstance, lightInstance, sunShape, sunAperture, transmissivity) )
        return;

//    std::cout << "Elapsed time (ReadyForRaytracing): " << timer.elapsed() << std::endl;

    if (!m_photons->getExporter() && !SetPhotonMapExportSettings() ) return;

    Run();
}

/*!
 * Runs the ray tracer for the analysis of the surface. A dialog will be opened to set the surface and flux calculation parameters.
 */
void MainWindow::RunFluxAnalysisRayTracer()
{
    TSceneKit* sceneKit = m_document->getSceneKit();
    if (!sceneKit) return;

    SunKit* sunKit = (SunKit*) sceneKit->getPart("lightList[0]", false);
    if (!sunKit) return;

    InstanceNode* rootSeparatorInstance = m_sceneModel->getInstance(QModelIndex());
    if (!rootSeparatorInstance) return;
    rootSeparatorInstance = rootSeparatorInstance->children[1];

    QVector<RandomFactory*> randomDeviateFactoryList = m_pluginManager->getRandomFactories();
    //Check if there is a random generator selected;
    if (m_selectedRandomDeviate == -1)
    {
        if (randomDeviateFactoryList.size() > 0) m_selectedRandomDeviate = 0;
        else return;
    }

    //Create the random generator
    Random* pRandomDeviate = randomDeviateFactoryList[m_selectedRandomDeviate]->create();

    FluxAnalysisDialog dialog(sceneKit, *m_sceneModel, rootSeparatorInstance, m_widthDivisions, m_heightDivisions, pRandomDeviate, this);
    dialog.exec();
}

/*!
 * Returns \a true if the tonatiuh model is correctly saved in the current file. Otherwise, returns \a false.
 *
 * If a current file is not defined, it calls to SaveAs funtios.
 *
 * \sa SaveAs, SaveFile.
 */
bool MainWindow::Save()
{
    if (m_currentFile.isEmpty() ) return SaveAs();
    else return SaveFile(m_currentFile);
}

/*!
 * Saves current selected node as a component in the files named \a componentFileName.
 */
void MainWindow::SaveComponent(QString componentFileName)
{
    if (!m_selectionModel->hasSelection() ) return;
    if (m_selectionModel->currentIndex() == ui->sceneModelView->rootIndex() ) return;

    QModelIndex componentIndex = ui->sceneModelView->currentIndex();
    SoNode* coinNode = m_sceneModel->getInstance(componentIndex)->getNode();

    if (!coinNode->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId() ) )
    {
        QMessageBox::warning(0, tr("Tonatiuh"),
                             tr("Selected node in not valid  for component node") );
        return;
    }

    TSeparatorKit* componentRoot = dynamic_cast< TSeparatorKit* > (coinNode);
    if (!componentRoot) return;

    if (componentFileName.isEmpty() ) return;

    QFileInfo componentFile(componentFileName);
    if (componentFile.completeSuffix().compare("tcmp") ) componentFileName.append(".tcmp");

    SoWriteAction SceneOuput;
    if (!SceneOuput.getOutput()->openFile(componentFileName.toLatin1().constData() ) )
    {
        QMessageBox::warning(0, tr("Tonatiuh"),
                             tr("Cannot open file %1. ")
                             .arg(componentFileName));
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
bool MainWindow::SaveAs()
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
    if (!m_selectionModel->hasSelection() ) return false;
    if (m_selectionModel->currentIndex() == ui->sceneModelView->rootIndex() ) return false;

    QModelIndex componentIndex = ui->sceneModelView->currentIndex();

    SoNode* coinNode = m_sceneModel->getInstance(componentIndex)->getNode();

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

    SoPath* selectionPath = selection->getPath(0);
    if (!selectionPath) return;

    if (!selectionPath->containsNode (m_document->getSceneKit() ) ) return;

    SoNodeKitPath* nodeKitPath = static_cast< SoNodeKitPath* >(selectionPath);
    if (nodeKitPath->getTail()->getTypeId().isDerivedFrom(SunKit::getClassTypeId() ) )
    {
        selection->deselectAll();
        QModelIndex currentIndex = m_selectionModel->currentIndex();
        m_selectionModel->setCurrentIndex(currentIndex, QItemSelectionModel::ClearAndSelect);
        return;
    }

    if (nodeKitPath->getTail()->getTypeId().isDerivedFrom(SoDragger::getClassTypeId() ) ) return;

    QModelIndex nodeIndex = m_sceneModel->IndexFromPath(*nodeKitPath);


    if (!nodeIndex.isValid() ) return;
    m_selectionModel->setCurrentIndex(nodeIndex, QItemSelectionModel::ClearAndSelect);
    m_selectionModel->select(nodeIndex, QItemSelectionModel::ClearAndSelect);
}

/*!
 * Changes to the node defined by \a node the value of the \a parameter to \a value.
 */
void MainWindow::SetParameterValue(SoNode* node, QString name, QString value)
{
    CmdModifyParameter* cmd = new CmdModifyParameter(node, name, value, m_sceneModel);
    if (m_commandStack) m_commandStack->push(cmd);
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
    ui->actionCalculateSunPosition->setEnabled(enabled);
}

/*!
 * Shows a windows with the applied commands.
 * The most recently executed command is always selected.
 * When a different command is selected the model returns to the state after selected command was applied.
 */
void MainWindow::ShowCommandView()
{
    m_commandView->show();
}

/*!
 * Shows selected node right menu.
 */
void MainWindow::ShowMenu(const QModelIndex& index)
{
    if (!index.isValid()) return;
    m_selectionModel->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);

    InstanceNode* instanceNode = m_sceneModel->getInstance(index);
    SoNode* coinNode = instanceNode->getNode();
    SoType type = coinNode->getTypeId();

    QMenu popupmenu(this);

    if (instanceNode->getParent() && instanceNode->getParent()->getNode()->getTypeId().isDerivedFrom(SoBaseKit::getClassTypeId() ) )
    {
        popupmenu.addAction(ui->actionCut);
        popupmenu.addAction(ui->actionCopy);
        popupmenu.addAction(ui->actionPasteCopy);
        popupmenu.addAction(ui->actionPasteLink);
        popupmenu.addAction(ui->actionDelete);
        popupmenu.addAction("Collapse all", ui->sceneModelView, SLOT(collapseAll()));
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

/*!
 * Shows a dialog with ray tracer options and modifies the ray tracer parameters if changes are done.
 */
void MainWindow::onRayOptionsDialog()
{
    QVector<RandomFactory*> randomDeviateFactoryList = m_pluginManager->getRandomFactories();
    RayOptionsDialog* options = new RayOptionsDialog(
        m_raysTraced,
        randomDeviateFactoryList, m_selectedRandomDeviate,
        m_widthDivisions, m_heightDivisions,
        m_drawRays, m_drawPhotons,
        m_bufferPhotons, m_increasePhotonMap, this);
    options->exec();

    SetRaysPerIteration(options->GetNumRays());
    SetRandomDeviateType(randomDeviateFactoryList[options->GetRandomFactoryIndex()]->name() );
    SetRayCastingGrid(options->GetWidthDivisions(), options->GetHeightDivisions() );
    SetRaysDrawingOptions(options->DrawRays(), options->DrawPhotons() );
    SetPhotonMapBufferSize(options->GetPhotonMapBufferSize() );
    SetIncreasePhotonMap(options->IncreasePhotonMap() );
}

void MainWindow::ShowWarning(QString message)
{
    QMessageBox::warning(this, "Tonatiuh", message);
}

//View menu actions
void MainWindow::on_actionViewAxes_toggled()
{
    m_graphicView[0]->ViewCoordinateSystem(ui->actionViewAxes->isChecked() );
    m_graphicView[1]->ViewCoordinateSystem(ui->actionViewAxes->isChecked() );
    m_graphicView[2]->ViewCoordinateSystem(ui->actionViewAxes->isChecked() );
    m_graphicView[3]->ViewCoordinateSystem(ui->actionViewAxes->isChecked() );
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

void MainWindow::on_actionOpenScriptEditor_triggered()
{
    QVector<RandomFactory*> randomDeviateFactoryList = m_pluginManager->getRandomFactories();
    ScriptEditorDialog editor(randomDeviateFactoryList, this);
    editor.exec();
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
//        InstanceNode* sceneInstance = m_sceneModel->NodeFromIndex(ui->sceneModelView->rootIndex() );
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

    InstanceNode* instance = m_sceneModel->getInstance(index);
    if (!instance) return;
    if (!instance->getNode()) return;

    CmdChangeNodeName* cmd = new CmdChangeNodeName(index, name, m_sceneModel);
    m_commandStack->push(cmd);

    setDocumentModified(true);
}

/*!
 * Adds a surface to the export sufaces list.
 */
void MainWindow::AddExportSurfaceURL(QString nodeURL)
{
    if (!m_photonsSettings) return;
    m_photonsSettings->exportSurfaceNodeList.push_back(nodeURL);
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
    m_commandStack->push(command);

    //UpdateLightDimensions();
    UpdateLightSize();
    setDocumentModified(true);

    ui->actionViewRays->setEnabled(false);
    ui->actionViewRays->setChecked(false);
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

    ui->actionViewRays->setEnabled(false);
    ui->actionViewRays->setChecked(false);
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
    if (!m_selectionModel->hasSelection() )
    {
        emit Abort(tr("Copy: No node seleted to copy.") );
        return;
    }
    if (m_selectionModel->currentIndex() == ui->sceneModelView->rootIndex() )
    {
        emit Abort(tr("Copy: The root node can not bee copied.") );
        return;
    }
    if (m_selectionModel->currentIndex().parent() == ui->sceneModelView->rootIndex() )
    {
        emit Abort(tr("Copy: The root node can not bee copied.") );
        return;
    }

    CmdCopy* command = new CmdCopy(m_selectionModel->currentIndex(), m_coinNode_Buffer, m_sceneModel);
    m_commandStack->push(command);

    setDocumentModified(true);
}

/*!
 * Copies the node defined with the \a nodeURL to the clipboard.
 *
 * The current node cannot be the model root node or concentrator node.
 */
void MainWindow::Copy(QString nodeURL)
{
    QModelIndex nodeIndex = m_sceneModel->IndexFromUrl(nodeURL);

    if (!nodeIndex.isValid() )
    {
        emit Abort(tr("Copy: There is no node with defined url.") );
        return;
    }
    if (nodeIndex == ui->sceneModelView->rootIndex() || nodeIndex.parent() == ui->sceneModelView->rootIndex() )
    {
        emit Abort(tr("Copy: The root node can not bee copied.") );
        return;
    }

    CmdCopy* command = new CmdCopy(nodeIndex, m_coinNode_Buffer, m_sceneModel);
    m_commandStack->push(command);

    setDocumentModified(true);
}

/*!
 * Creates a new group node as a selected node child.
 */
void MainWindow::CreateGroupNode()
{
    QModelIndex index = ui->sceneModelView->currentIndex();
    if (!index.isValid()) return;
    ui->sceneModelView->expand(index);

    InstanceNode* instance = m_sceneModel->getInstance(index);
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

    TSeparatorKit* kit = new TSeparatorKit();
    CmdInsertSeparatorKit* cmd = new CmdInsertSeparatorKit(kit, QPersistentModelIndex(index), m_sceneModel);
    m_commandStack->push(cmd);

    QString name("Node");
    int count = 0;
    while (!m_sceneModel->SetNodeName(kit, name))
        name = QString("Node_%1").arg(++count);

    setDocumentModified(true);
}

/*!
 * Creates a surface node as selected node child.
 */
void MainWindow::CreateSurfaceNode()
{
    QModelIndex index = ui->sceneModelView->currentIndex();
    if (!index.isValid()) return;
    ui->sceneModelView->expand(index);

    InstanceNode* instance = m_sceneModel->getInstance(index);
    if (!instance) return;
    SoNode* node = instance->getNode();
    if (!node) return;
    if (!node->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId()))
        return;

    TShapeKit* kit = new TShapeKit;
    CmdInsertShapeKit* cmd = new CmdInsertShapeKit(kit, index, m_sceneModel);
    m_commandStack->push(cmd);

    QString name("Shape");
    int count = 0;
    while (!m_sceneModel->SetNodeName(kit, name))
        name = QString("Shape_%1").arg(++count);

    SelectNode(instance->getURL() + "/" + name);
    setDocumentModified(true);
}

/*!
 * Creates a \a componentType component node with the name \a nodeName.
 */
void MainWindow::CreateComponentNode(QString componentType, QString nodeName, int numberofParameters, QVector<QVariant> parametersList)
{
    QModelIndex parentIndex = ui->sceneModelView->currentIndex();
    if (!parentIndex.isValid()) return;

    InstanceNode* parentInstance = m_sceneModel->getInstance(parentIndex);
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

    CmdInsertSeparatorKit* cmd = new CmdInsertSeparatorKit(componentLayout, QPersistentModelIndex(parentIndex), m_sceneModel);
    QString commandText = QString("Create Component: %1").arg(pComponentFactory->name().toLatin1().constData() );
    cmd->setText(commandText);
    m_commandStack->push(cmd);

    UpdateLightSize();
    setDocumentModified(true);
}

/*!
 * Creates a \a materialType material node from the as current selected node child.
 *
 * If the current node is not a surface type node or \a materialType is not a valid type, the material node will not be created.
 */
void MainWindow::CreateMaterial(QString name)
{
    MaterialFactory* f = m_pluginManager->getMaterialMap().value(name, 0);
    if (f)
        CreateMaterial(f);
    else
        emit Abort("CreateMaterial: Material not found");
}

/*!
 * Creates a \a shapeType shape node from the as current selected node child.
 *
 * If the current node is not a valid parent node or \a shapeType is not a valid type, the shape node will not be created.
 *
 */
void MainWindow::CreateShape(QString name)
{
    ShapeFactory* f = m_pluginManager->getShapeMap().value(name, 0);
    if (f)
        CreateShape(f);
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
void MainWindow::CreateShape(QString name, int numberOfParameters, QVector<QVariant> parametersList)
{
    ShapeFactory* f = m_pluginManager->getShapeMap().value(name, 0);
    if (f)
        CreateShape(f, numberOfParameters, parametersList);
    else
        emit Abort("CreateShape: Selected shape type is not valid.");
}

/*!
 * Creates a \a trackerType shape node from the as current selected node child.
 *
 * If the current node is not a valid parent node or \a trackerType is not a valid type, the shape node will not be created.
 *
 */
void MainWindow::CreateTracker(QString name)
{
    TrackerFactory* f = m_pluginManager->getTrackerMap().value(name, 0);
    if (f)
        CreateTracker(f);
    else
        emit Abort("CreateTracker: Selected tracker type is not valid.");
}

/*!
 * If there is a node selected and this node is not the root node, cuts current selected node from the model. Otherwise, nothing is done.
 */
void MainWindow::Cut()
{
    int validNode = 1;
    if (!m_selectionModel->hasSelection() ) validNode = 0;
    if (m_selectionModel->currentIndex() == ui->sceneModelView->rootIndex() ) validNode = 0;
    if (m_selectionModel->currentIndex().parent() == ui->sceneModelView->rootIndex() ) validNode = 0;
    if (!validNode)
    {
        emit Abort(tr("Cut: No valid node selected to cut.") );
        return;
    }
    CmdCut* cmd = new CmdCut(m_selectionModel->currentIndex(), m_coinNode_Buffer, m_sceneModel);
    m_commandStack->push(cmd);

    UpdateLightSize();
    setDocumentModified(true);
}

/*!
 * If there \a nodeURL is a valid node url and this node is not the root node, cuts current selected node from the model. Otherwise, nothing is done.
 */
void MainWindow::Cut(QString nodeURL)
{
    if (nodeURL.isEmpty() )
    {
        emit Abort(tr("Cut: There is no node with defined url.") );
        return;
    }

    QModelIndex nodeIndex = m_sceneModel->IndexFromUrl(nodeURL);

    if (!nodeIndex.isValid() )
    {
        emit Abort(tr("Cut: There is no node with defined url.") );
        return;
    }

    if (nodeIndex == ui->sceneModelView->rootIndex() )
    {
        emit Abort(tr("Cut: Selected node is not valid node to cut.") );
        return;
    }
    if (nodeIndex.parent() == ui->sceneModelView->rootIndex() )
    {
        emit Abort(tr("Cut: Selected node is not valid node to cut.") );
        return;
    }

    CmdCut* command = new CmdCut(nodeIndex, m_coinNode_Buffer, m_sceneModel);
    m_commandStack->push(command);

    UpdateLightSize();
    setDocumentModified(true);
}

/*!
 * * Deletes selected node if there is a valid node to delete.
 */
void MainWindow::Delete()
{
    if (!m_selectionModel->hasSelection() )
    {
        emit Abort("Delete: There is no node selected to delete");
        return;
    }

    QModelIndex index = m_selectionModel->currentIndex();
    m_selectionModel->clearSelection();

    InstanceNode* instance = m_sceneModel->getInstance(index);
    m_selectionModel->setCurrentIndex(m_sceneModel->IndexFromUrl(instance->getParent()->getURL()), QItemSelectionModel::ClearAndSelect);

    Delete(index);
}

/*!
 * Creates a new delete command, where the node with the \a nodeURL was deleted.
 *
 * If \a nodeURL is not a valid node url, nothing is done.
 */
void MainWindow::Delete(QString nodeURL)
{
    QModelIndex index = m_sceneModel->IndexFromUrl(nodeURL);
    if (!index.isValid())
    {
        emit Abort(tr("Delete: There is no node with defined url.") );
        return;
    }

    Delete(index);
    if (m_selectionModel->isSelected(index)) m_selectionModel->clearSelection();
}

/*!
 * Creates a new delete command, where the \a index node was deleted.
 *
 * Returns \a true if the node was successfully deleted, otherwise returns \a false.
 */
bool MainWindow::Delete(QModelIndex index)
{
    if (!index.isValid()) return false;
    if (index == ui->sceneModelView->rootIndex() ) return false;
    if (index.parent() == ui->sceneModelView->rootIndex() ) return false;

    InstanceNode* instance = m_sceneModel->getInstance(index);
    SoNode* node = instance->getNode();

    if (node->getTypeId().isDerivedFrom(Tracker::getClassTypeId()))
    {
        CmdDeleteTracker* cmd = new CmdDeleteTracker(index, m_document->getSceneKit(), *m_sceneModel);
        m_commandStack->push(cmd);
    }
    else if (node->getTypeId().isDerivedFrom(SunKit::getClassTypeId()))
        return false;
    else
    {
        CmdDelete* cmd = new CmdDelete(index, m_sceneModel);
        m_commandStack->push(cmd);
    }

//    UpdateLightSize();
    setDocumentModified(true);
    return true;
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
    if ((!ui->sceneModelView->currentIndex().isValid() ) || (ui->sceneModelView->currentIndex() == ui->sceneModelView->rootIndex() ) )
        parentIndex = m_sceneModel->index (0,0,ui->sceneModelView->rootIndex());
    else
        parentIndex = ui->sceneModelView->currentIndex();

    SoNode* coinNode = m_sceneModel->getInstance(parentIndex)->getNode();
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

    CmdInsertSeparatorKit* cmdInsertSeparatorKit = new CmdInsertSeparatorKit(componentLayout, QPersistentModelIndex(parentIndex), m_sceneModel);
    cmdInsertSeparatorKit->setText("Insert SeparatorKit node");
    m_commandStack->push(cmdInsertSeparatorKit);

    UpdateLightSize();
    setDocumentModified(true);
}

/*!
 * Starts new tonatiuh empty model.
 */
void MainWindow::New()
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
void MainWindow::Open(QString fileName)
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
 * Inserts a new node as a \a nodeURL node child. The new node is a copy to node saved into the clipboard.
 * The \a pasteType take "Shared" or "Copied" values.
 */
void MainWindow::Paste(QString nodeURL, QString pasteType)
{
    if (!m_coinNode_Buffer)
    {
        emit Abort(tr("Paste: There is not node copied.") );
        return;
    }

    QModelIndex nodeIndex = m_sceneModel->IndexFromUrl(nodeURL);
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
    if (!m_selectionModel->hasSelection())
    {
        emit Abort("PasteCopy: There is not node copied.");
        return;
    }
    Paste(m_selectionModel->currentIndex(), tgc::Copied);
}

/*!
 * Inserts a new node as a current node child. The new node is a link to node saved into the clipboard.
 */
void MainWindow::PasteLink()
{
    if (!m_selectionModel->hasSelection())
    {
        emit Abort("PasteCopy: There is not node copied.");
        return;
    }
    Paste(m_selectionModel->currentIndex(), tgc::Shared);
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
    Air* transmissivity = 0;

    QElapsedTimer timer;
    timer.start();

    UpdateLightSize();

    if (ReadyForRaytracing(instanceRoot, instanceSun, sunShape, sunAperture, transmissivity) )
    {
        if (!m_photons->getExporter() )
        {
            if (!m_photonsSettings) return;
            PhotonsAbstract* photonsExporter = CreatePhotonMapExport();
            if (!photonsExporter) return;
            if (!m_photons->setExporter(photonsExporter)) return;
        }

        QVector<InstanceNode*> exportSuraceList;
        for (QString s : m_photonsSettings->exportSurfaceNodeList)
        {
            m_sceneModel->IndexFromUrl(s);
            InstanceNode* node = m_sceneModel->getInstance(m_sceneModel->IndexFromUrl(s));
            exportSuraceList << node;
        }

        // compute bounding boxes and world to object transforms
        instanceRoot->updateTree(Transform::Identity);

        m_photons->setTransform(instanceRoot->getTransform().inversed() ); //?

        SunKit* sunKit = static_cast<SunKit*> (instanceSun->getNode() );
        if (!sunKit->findTexture(m_widthDivisions, m_heightDivisions, instanceRoot))
        {
            emit Abort(tr("There are no surfaces defined for ray tracing") );
            ShowRaysIn3DView();
            return;
        }

        QVector<long> raysPerThread;
        int progressMax = 100;
        ulong t1 = m_raysTraced/progressMax;
        for (int progress = 0; progress < progressMax; ++progress)
            raysPerThread << t1;

        if (t1*progressMax < m_raysTraced)
            raysPerThread << m_raysTraced - t1*progressMax;



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
        if (!dynamic_cast<AirVacuum*>(transmissivity))
            airTemp = transmissivity;

        photonMap = QtConcurrent::map(raysPerThread, RayTracer(instanceRoot,
            instanceSun, sunAperture, sunShape, airTemp,
            *m_rand,
            &mutex, m_photons, &mutexPhotonMap,
            exportSuraceList) );
        futureWatcher.setFuture(photonMap);

        // Display the dialog and start the event loop.
        progressDialog.exec();
        futureWatcher.waitForFinished();
        std::cout << "QtConcurrent finished: " << timer.elapsed() << std::endl;

        m_raysTracedTotal += m_raysTraced;

        if (exportSuraceList.count() < 1)
            ShowRaysIn3DView();
        else {
            ui->actionViewRays->setEnabled(false);
            ui->actionViewRays->setChecked(false);
        }

        double area = sunAperture->getArea();
        double irradiance = sunKit->irradiance.getValue();
        double power = area*irradiance/m_raysTracedTotal;
        m_photons->endExport(power);
    }

    std::cout << "Elapsed time (Run): " << timer.elapsed() << std::endl;
}

/*
 * Runs ray trace to calculate a flux distribution map in the surface of the node \a nodeURL related to the side \a surfaceSide.
 * The map will be calculated with the parameters \a nOfRays, \a heightDivisions and \a heightDivisions.
 * The results will save in a file \a directory \a QString fileName, the coordinates of the cells depending on the variable \a saveCoord.
 */
void MainWindow::RunFluxAnalysis(QString nodeURL, QString surfaceSide, unsigned int nOfRays, int heightDivisions, int widthDivisions, QString directory, QString file, bool saveCoords)
{
    TSceneKit* sceneKit = m_document->getSceneKit();
    if (!sceneKit) return;

    SunKit* lightKit = (SunKit*) sceneKit->getPart("lightList[0]", false);
    if (!lightKit) return;

    InstanceNode* rootSeparatorInstance = m_sceneModel->getInstance(QModelIndex());
    if (!rootSeparatorInstance) return;
    rootSeparatorInstance = rootSeparatorInstance->children[1];

    QVector<RandomFactory*> randomDeviateFactoryList = m_pluginManager->getRandomFactories();
    //Check if there is a random generator selected;
    if (m_selectedRandomDeviate == -1)
    {
        if (randomDeviateFactoryList.size() > 0) m_selectedRandomDeviate = 0;
        else return;
    }

    //Create the random generator
    if (!m_rand) m_rand =  randomDeviateFactoryList[m_selectedRandomDeviate]->create();

    FluxAnalysis fluxAnalysis(sceneKit, *m_sceneModel, rootSeparatorInstance, m_widthDivisions, m_heightDivisions, m_rand);
    fluxAnalysis.run(nodeURL, surfaceSide, nOfRays, false, heightDivisions, widthDivisions);
    fluxAnalysis.write(directory, file, saveCoords);
}

/*!
 * Saves current tonatiuh model into \a fileName file.
 */
void MainWindow::SaveAs(QString fileName)
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
 * Changes selected node to the node with \a nodeUrl. If model does not contains a node with defined url,
 * the selection will be null.
 */
void MainWindow::SelectNode(QString url)
{
    QModelIndex index = m_sceneModel->IndexFromUrl(url);
    m_selectionModel->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
}

/*!
 * Sets to export all surfaces photons.
 */
void MainWindow::SetExportAllPhotonMap()
{
    if (!m_photonsSettings) return;
    m_photonsSettings->exportSurfaceNodeList.clear();
}

/*!
 * If \a enabled is true, sets to export photons coordinates. Otherwise, the photons will not be exported.
 * If \a global is true, the coordinates will be exported in global coordinate system. Otherwise, into local to surface.
 */
void MainWindow::SetExportCoordinates(bool enabled, bool global)
{
    if (!m_photonsSettings) return;
    m_photonsSettings->exportCoordinates = enabled;
    m_photonsSettings->exportInGlobalCoordinates = global;
}

/*!
 * If \a enabled is true, sets to export intersection surface.
 * Otherwise, the intersection surface will not be exported.
 */
void MainWindow::SetExportIntersectionSurface(bool enabled)
{
    if (!m_photonsSettings) return;
    m_photonsSettings->exportSurfaceID = enabled;
}

/*!
 * If \a enabled is true, sets to export intersection surface side.
 * Otherwise, the intersection surface side will not be exported.
 */
void MainWindow::SetExportIntersectionSurfaceSide(bool enabled)
{
    if (!m_photonsSettings) return;
    m_photonsSettings->exportIntersectionSurfaceSide = enabled;
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

    m_photonsSettings->modeTypeName = name;
}

/*!
 * If \a enabled is true, sets to export for each photon the prevous and next photoan id.
 * Otherwise, this information will not be exported.
 */
void MainWindow::SetExportPreviousNextPhotonID(bool enabled)
{
    if (!m_photonsSettings) return;
    m_photonsSettings->exportPreviousNextPhotonID = enabled;
}

/*!
 * Sets to selected export type parameter named \a parameterName the value \a parameterValue.
 */
void MainWindow::SetExportTypeParameterValue(QString parameterName, QString parameterValue)
{
    if (!m_photonsSettings) return;
    m_photonsSettings->AddParameter(parameterName, parameterValue);
}

/*!
 * If \a increase is false, starts with a new photon map every ray tracer. Otherwise, the photon map increases.
 */
void MainWindow::SetIncreasePhotonMap(bool increase)
{
    m_increasePhotonMap = increase;
}

/*!
 * Sets \a nodeName as the current node name.
 */
void MainWindow::SetNodeName(QString name)
{
    if (!m_selectionModel->hasSelection() )
    {
        emit Abort(tr("SetNodeName: No node selected.") );
        return;
    }

    if (m_selectionModel->currentIndex() == ui->sceneModelView->rootIndex())
    {
        emit Abort(tr("SetNodeName: Cannot change the name of the current selected node cannot.") );
        return;
    }

    ChangeNodeName(m_selectionModel->currentIndex(), name);
}

/*!
 * Sets the number of photons that the photon map can store to \a nPhotons.
 */
void MainWindow::SetPhotonMapBufferSize(uint nPhotons)
{
    m_bufferPhotons = nPhotons;
}

/*!
 * Sets the random number generator type, \a typeName, for ray tracing.
 */
void MainWindow::SetRandomDeviateType(QString name)
{
    QVector<RandomFactory*> factoryList = m_pluginManager->getRandomFactories();
    if (factoryList.size() == 0) return;

    QVector< QString > randomNames;
    for (int i = 0; i < factoryList.size(); i++)
        randomNames << factoryList[i]->name();

    int oldSelectedRandomDeviate = m_selectedRandomDeviate;

    if (randomNames.indexOf(name) < 0)
    {
        emit Abort(tr("SetRandomDeviateType: Defined random generator is not valid type.") );
        return;
    }
    m_selectedRandomDeviate = randomNames.indexOf(name);
    if (oldSelectedRandomDeviate != m_selectedRandomDeviate)
    {
        delete m_rand;
        m_rand = 0;
    }
}

/*!
 * Sets the ray casting surface grid elemets to \a widthDivisions x \a heightDivisions.
 */
void MainWindow::SetRayCastingGrid(int widthDivisions, int heightDivisions)
{
    m_widthDivisions = widthDivisions;
    m_heightDivisions = heightDivisions;
}

/*!
 * Sets the parameters to represent the ray tracer results.
 * Tonatiuh draws the \a raysFaction faction of traced rays. If \a drawPhotons is true all photons are represented.
 *
 */
void MainWindow::SetRaysDrawingOptions(bool drawRays, bool drawPhotons)
{
    m_drawRays = drawRays;
    m_drawPhotons = drawPhotons;
}

/*!
 *    Sets \a rays as the number of rays to trace for each run action.
 */
void MainWindow::SetRaysPerIteration(uint rays)
{
    m_raysTraced = rays;
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

    CmdSunKitModified* command = new CmdSunKitModified(sunKit, sceneKit, m_sceneModel);
    m_commandStack->push(command);

    //UpdateLightDimensions();
    UpdateLightSize();

    ui->parametersTabs->UpdateView();
    setDocumentModified(true);

    ui->actionCalculateSunPosition->setEnabled(true);
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

    CmdModifyParameter* cmd = new CmdModifyParameter(sunshape, parameter, value, m_sceneModel);
    if (m_commandStack) m_commandStack->push(cmd);
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
    m_commandStack->push(cmd);
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

    CmdModifyParameter* cmd = new CmdModifyParameter(air, parameter, value, m_sceneModel);
    if (m_commandStack) m_commandStack->push(cmd);
    setDocumentModified(true);
}

/*!
 * Sets to the \a parameter of the node \a nodeUrl the value defined at \a value.
 */
void MainWindow::SetValue(QString nodeUrl, QString parameter, QString value)
{
    if (nodeUrl.isEmpty() || parameter.isEmpty() || value.isEmpty() )
    {
        emit Abort(tr("SetValue: You must define a node url, a parameter name and a value.") );
        return;
    }

    QModelIndex nodeIndex = m_sceneModel->IndexFromUrl(nodeUrl);
    if (!nodeIndex.isValid() )
    {
        emit Abort(tr("SetValue: Defined node url is not a valid url.") );
        return;
    }

    InstanceNode* nodeInstance = m_sceneModel->getInstance(nodeIndex);
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


//Manipulators actions
void MainWindow::SoTransform_to_SoCenterballManip()
{
    //Transform to a SoCenterballManip manipulator
    QModelIndex currentIndex = ui->sceneModelView->currentIndex();

    InstanceNode* instanceNode = m_sceneModel->getInstance(currentIndex);
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
    QModelIndex currentIndex = ui->sceneModelView->currentIndex();

    InstanceNode* instanceNode = m_sceneModel->getInstance(currentIndex);
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
    QModelIndex currentIndex = ui->sceneModelView->currentIndex();

    InstanceNode* instanceNode = m_sceneModel->getInstance(currentIndex);
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
    QModelIndex currentIndex = ui->sceneModelView->currentIndex();

    InstanceNode* instanceNode = m_sceneModel->getInstance(currentIndex);
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
    QModelIndex currentIndex = ui->sceneModelView->currentIndex();

    InstanceNode* instanceNode = m_sceneModel->getInstance(currentIndex);
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
    QModelIndex currentIndex = ui->sceneModelView->currentIndex();

    InstanceNode* instanceNode = m_sceneModel->getInstance(currentIndex);
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
    QModelIndex currentIndex = ui->sceneModelView->currentIndex();

    InstanceNode* instanceNode = m_sceneModel->getInstance(currentIndex);
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
    QModelIndex currentIndex = ui->sceneModelView->currentIndex();

    InstanceNode* instanceNode = m_sceneModel->getInstance(currentIndex);
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
    InstanceNode* instance = m_sceneModel->getInstance(index);
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
    QModelIndex parentIndex = ( (!ui->sceneModelView->currentIndex().isValid() ) || (ui->sceneModelView->currentIndex() == ui->sceneModelView->rootIndex() ) ) ?
                m_sceneModel->index(0, 0, ui->sceneModelView->rootIndex()) :
                ui->sceneModelView->currentIndex();

    InstanceNode* parentInstance = m_sceneModel->getInstance(parentIndex);
    SoNode* parentNode = parentInstance->getNode();
    if (!parentNode->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId() ) ) return;

    TSeparatorKit* componentLayout = factory->CreateTComponent(m_pluginManager);
    if (!componentLayout) return;

    QString typeName = factory->name();
    componentLayout->setName(typeName.toStdString().c_str() );

    CmdInsertSeparatorKit* cmd = new CmdInsertSeparatorKit(componentLayout, QPersistentModelIndex(parentIndex), m_sceneModel);
    QString text = QString("Create Component: %1").arg(factory->name().toLatin1().constData() );
    cmd->setText(text);
    m_commandStack->push(cmd);

    UpdateLightSize();
    setDocumentModified(true);
}

/*!
 * Creates a material node from the \a pMaterialFactory as current selected node child.
 *
 * If the current node is not a surface type node, the material node will not be created.
 */
void MainWindow::CreateMaterial(MaterialFactory* factory)
{
    QModelIndex index = ui->sceneModelView->currentIndex();
    if (!index.isValid()) return;
    ui->sceneModelView->expand(index);

    InstanceNode* instance = m_sceneModel->getInstance(index);
    SoNode* node = instance->getNode();
    TShapeKit* kit = dynamic_cast<TShapeKit*>(node);
    if (!kit) return;

    MaterialRT* material = factory->create();
    material->setName(factory->name().toStdString().c_str());

    CmdInsertShape* cmd = new CmdInsertShape(kit, material, m_sceneModel);
    m_commandStack->push(cmd);

    setDocumentModified(true);
}

/*!
 * Creates a shape node from the \a pShapeFactory as current selected node child.
 *
 * If the current node is not a surface type node, the shape node will not be created.
 */
void MainWindow::CreateShape(ShapeFactory* factory)
{
    QModelIndex index = ui->sceneModelView->currentIndex();
    if (!index.isValid()) return;
    ui->sceneModelView->expand(index);

    InstanceNode* instance = m_sceneModel->getInstance(index);
    SoNode* node = instance->getNode();
    TShapeKit* kit = dynamic_cast<TShapeKit*>(node);
    if (!kit) return;

    ShapeRT* shape = factory->create();
    shape->setName(factory->name().toStdString().c_str());

    CmdInsertShape* cmd = new CmdInsertShape(kit, shape, m_sceneModel);
    m_commandStack->push(cmd);

//    UpdateLightSize();
    setDocumentModified(true);
}

/*!
 * Creates a shape node from the \a pShapeFactory as current selected node child.
 *
 * If the current node is not a surface type node, the shape node will not be created.
 */
void MainWindow::CreateShape(ShapeFactory* factory, int /*numberofParameters*/, QVector<QVariant> parametersList)
{
    QModelIndex index = ui->sceneModelView->currentIndex();
    if (!index.isValid()) return;

    InstanceNode* instance = m_sceneModel->getInstance(index);
    SoNode* node = instance->getNode();
    TShapeKit* kit = dynamic_cast<TShapeKit*>(node);
    if (!kit) return;

    ShapeRT* shape = factory->create(parametersList);
    shape->setName(factory->name().toStdString().c_str() );

    CmdInsertShape* cmd = new CmdInsertShape(kit, shape, m_sceneModel);
    m_commandStack->push(cmd);

//    UpdateLightSize();
    setDocumentModified(true);
}

void MainWindow::CreateProfile(ProfileFactory* factory)
{
    QModelIndex index = ui->sceneModelView->currentIndex();
    if (!index.isValid()) return;
    ui->sceneModelView->expand(index);

    InstanceNode* instance = m_sceneModel->getInstance(index);
    SoNode* node = instance->getNode();
    TShapeKit* kit = dynamic_cast<TShapeKit*>(node);
    if (!kit) return;

    ProfileRT* profile = factory->create();
    profile->setName(factory->name().toStdString().c_str());

    CmdInsertShape* cmd = new CmdInsertShape(kit, profile, m_sceneModel);
    m_commandStack->push(cmd);

//    UpdateLightSize();
    setDocumentModified(true);
}

/*!
 * Creates a shape node from the \a pTrackerFactory as current selected node child.
 *
 */
void MainWindow::CreateTracker(TrackerFactory* factory)
{
    QModelIndex parentIndex = ui->sceneModelView->currentIndex();
    if (!parentIndex.isValid()) return;

    InstanceNode* parentInstance = m_sceneModel->getInstance(parentIndex);
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

    CmdInsertTracker* cmd = new CmdInsertTracker(tracker, parentIndex, m_sceneModel);
    m_commandStack->push(cmd);

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
    TSceneKit* coinScene = m_document->getSceneKit();

    m_sceneModel->setSceneKit(*coinScene);
    m_graphicsRoot->AddModel(coinScene);

    QModelIndex viewLayoutIndex = m_sceneModel->IndexFromUrl("");
    InstanceNode* viewLayout = m_sceneModel->getInstance(viewLayoutIndex);
    ui->sceneModelView->setRootIndex(viewLayoutIndex);

    InstanceNode* concentratorRoot = viewLayout->children[0];

    m_selectionModel->setCurrentIndex(m_sceneModel->IndexFromUrl(concentratorRoot->getURL() ), QItemSelectionModel::ClearAndSelect);
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
SoSeparator* MainWindow::CreateGrid()
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
    QVector<PhotonsFactory*> factoryList = m_pluginManager->getExportFactories();
    if (factoryList.size() == 0) return 0;

    QVector< QString > exportPMModeNames;
    for (int i = 0; i < factoryList.size(); i++)
        exportPMModeNames << factoryList[i]->name();

    int exportModeFactoryIndex = exportPMModeNames.indexOf(m_photonsSettings->modeTypeName);
    if (exportModeFactoryIndex < 0) return 0;

    PhotonsFactory* pExportModeFactory = factoryList[exportModeFactoryIndex];
    if (!pExportModeFactory) return 0;

    PhotonsAbstract* pExportMode = pExportModeFactory->create();
    if (!pExportMode) return 0;

    pExportMode->SetSaveCoordinatesEnabled(m_photonsSettings->exportCoordinates);
    pExportMode->SetSaveCoordinatesInGlobalSystemEnabled(m_photonsSettings->exportInGlobalCoordinates);
    pExportMode->SetSavePreviousNextPhotonsID(m_photonsSettings->exportPreviousNextPhotonID);
    pExportMode->SetSaveSideEnabled(m_photonsSettings->exportIntersectionSurfaceSide);
    pExportMode->SetSaveSurfacesIDEnabled(m_photonsSettings->exportSurfaceID);


    if (m_photonsSettings->exportSurfaceNodeList.count() > 0)
        pExportMode->SetSaveAllPhotonsEnabled();
    else
        pExportMode->SetSaveSurfacesURLList(m_photonsSettings->exportSurfaceNodeList);

    QMap< QString, QString > exportTypeParameters = m_photonsSettings->modeTypeParameters;
    QMap< QString, QString >::const_iterator i = exportTypeParameters.constBegin();
    while (i != exportTypeParameters.constEnd() )
    {
        pExportMode->SetSaveParameterValue(i.key(), i.value() );
        ++i;
    }

    pExportMode->SetSceneModel(*m_sceneModel);

    return pExportMode;
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

        if (answer == QMessageBox::Yes) return Save();
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
bool MainWindow::Paste(QModelIndex nodeIndex, tgc::PasteType type)
{
    if (!nodeIndex.isValid() ) return false;
    if (!m_coinNode_Buffer) return false;

    InstanceNode* ancestor = m_sceneModel->getInstance(nodeIndex);
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

    CmdPaste* commandPaste = new CmdPaste(type, m_selectionModel->currentIndex(), m_coinNode_Buffer, *m_sceneModel);
    m_commandStack->push(commandPaste);

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

    if (!sceneKit->getPart("air", false))
        air = 0;
    else
        air = static_cast<Air*>(sceneKit->getPart("air", false));

    InstanceNode* sceneInstance = m_sceneModel->getInstance(QModelIndex());
    if (!sceneInstance) return false;

    instanceSun = sceneInstance->children[0];
    if (!instanceSun) return false;

    instanceLayout = sceneInstance->children[1];
    if (!instanceLayout) return false;

    //Check if there is a light and is properly configured
    if (!sceneKit->getPart("lightList[0]", false) ) return false;
    SunKit* sunKit = static_cast<SunKit*>(sceneKit->getPart("lightList[0]", false) );
//    sunKit->updateTransform();

    if (!sunKit->getPart("tsunshape", false)) return false;
    sunShape = static_cast<SunShape*>(sunKit->getPart("tsunshape", false));

    if (!sunKit->getPart("icon", false)) return false;
    sunAperture = static_cast<SunAperture*>(sunKit->getPart("icon", false));

    if (!sunKit->getPart("transform", false)) return false;
    SoTransform* sunTransform = static_cast<SoTransform*>(sunKit->getPart("transform", false));
    instanceSun->setTransform(tgf::makeTransform(sunTransform));

    QVector<RandomFactory*> randomDeviateFactoryList = m_pluginManager->getRandomFactories();
    //Check if there is a random generator selected;
    if (m_selectedRandomDeviate == -1)
    {
        if (randomDeviateFactoryList.size() > 0) m_selectedRandomDeviate = 0;
        else return false;
    }

    //Create the random generator
    if (!m_rand) m_rand = randomDeviateFactoryList[m_selectedRandomDeviate]->create();


    //Create the photon map where photons are going to be stored
    if (!m_increasePhotonMap)
    {
        delete m_photons;
        m_photons = new Photons();
        m_photons->setBufferSize(m_bufferPhotons);
        m_raysTracedTotal = 0;
    }

    if (!m_photons)
    {
        m_photons = new Photons();
        m_photons->setBufferSize(m_bufferPhotons);
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
    if (!m_document->WriteFile(fileName) )
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

/*!
 * Defines the settings to save or export and save photon map.
 */
bool MainWindow::SetPhotonMapExportSettings()
{
    QVector<PhotonsFactory*> exportPhotonMapModeList = m_pluginManager->getExportFactories();
    RayExportDialog dialog(*m_sceneModel, exportPhotonMapModeList);
    if (!dialog.exec() ) return false;

    if (m_photonsSettings) delete m_photonsSettings;

    m_photonsSettings = new PhotonsSettings;
    *m_photonsSettings = dialog.GetExportPhotonMapSettings();
    return true;
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
            a, SIGNAL(CreateMaterial(MaterialFactory*)),
            this, SLOT(CreateMaterial(MaterialFactory*))
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
                a, SIGNAL(CreateShape(ShapeFactory*)),
                this, SLOT(CreateShape(ShapeFactory*))
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
            a, SIGNAL(CreateProfile(ProfileFactory*)),
            this, SLOT(CreateProfile(ProfileFactory*))
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
            a, SIGNAL(CreateTracker(TrackerFactory*)),
            this, SLOT(CreateTracker(TrackerFactory*))
        );
    }

    QPushButton* button = new QPushButton;
    button->setIcon(QIcon(":/images/scene/nodeTracker.png"));
    button->setToolTip("Trackers");
    button->setMenu(menu);
    findChild<QToolBar*>("insertToolBar")->addWidget(button);
}

/**
 * Action slot to show/hide viewer background.
 */
void MainWindow::ShowBackground()
{
    bool v = ui->actionViewBackground->isChecked();
    m_graphicsRoot->ShowBackground(v);
}

/**
 * Action slot to show/hide a grid with the scene dimensions.
 */
void MainWindow::ShowGrid()
{
    m_graphicsRoot->ShowGrid(ui->actionViewGrid->isChecked());

    /*if( ui->actionViewGrid->isChecked() )
           {
            InstanceNode* sceneInstance = m_sceneModel->NodeFromIndex( ui->sceneModelView->rootIndex() );
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
    ui->actionViewRays->setEnabled(false);
    ui->actionViewRays->setChecked(false);

    if (m_drawRays || m_drawPhotons)
    {
        SoSeparator* rays = new SoSeparator;
        rays->setName("Rays");

        if (m_drawPhotons)
        {
            SoSeparator* points = trf::DrawPhotons(*m_photons);
            rays->addChild(points);
        }

        if (m_drawRays)
        {
            SoSeparator* currentRays = trf::DrawRays(*m_photons, m_raysTracedTotal);
            if (currentRays) rays->addChild(currentRays);
        }
        m_graphicsRoot->AddRays(rays);

        ui->actionViewRays->setEnabled(true);
        ui->actionViewRays->setChecked(true);
    }
}

/*!
 * Returns to the start origin state and starts with a new model defined in \a fileName.
 * If the file name is not defined, it starts with an empty scene.
 */
bool MainWindow::StartOver(const QString& fileName)
{
    InstanceNode* sceneInstance = m_sceneModel->getInstance(ui->sceneModelView->rootIndex() );

    InstanceNode* concentratorRoot = sceneInstance->children[sceneInstance->children.size() - 1];
    m_selectionModel->setCurrentIndex(m_sceneModel->IndexFromUrl(concentratorRoot->getURL() ), QItemSelectionModel::ClearAndSelect);

    ui->actionViewRays->setEnabled(false);
    ui->actionViewRays->setChecked(false);

    m_graphicsRoot->RemoveRays();
    m_graphicsRoot->RemoveModel();

    m_commandStack->clear();
    m_sceneModel->Clear();

//    SetSunPositionCalculatorEnabled(0);

    if (!fileName.isEmpty() && m_document->ReadFile(fileName) )
    {
        statusBar()->showMessage(tr("File loaded"), 2000);
        SetCurrentFile(fileName);
    }
    else
    {
        m_document->New();
        statusBar()->showMessage(tr("New file"), 2000);
        SetCurrentFile("");
    }

    ChangeModelScene();
    ui->sceneModelView->expandToDepth(0);
    SelectNode("//Layout");
    on_actionViewAll_triggered();
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
    SoSceneKit* sceneKit = m_document->getSceneKit();

    SunKit* sunKit = static_cast<SunKit*>(sceneKit->getPart("lightList[0]", false) );
    if (!sunKit) return;

    SoGroup* separatorKit = static_cast<SoGroup*>(sceneKit->getPart("group", false) );
    if (!separatorKit) return;

    SoGetBoundingBoxAction* action = new SoGetBoundingBoxAction(SbViewportRegion() );
    separatorKit->getBoundingBox(action);
    SbBox3f box = action->getBoundingBox();
    delete action;

    if (!box.isEmpty())
    {
        Box3D sceneBox(
            Vector3D(box.getMin()[0], box.getMin()[1], box.getMin()[2]),
            Vector3D(box.getMax()[0], box.getMax()[1], box.getMax()[2])
        );

        if (sunKit) sunKit->setBox(sceneBox);
    }

    m_sceneModel->UpdateSceneModel();
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
        if (n < m_recentFiles.count() ) {
            QString text = tr("&%1 %2")
                    .arg(n + 1)
                    .arg(StrippedName(m_recentFiles[n]) );
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
    settings.setValue("geometry", geometry() );

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
    camera->viewAll(m_graphicsRoot->GetNode(), vpr);
}

void MainWindow::on_actionViewSelected_triggered()
{
    QModelIndex index = m_selectionModel->currentIndex();
    InstanceNode* node = m_sceneModel->getInstance(index);

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
