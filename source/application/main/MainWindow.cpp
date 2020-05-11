#include "MainWindow.h"

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

#include "libraries/geometry/gc.h"
#include "libraries/geometry/gf.h"
#include "libraries/geometry/Ray.h"
#include "libraries/geometry/Transform.h"

#include "actions/ActionInsertComponent.h"
#include "actions/ActionInsertMaterial.h"
#include "actions/ActionInsertShape.h"
#include "actions/ActionInsertTracker.h"

#include "commands/CmdChangeNodeName.h"
#include "commands/CmdCopy.h"
#include "commands/CmdCut.h"
#include "commands/CmdDelete.h"
#include "commands/CmdDeleteTracker.h"
#include "commands/CmdInsertMaterial.h"
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
#include "tree/SceneModel.h"
#include "calculator/SunCalculatorDialog.h"
#include "kernel/air/AirAbstract.h"
#include "kernel/air/AirFactory.h"
#include "kernel/air/AirVacuum.h"
#include "kernel/component/ComponentFactory.h"
#include "kernel/run/InstanceNode.h"
#include "kernel/material/MaterialAbstract.h"
#include "kernel/material/MaterialFactory.h"
#include "kernel/photons/PhotonExport.h"
#include "kernel/photons/PhotonExportFactory.h"
#include "kernel/photons/PhotonExportSettings.h"
#include "kernel/photons/PhotonMap.h"
#include "kernel/random/RandomAbstract.h"
#include "kernel/random/RandomFactory.h"
#include "kernel/run/RayTracer.h"
#include "kernel/scene/TSceneKit.h"
#include "kernel/scene/TSeparatorKit.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/trf.h"
#include "kernel/shape/ShapeFactory.h"
#include "kernel/sun/SunFactory.h"
#include "kernel/sun/TLightKit.h"
#include "kernel/sun/TLightShape.h"
#include "kernel/TonatiuhFunctions.h"
#include "kernel/trackers/TDefaultTracker.h"
#include "kernel/trackers/TrackerAbstract.h"
#include "kernel/trackers/TrackerFactory.h"
#include "main/Document.h"
#include "run/ExportPhotonsDialog.h"
#include "run/FluxAnalysis.h"
#include "run/FluxAnalysisDialog.h"
#include "run/RayTraceDialog.h"
#include "script/ScriptEditorDialog.h"
#include "view/GraphicRoot.h"
#include "view/GraphicView.h"
#include "widgets/AboutDialog.h"
#include "widgets/AirDialog.h"
#include "widgets/GridDialog.h"
#include "widgets/NetworkConnectionsDialog.h"
#include "widgets/SunDialog.h"
//#include "ProgressUpdater.h"


void startManipulator(void* data, SoDragger* dragger)
{
    MainWindow* mainwindow = static_cast<MainWindow*>(data);
    mainwindow->StartManipulation(dragger);
}

void finishManipulator(void* data, SoDragger* /*dragger*/)
{
    MainWindow* mainwindow = static_cast<MainWindow*>(data);
    mainwindow->FinishManipulation();
}

/*!
 * Creates a new MainWindow object.
 */
MainWindow::MainWindow(QString tonatiuhFile, QWidget* parent, Qt::WindowFlags flags):
    QMainWindow(parent, flags),
    m_commandStack(0),
    m_commandView(0),
    m_currentFile(""),
    m_document(0),
    m_recentFiles(""),
    m_pluginManager(0),
    m_sceneModel(0),
    m_selectionModel(0),
    m_rand(0),
    m_selectedRandomDeviate(-1),
    m_bufferPhotons(5000000),
    m_increasePhotonMap(false),
    m_pExportModeSettings(0),
    m_pPhotonMap(0),
    m_lastExportFileName(""),
    m_lastExportSurfaceUrl(""),
    m_lastExportInGlobal(true),
    m_graphicsRoot(0),
    m_coinNode_Buffer(0),
    m_manipulators_Buffer(0),
    m_raysTracedTotal(0),
    m_raysTraced(10000),
    m_heightDivisions(200),
    m_widthDivisions(200),
    m_drawPhotons(false),
    m_drawRays(true),
    m_gridXElements(0),
    m_gridZElements(0),
    m_gridXSpacing(0),
    m_gridZSpacing(0),
    m_graphicView(0),
    m_focusView(0)
{
    setupUi(this);

    SetupActions();
    SetupMenus();
    SetupDocument();
    SetupGraphcisRoot();
    SetupModels();
    SetupViews();
    SetupPluginsManager();
    SetupTriggers();

    ReadSettings();

    if (!tonatiuhFile.isEmpty() )
        StartOver(tonatiuhFile);

    SelectNode("//SunNode/RootNode");

    fileToolBar->hide();
    editToolBar->hide();


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
    //delete m_pPluginManager;
    delete m_sceneModel;
    delete m_document;
    delete m_commandStack;
    delete m_commandView;
    delete m_rand;
    delete m_pPhotonMap;
}

/*!
 * Finish the manipulation of the current selected node.
 */
void MainWindow::FinishManipulation()
{
    QModelIndex currentIndex = sceneModelView->currentIndex();
    SoBaseKit* coinNode = static_cast< SoBaseKit* >(m_sceneModel->NodeFromIndex(currentIndex)->GetNode() );

    SoTransform* nodeTransform = static_cast< SoTransform* >(coinNode->getPart("transform", true) );

    QUndoCommand* command = new QUndoCommand();

    QString translationValue = QString("%1 %2 %3").arg(QString::number(nodeTransform->translation.getValue()[0]),
            QString::number(nodeTransform->translation.getValue()[1]),
            QString::number(nodeTransform->translation.getValue()[2]) );
    new CmdModifyParameter(nodeTransform, QString("translation"), translationValue, m_sceneModel, command);
    m_commandStack->push(command);

    UpdateLightSize();
    m_document->SetDocumentModified(true);

}

void MainWindow::ExecuteScriptFile(QString tonatiuhScriptFile)
{
    //New();

    QVector< RandomFactory* > randomDeviateFactoryList = m_pluginManager->getRandomFactories();
    ScriptEditorDialog editor(randomDeviateFactoryList, this);
    editor.show();

    editor.ExecuteScript(tonatiuhScriptFile);

    editor.done(0);
}

void MainWindow::SetPluginManager(PluginManager* pluginManager)
{
    m_pluginManager = pluginManager;
    if (m_pluginManager) SetupPluginsManager();
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
    if (!coinScenePath) gf::SevereError("PathFromIndex Null coinScenePath.");

    SoNodeKitPath* nodePath = static_cast< SoNodeKitPath* > (coinScenePath);
    if (!nodePath) gf::SevereError("PathFromIndex Null nodePath.");


    nodePath->truncate(nodePath->getLength() - 1);
    SoBaseKit* coinNode =  static_cast< SoBaseKit* > (nodePath->getTail() );

    QModelIndex nodeIndex = m_sceneModel->IndexFromPath(*nodePath);
    m_selectionModel->setCurrentIndex(nodeIndex, QItemSelectionModel::ClearAndSelect);

    SoNode* manipulator = coinNode->getPart("transform", true);
    m_manipulators_Buffer = new QStringList();

    SoFieldList fieldList;
    int totalFields = manipulator->getFields(fieldList);

    SoField* pField = 0;
    SbName fieldName;
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
void MainWindow::DefineSunLight()
{
    TSceneKit* sceneKit = m_document->GetSceneKit();
    if (!sceneKit) return;

    InstanceNode* sceneInstance = m_sceneModel->NodeFromIndex(sceneModelView->rootIndex() );
    InstanceNode* concentratorRoot = sceneInstance->children[sceneInstance->children.size() - 1];
    m_selectionModel->setCurrentIndex(m_sceneModel->IndexFromNodeUrl(concentratorRoot->GetNodeURL() ), QItemSelectionModel::ClearAndSelect);

    TLightKit* lightKitOld = 0;
    if (sceneKit->getPart("lightList[0]", false) )
        lightKitOld = static_cast<TLightKit*>(sceneKit->getPart("lightList[0]", false) );

    SunDialog dialog(*m_sceneModel, lightKitOld, m_pluginManager->getSunMap(), this);
    if (!dialog.exec()) return;

    TLightKit* lightKit = dialog.getLightKit();
    if (!lightKit) return;
    sceneKit->azimuth = dialog.getAzimuth();
    sceneKit->elevation = dialog.getElevation();

    CmdLightKitModified* cmd = new CmdLightKitModified(lightKit, sceneKit, *m_sceneModel);
    m_commandStack->push(cmd);
    UpdateLightSize();

    parametersTabs->UpdateView();
    m_document->SetDocumentModified(true);

//    actionCalculateSunPosition->setEnabled(true);
    actionViewRays->setEnabled(false);
    actionViewRays->setChecked(false);
}

/*!
 * Opens a dialog to define the scene transmissivity.
 */
void MainWindow::DefineTransmissivity()
{
    AirDialog dialog(m_pluginManager->getAirMap(), this);

    TSceneKit* sceneKit = m_document->GetSceneKit();
    if (!sceneKit) return;
    AirAbstract* airOld = static_cast<AirAbstract*>(sceneKit->getPart("transmissivity", false));
    if (airOld) dialog.setModel(airOld);

    if (!dialog.exec()) return;

    AirAbstract* airNew = dialog.getModel();
    CmdAirModified* cmd = new CmdAirModified(airNew, sceneKit);
    if (m_commandStack) m_commandStack->push(cmd);
    m_document->SetDocumentModified(true);
}

void MainWindow::DisconnectAllTrackers(bool disconnect)
{
    if (disconnect) m_sceneModel->DisconnectAllTrackers();
    else m_sceneModel->ReconnectAllTrackers();
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
    if ((!sceneModelView->currentIndex().isValid() ) || (sceneModelView->currentIndex() == sceneModelView->rootIndex() ) )
        parentIndex = m_sceneModel->index (0,0,sceneModelView->rootIndex());
    else
        parentIndex = sceneModelView->currentIndex();

    SoNode* coinNode = m_sceneModel->NodeFromIndex(parentIndex)->GetNode();

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
    if (node == sceneModelView->rootIndex() ) return;


    InstanceNode* nodeInstnace = m_sceneModel->NodeFromIndex(node);
    if (nodeInstnace->GetParent()&&nodeInstnace->GetParent()->GetNode()->getTypeId().isDerivedFrom(SoBaseKit::getClassTypeId() ) )
    {
        SoNode* coinNode = nodeInstnace->GetNode();
        //if( coinNode->getTypeId().isDerivedFrom( TTracker::getClassTypeId() ) ) return;

        QUndoCommand* dragAndDrop = new QUndoCommand();
        dragAndDrop->setText("Drag and Drop node");
        new CmdCut(node, m_coinNode_Buffer, m_sceneModel, dragAndDrop);

        new CmdPaste(tgc::Copied, newParent, coinNode, *m_sceneModel, dragAndDrop);
        m_commandStack->push(dragAndDrop);

        UpdateLightSize();
        m_document->SetDocumentModified(true);
    }

}

/*!
 * Inserts a copy of the node \a node as a \a newParent child.
 */
void MainWindow::ItemDragAndDropCopy(const QModelIndex& newParent, const QModelIndex& node)
{
    InstanceNode* nodeInstnace = m_sceneModel->NodeFromIndex(node);
    SoNode* coinNode = nodeInstnace->GetNode();
    //if( coinNode->getTypeId().isDerivedFrom( TTracker::getClassTypeId() ) ) return;

    QUndoCommand* dragAndDropCopy = new QUndoCommand();
    dragAndDropCopy->setText("Drag and Drop Copy");
    new CmdCopy(node, m_coinNode_Buffer, m_sceneModel);
    new CmdPaste(tgc::Shared, newParent, coinNode, *m_sceneModel, dragAndDropCopy);
    m_commandStack->push(dragAndDropCopy);

    UpdateLightSize();
    m_document->SetDocumentModified(true);
}

/*!
 * Shows a dialog to the user to open a existing tonatiuh file.
 */
void MainWindow::Open()
{
    if (OkToContinue() )
    {
        // HKEY_CURRENT_USER\Software\CyI\Tonatiuh
        QSettings settings("CyI", "Tonatiuh");
        QString openDirectory = settings.value("openDirectory", QString("../examples") ).toString();

        QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("Open"), openDirectory,
            tr("Tonatiuh files (*.tnh)")
        );

        if (fileName.isEmpty() ) return;

        QFileInfo file(fileName);
        settings.setValue("openDirectory", file.path());
        StartOver(fileName);
    }
}

/*!
 * Opens selected tonatiuh file from the recent file menu.
 */
void MainWindow::OpenRecentFile()
{
    if (OkToContinue() )
    {
        QAction* action = qobject_cast<QAction*>(sender() );

        if (action)
        {
            QString fileName = action->data().toString();
            StartOver(fileName);
        }
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
 * Runs complete ray tracer. First defines export settings if they are not defined and then runs ray tracer.
 */
void MainWindow::RunCompleteRayTracer()
{
    InstanceNode* rootSeparatorInstance = 0;
    InstanceNode* lightInstance = 0;
    SoTransform* lightTransform = 0;
    SunAbstract* sunShape = 0;
    TLightShape* raycastingSurface = 0;
    AirAbstract* transmissivity = 0;

    QElapsedTimer timer;
    timer.start();

    if (!ReadyForRaytracing(rootSeparatorInstance, lightInstance, lightTransform, sunShape, raycastingSurface, transmissivity) )
        return;

    if (!m_pPhotonMap->GetExportMode() && !SetPhotonMapExportSettings() ) return;


    Run();

    std::cout << "Elapsed time (RunCompleteRayTracer): " << timer.elapsed() << std::endl;
}



/*!
 * Runs the ray tracer for the analysis of the surface. A dialog will be opened to set the surface and flux calculation parameters.
 */
void MainWindow::RunFluxAnalysisRayTracer()
{
    TSceneKit* coinScene = m_document->GetSceneKit();
    if (!coinScene) return;

    TLightKit* lightKit = static_cast< TLightKit* >(coinScene->getPart("lightList[0]", false) );
    if (!lightKit) return;

    InstanceNode*  rootSeparatorInstance = m_sceneModel->NodeFromIndex(sceneModelView->rootIndex() );
    if (!rootSeparatorInstance) return;

    QVector< RandomFactory* > randomDeviateFactoryList = m_pluginManager->getRandomFactories();
    //Check if there is a random generator selected;
    if (m_selectedRandomDeviate == -1)
    {
        if (randomDeviateFactoryList.size() > 0) m_selectedRandomDeviate = 0;
        else return;
    }

    //Create the random generator
    RandomAbstract* pRandomDeviate =  randomDeviateFactoryList[m_selectedRandomDeviate]->create();

    FluxAnalysisDialog dialog(coinScene, *m_sceneModel, rootSeparatorInstance, m_widthDivisions, m_heightDivisions, pRandomDeviate);
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
    if (m_selectionModel->currentIndex() == sceneModelView->rootIndex() ) return;

    QModelIndex componentIndex = sceneModelView->currentIndex();
    SoNode* coinNode = m_sceneModel->NodeFromIndex(componentIndex)->GetNode();

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
    QString saveDirectory = settings.value("saveDirectory", ".").toString();

    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("Save"),
        saveDirectory,
        "Tonatiuh files (*.tnh);; Tonatiuh debug (*.tnhd)"
    );

    if (fileName.isEmpty() ) return false;

    QFileInfo file(fileName);
    settings.setValue("saveDirectory", file.absolutePath() );

//    if (file.completeSuffix().compare("tnh") )
//        fileName.append(".tnh");
    return SaveFile(fileName);
}

/*!
 * Saves selected node subtree as a component in a file.
 * A dialog is opened to select a file name and its location.
 */
bool MainWindow::SaveComponent()
{
    if (!m_selectionModel->hasSelection() ) return false;
    if (m_selectionModel->currentIndex() == sceneModelView->rootIndex() ) return false;

    QModelIndex componentIndex = sceneModelView->currentIndex();

    SoNode* coinNode = m_sceneModel->NodeFromIndex(componentIndex)->GetNode();

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

    if (!selectionPath->containsNode (m_document->GetSceneKit() ) ) return;

    SoNodeKitPath* nodeKitPath = static_cast< SoNodeKitPath* >(selectionPath);
    if (nodeKitPath->getTail()->getTypeId().isDerivedFrom(TLightKit::getClassTypeId() ) )
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
    CmdModifyParameter* command = new CmdModifyParameter(node, name, value, m_sceneModel);
    if (m_commandStack) m_commandStack->push(command);

    UpdateLightSize();
    m_document->SetDocumentModified(true);
}

/*!
 * This property holds whether the SunPositionCalculator action is enabled.
 * If the action is disabled, it does not disappear from menu, but it is displayed
 * in a way which indicates that they are unavailable.
 */
void MainWindow::SetSunPositionCalculatorEnabled(int enabled)
{
    actionCalculateSunPosition->setEnabled(enabled);
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
    if (!index.isValid() ) return;
    m_selectionModel->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);

    InstanceNode* instanceNode = m_sceneModel->NodeFromIndex(index);
    SoNode* coinNode = instanceNode->GetNode();
    SoType type = coinNode->getTypeId();

    QMenu popupmenu(this);

    if (instanceNode->GetParent()&&instanceNode->GetParent()->GetNode()->getTypeId().isDerivedFrom(SoBaseKit::getClassTypeId() ) )
    {
        popupmenu.addAction(actionCut);
        popupmenu.addAction(actionCopy);
        popupmenu.addAction(actionPasteCopy);
        popupmenu.addAction(actionPasteLink);
        popupmenu.addAction(actionDelete);
    }

    if (type.isDerivedFrom(TSeparatorKit::getClassTypeId() ) )
    {
//        QMenu* trackersMenu = popupmenu.addMenu("Trackers");
//        trackersMenu->addAction(actionSetAimingPointRelative);
//        trackersMenu->addAction(actionSetAimingPointAbsolute);

        QMenu* transformMenu = popupmenu.addMenu("Convert to");
        //QMenu transformMenu( "Convert to", &popupmenu );
        //popupmenu.addAction( transformMenu.menuAction() );
        TSeparatorKit* coinKit = dynamic_cast< TSeparatorKit* > (coinNode);
        SoTransform* transform = static_cast< SoTransform* >(coinKit->getPart("transform", true) );
        SoType transformType = transform->getTypeId();

        if (!transformType.isDerivedFrom(SoCenterballManip::getClassTypeId()) ) transformMenu->addAction(tr("SoCenterballManip"),  this, SLOT(SoTransform_to_SoCenterballManip()));
        if (!transformType.isDerivedFrom(SoHandleBoxManip::getClassTypeId()) ) transformMenu->addAction(tr("SoHandleBoxManip"), this, SLOT(SoTransform_to_SoHandleBoxManip()));
        if (!transformType.isDerivedFrom(SoJackManip::getClassTypeId()) ) transformMenu->addAction(tr("SoJackManip"), this, SLOT(SoTransform_to_SoJackManip()));
        if (!transformType.isDerivedFrom(SoTabBoxManip::getClassTypeId()) ) transformMenu->addAction(tr("SoTabBoxManip"), this, SLOT(SoTransform_to_SoTabBoxManip()));
        if (!transformType.isDerivedFrom(SoTrackballManip::getClassTypeId()) ) transformMenu->addAction(tr("SoTrackballManip"),  this, SLOT(SoTransform_to_SoTrackballManip()));
        if (!transformType.isDerivedFrom(SoTransformBoxManip::getClassTypeId()) ) transformMenu->addAction(tr("SoTransformBoxManip"), this, SLOT(SoTransform_to_SoTransformBoxManip()));
        if (!transformType.isDerivedFrom(SoTransformerManip::getClassTypeId()) ) transformMenu->addAction(tr("SoTransformerManip"), this, SLOT(SoTransform_to_SoTransformerManip()));

        if (transformType.isDerivedFrom(SoTransformManip::getClassTypeId()) ) transformMenu->addAction(tr("SoTransform"), this, SLOT(SoManip_to_SoTransform()) );
    }

    //Mostramos el menu contextual
    popupmenu.exec(QCursor::pos() );
}

/*!
 * Shows a dialog with ray tracer options and modifies the ray tracer parameters if changes are done.
 */
void MainWindow::ShowRayTracerOptionsDialog()
{
    QVector<RandomFactory*> randomDeviateFactoryList = m_pluginManager->getRandomFactories();
    RayTraceDialog* options = new RayTraceDialog(
        m_raysTraced,
        randomDeviateFactoryList, m_selectedRandomDeviate,
        m_widthDivisions,m_heightDivisions,
        m_drawRays, m_drawPhotons,
        m_bufferPhotons, m_increasePhotonMap, this);
    options->exec();

    SetRaysPerIteration(options->GetNumRays() );
    SetRandomDeviateType(randomDeviateFactoryList[options->GetRandomFactoryIndex()]->name() );
    SetRayCastingGrid(options->GetWidthDivisions(), options->GetHeightDivisions() );
    SetRaysDrawingOptions(options->DrawRays(), options->DrawPhotons() );
    SetPhotonMapBufferSize(options->GetPhotonMapBufferSize() );
    SetIncreasePhotonMap(options->IncreasePhotonMap() );
}

void MainWindow::ShowWarning(QString message)
{
    QMessageBox::warning(this, tr("Tonatiuh"), message);
}

/*!
 * Reverts a change to Tonatiuh model. The model is returne to the previous state before the command is applied.
 */
void MainWindow::Undo()
{
    m_commandStack->undo();
    UpdateLightSize();
}

//View menu actions
void MainWindow::on_actionViewAxes_toggled()
{
    m_graphicView[0]->ViewCoordinateSystem(actionViewAxes->isChecked() );
    m_graphicView[1]->ViewCoordinateSystem(actionViewAxes->isChecked() );
    m_graphicView[2]->ViewCoordinateSystem(actionViewAxes->isChecked() );
    m_graphicView[3]->ViewCoordinateSystem(actionViewAxes->isChecked() );
}

void MainWindow::on_actionQuadView_toggled()
{
    if (!actionQuadView->isChecked()) {
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

void MainWindow::on_actionViewSun_triggered()
{
    SoSceneKit* coinScene = m_document->GetSceneKit();
    if (!coinScene) return;

    //Check if there is a light and is properly configured
    if (!coinScene->getPart("lightList[0]", false) ) return;
    TLightKit* lightKit = static_cast< TLightKit* >(coinScene->getPart("lightList[0]", false) );
    if (!lightKit) return;

    SoSearchAction coinSearch;
    coinSearch.setNode(lightKit);
    coinSearch.setInterest(SoSearchAction::FIRST);
    coinSearch.apply(m_graphicsRoot->GetNode() );

    SoPath* lightPath = coinSearch.getPath();
    if (!lightPath) gf::SevereError("MainWindow Null light path.");

    const SbViewportRegion vpr = m_graphicView[m_focusView]->GetViewportRegion();
    SoGetMatrixAction* getmatrixaction = new SoGetMatrixAction(vpr);
    getmatrixaction->apply(lightPath);

    Transform lightToWorld = tgf::TransformFromMatrix(getmatrixaction->getMatrix() );

    SoCamera* cam = m_graphicView[m_focusView]->GetCamera();
    Point3D camPosition = lightToWorld(Point3D(0.0, cam->focalDistance.getValue(), 0.0) );

    delete getmatrixaction;

    SbVec3f target = getTarget(cam);

    cam->position.setValue(target + SbVec3f(camPosition.x, camPosition.y, camPosition.z) );
    cam->pointAt(target);
    //cam->viewAll( m_graphicsRoot->GetNode(), vpr );
}

SbVec3f MainWindow::getTarget(SoCamera* cam)
{
    SbVec3f axis;
    float angle;
    cam->orientation.getValue(axis,angle);
    SbRotation* rotation  = new SbRotation (axis,angle);
    SbVec3f center;
    rotation->multVec(SbVec3f (0, 0, -cam->focalDistance.getValue()), center);
    center += cam->position.getValue();
    return center;
}

//void MainWindow::on_action_X_Y_Plane_triggered()
//{
//    SoCamera* cam = m_graphicView[m_focusView]->GetCamera();

//    SbVec3f target = getTarget(cam);

//    cam->position.setValue(target + SbVec3f(0, 0, cam->focalDistance.getValue() ) );
//    cam->pointAt(target, SbVec3f(0, 1, 0)  );

//    //SbViewportRegion vpr = m_graphicView[m_focusView]->GetViewportRegion();
//    //cam->viewAll( m_graphicsRoot->GetNode(), vpr );
//}

void MainWindow::on_actionViewTop_triggered()
{
    SoCamera* camera = m_graphicView[m_focusView]->GetCamera();
    SbVec3f target = getTarget(camera);

    camera->position.setValue(target + SbVec3f(0, 0, camera->focalDistance.getValue()) );
    camera->pointAt(target, SbVec3f(0., 1., 0.) );

    //SbViewportRegion vpr = m_graphicView[m_focusView]->GetViewportRegion();
    //cam->viewAll( m_graphicsRoot->GetNode(), vpr );
}

//void MainWindow::on_action_Y_Z_Plane_triggered()
//{
//    SoCamera* cam = m_graphicView[m_focusView]->GetCamera();
//    SbVec3f target = getTarget(cam);

//    cam->position.setValue(target + SbVec3f(-cam->focalDistance.getValue(), 0, 0) );
//    cam->pointAt(target, SbVec3f(0, 1, 0)  );
//}

void MainWindow::on_actionOpenScriptEditor_triggered()
{
    QVector< RandomFactory* > randomDeviateFactoryList = m_pluginManager->getRandomFactories();
    ScriptEditorDialog editor(randomDeviateFactoryList, this);
    editor.exec();
}

/*
 * Checks for Stephanie's updates and installs them.

   void MainWindow::on_actionCheckForUpdates_triggered()
   {
    m_updateManager->CheckForUpdates( );
   }

 */
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
    GridDialog gridDialog(m_gridXElements, m_gridZElements, m_gridXSpacing, m_gridZSpacing);
    if (!gridDialog.exec()) return;

    m_graphicsRoot->RemoveGrid();

    m_gridXElements = gridDialog.GetXDimension();
    m_gridZElements = gridDialog.GetZDimension();

    if (gridDialog.IsSizeDefined() )
    {
        m_gridXSpacing = gridDialog.GetXSpacing();
        m_gridZSpacing = gridDialog.GetZSpacing();
    }
    else
    {
        InstanceNode* sceneInstance = m_sceneModel->NodeFromIndex(sceneModelView->rootIndex() );
        if (!sceneInstance) return;
        InstanceNode* rootInstance = sceneInstance->children[sceneInstance->children.size() - 1 ];
        if (!rootInstance) return;

        SoGetBoundingBoxAction* bbAction = new SoGetBoundingBoxAction(SbViewportRegion() );
        rootInstance->GetNode()->getBoundingBox(bbAction);

        SbBox3f box = bbAction->getXfBoundingBox().project();
        delete bbAction;

        m_gridXSpacing = 10;
        m_gridZSpacing = 10;

        if (!box.isEmpty() )
        {
            SbVec3f min, max;
            box.getBounds(min, max);

            m_gridXSpacing = (2 *  std::max(fabs(max[0]), fabs(min[0]) ) + 5) / m_gridXElements;
            m_gridZSpacing = (2 *  std::max(fabs(max[2]), fabs(min[2]) ) + 5) / m_gridZElements;
        }
    }

//    m_graphicsRoot->AddGrid(CreateGrid(m_gridXElements, m_gridZElements, m_gridXSpacing, m_gridZSpacing) );
    m_graphicsRoot->AddGrid(CreateGrid() );
    m_graphicsRoot->ShowGrid(true);
}

/*!
 *
 */
void MainWindow::ChangeNodeName(const QModelIndex& index, const QString& newName)
{
    if (!index.isValid() ) return;

    InstanceNode* nodeInstance = m_sceneModel->NodeFromIndex(index);
    if (!nodeInstance) return;
    if (!nodeInstance->GetNode() ) return;

    CmdChangeNodeName* command = new CmdChangeNodeName(index, newName, m_sceneModel);
    QString commandText = QString("Node name changed to: %1").arg(newName);
    command->setText(commandText);
    m_commandStack->push(command);

    m_document->SetDocumentModified(true);

}

/*!
 * Adds a surface to the export sufaces list.
 */
void MainWindow::AddExportSurfaceURL(QString nodeURL)
{
    if (!m_pExportModeSettings) return;
    m_pExportModeSettings->exportSurfaceNodeList.push_back(nodeURL);
}

/*!
 * Changes the light position to the position defined by \a azimuth and \a elevation.
 * The parameters are defined in degree.
 */
void MainWindow::ChangeSunPosition(double azimuth, double elevation)
{

    SoSceneKit* coinScene = m_document->GetSceneKit();
    TLightKit* lightKit = static_cast< TLightKit* >(coinScene->getPart("lightList[0]", false) );
    if (!lightKit)
    {
        QMessageBox::warning(this, "Tonatiuh warning", tr("ChangeSunPosition:: Sun not defined in scene") );
        return;
    }
    CmdLightPositionModified* command = new CmdLightPositionModified(lightKit, azimuth * gc::Degree, (90 - elevation) * gc::Degree);
    m_commandStack->push(command);

    //UpdateLightDimensions();
    UpdateLightSize();
    m_document->SetDocumentModified(true);


    actionViewRays->setEnabled(false);
    actionViewRays->setChecked(false);

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

    cTime myTime = { year, month, day, hours, minutes, seconds };
    cLocation myLocation = { longitude, latitude };
    cSunCoordinates results;
    sunpos(myTime, myLocation, &results);
    ChangeSunPosition(results.dAzimuth, (90 - results.dZenithAngle) );


    actionViewRays->setEnabled(false);
    actionViewRays->setChecked(false);


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
    if (m_selectionModel->currentIndex() == sceneModelView->rootIndex() )
    {
        emit Abort(tr("Copy: The root node can not bee copied.") );
        return;
    }
    if (m_selectionModel->currentIndex().parent() == sceneModelView->rootIndex() )
    {
        emit Abort(tr("Copy: The root node can not bee copied.") );
        return;
    }

    CmdCopy* command = new CmdCopy(m_selectionModel->currentIndex(), m_coinNode_Buffer, m_sceneModel);
    m_commandStack->push(command);

    m_document->SetDocumentModified(true);
}

/*!
 * Copies the node defined with the \a nodeURL to the clipboard.
 *
 * The current node cannot be the model root node or concentrator node.
 */
void MainWindow::Copy(QString nodeURL)
{
    QModelIndex nodeIndex = m_sceneModel->IndexFromNodeUrl(nodeURL);

    if (!nodeIndex.isValid() )
    {
        emit Abort(tr("Copy: There is no node with defined url.") );
        return;
    }
    if (nodeIndex == sceneModelView->rootIndex() || nodeIndex.parent() == sceneModelView->rootIndex() )
    {
        emit Abort(tr("Copy: The root node can not bee copied.") );
        return;
    }

    CmdCopy* command = new CmdCopy(nodeIndex, m_coinNode_Buffer, m_sceneModel);
    m_commandStack->push(command);

    m_document->SetDocumentModified(true);
    return;

}

/*!
 * Creates a new group node as a selected node child.
 */
void MainWindow::CreateGroupNode()
{
    QModelIndex parentIndex;
    if ((!sceneModelView->currentIndex().isValid() ) || (sceneModelView->currentIndex() == sceneModelView->rootIndex()))
        parentIndex = m_sceneModel->index (0,0,sceneModelView->rootIndex());
    else
        parentIndex = sceneModelView->currentIndex();

    sceneModelView->expand(parentIndex);

    InstanceNode* parentInstance = m_sceneModel->NodeFromIndex(parentIndex);
    if (!parentInstance)
    {
        emit Abort(tr("CreateGroupNode: Error creating new group node.") );
        return;
    }

    SoNode* coinNode = parentInstance->GetNode();
    if (!coinNode)
    {
        emit Abort(tr("CreateGroupNode: Error creating new group node.") );
        return;
    }

    if (coinNode->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId() ) )
    {
        TSeparatorKit* separatorKit = new TSeparatorKit();

        CmdInsertSeparatorKit* cmdInsertSeparatorKit = new CmdInsertSeparatorKit(separatorKit, QPersistentModelIndex(parentIndex), m_sceneModel);
        cmdInsertSeparatorKit->setText("Insert node");
        m_commandStack->push(cmdInsertSeparatorKit);

        QString name("Node");
        int count = 0;
        while (!m_sceneModel->SetNodeName(separatorKit, name))
            name = QString("Node_%1").arg(++count);

        UpdateLightSize();
        m_document->SetDocumentModified(true);
    }
}

/*!
 * Creates a new analyzer node as a selected node child.
 */

/*!
 * Creates a \a componentType component node with the name \a nodeName.
 */
void MainWindow::CreateComponentNode(QString componentType, QString nodeName, int numberofParameters, QVector< QVariant > parametersList)
{
    QModelIndex parentIndex = ( (!sceneModelView->currentIndex().isValid() ) || (sceneModelView->currentIndex() == sceneModelView->rootIndex() ) ) ?
                m_sceneModel->index(0, 0, sceneModelView->rootIndex()) :
                sceneModelView->currentIndex();

    InstanceNode* parentInstance = m_sceneModel->NodeFromIndex(parentIndex);
    SoNode* parentNode = parentInstance->GetNode();
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
    m_document->SetDocumentModified(true);

    ComponentFactory* pComponentFactory = factoryList[selectedCompoent];
    if (!pComponentFactory) return;

    //CreateComponent( pComponentFactory );

    TSeparatorKit* componentRootNode = pComponentFactory->CreateTComponent(m_pluginManager, numberofParameters, parametersList);
    if (!componentRootNode) return;


    QString typeName = pComponentFactory->name();
    componentRootNode->setName(nodeName.toStdString().c_str() );

    TSceneKit* scene = m_document->GetSceneKit();
    TLightKit* lightKit = static_cast< TLightKit* >(scene->getPart("lightList[0]", false) );
    if (lightKit)
    {
        SoSearchAction trackersSearch;
        trackersSearch.setType(TrackerAbstract::getClassTypeId() );
        trackersSearch.setInterest(SoSearchAction::ALL);
        trackersSearch.apply(componentRootNode);
        SoPathList& trackersPath = trackersSearch.getPaths();

        for (int index = 0; index <trackersPath.getLength(); ++index)
        {
            SoFullPath* trackerPath = static_cast< SoFullPath* > (trackersPath[index]);
            TrackerAbstract* tracker = static_cast< TrackerAbstract* >(trackerPath->getTail() );
//            tracker->SetAzimuthAngle(&lightKit->azimuth);
//            tracker->SetZenithAngle(&lightKit->zenith);
        }
    }

    CmdInsertSeparatorKit* cmdInsertSeparatorKit = new CmdInsertSeparatorKit(componentRootNode, QPersistentModelIndex(parentIndex), m_sceneModel);
    QString commandText = QString("Create Component: %1").arg(pComponentFactory->name().toLatin1().constData() );
    cmdInsertSeparatorKit->setText(commandText);
    m_commandStack->push(cmdInsertSeparatorKit);

    UpdateLightSize();
    m_document->SetDocumentModified(true);
}

/*!
 * Creates a \a materialType material node from the as current selected node child.
 *
 * If the current node is not a surface type node or \a materialType is not a valid type, the material node will not be created.
 */
void MainWindow::CreateMaterial(QString materialType)
{
    QVector< MaterialFactory* > factoryList = m_pluginManager->getMaterialFactories();
    if (factoryList.size() == 0) return;

    QVector< QString > materialNames;
    for (int i = 0; i < factoryList.size(); i++)
        materialNames << factoryList[i]->name();

    int selectedMaterial = materialNames.indexOf(materialType);
    if (selectedMaterial < 0)
    {
        emit Abort(tr("CreateMaterial: Selected material type is not valid.") );
        return;
    }

    CreateMaterial(factoryList[selectedMaterial]);
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
        emit Abort(tr("CreateShape: Selected shape type is not valid."));
}


/*!
 * Creates a \a shapeType shape node from the as current selected node child with the parameters defined in \a parametersList. \a numberOfParameters is the
 * number of parametners in the vector \a numberOfParameters
 *
 * If the current node is not a valid parent node or \a shapeType is not a valid type, the shape node will not be created.
 *
 */
void MainWindow::CreateShape(QString shapeType, int numberOfParameters, QVector< QVariant > parametersList)
{
    QVector<ShapeFactory*> factoryList = m_pluginManager->getShapeFactories();
    if (factoryList.size() == 0) return;

    QVector< QString > shapeNames;
    for (int i = 0; i < factoryList.size(); i++)
        shapeNames << factoryList[i]->name();

    int selectedShape = shapeNames.indexOf(shapeType);
    if (selectedShape < 0)
    {
        emit Abort(tr("CreateShape: Selected shape type is not valid.") );
        return;
    }

    CreateShape(factoryList[selectedShape], numberOfParameters, parametersList);
}

/*!
 * Creates a surface node as selected node child.
 */
void MainWindow::CreateSurfaceNode()
{
    QModelIndex parentIndex;
    if ((!sceneModelView->currentIndex().isValid() ) || (sceneModelView->currentIndex() == sceneModelView->rootIndex()))
        parentIndex = m_sceneModel->index(0, 0, sceneModelView->rootIndex());
    else
        parentIndex = sceneModelView->currentIndex();

    sceneModelView->expand(parentIndex);

    InstanceNode* parentInstance = m_sceneModel->NodeFromIndex(parentIndex);
    if (!parentInstance) return;

    SoNode* selectedCoinNode = parentInstance->GetNode();
    if (!selectedCoinNode) return;

    if (selectedCoinNode->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId() ) )
    {
        TShapeKit* shapeKit = new TShapeKit;

        CmdInsertShapeKit* insertShapeKit = new CmdInsertShapeKit(parentIndex, shapeKit, m_sceneModel);
        m_commandStack->push(insertShapeKit);

        QString name("Shape");
        int count = 0;
        while (!m_sceneModel->SetNodeName(shapeKit, name))
            name = QString("Shape_%1").arg(++count);

        UpdateLightSize();
        m_document->SetDocumentModified(true);
    }
}


/*!
 * Creates a \a trackerType shape node from the as current selected node child.
 *
 * If the current node is not a valid parent node or \a trackerType is not a valid type, the shape node will not be created.
 *
 */
void MainWindow::CreateTracker(QString trackerType)
{
    QVector< TrackerFactory* > factoryList = m_pluginManager->getTrackerFactories();
    if (factoryList.size() == 0) return;

    QVector< QString > trackerNames;
    for (int i = 0; i < factoryList.size(); i++)
        trackerNames << factoryList[i]->name();

    int selectedTracker = trackerNames.indexOf(trackerType);
    if (selectedTracker < 0)
    {
        emit Abort(tr("CreateTracker: Selected tracker type is not valid.") );
        return;
    }

    CreateTracker(factoryList[selectedTracker]);
}

/*!
 * If there is a node selected and this node is not the root node, cuts current selected node from the model. Otherwise, nothing is done.
 */
void MainWindow::Cut()
{
    int validNode = 1;
    if (!m_selectionModel->hasSelection() ) validNode = 0;
    if (m_selectionModel->currentIndex() == sceneModelView->rootIndex() ) validNode = 0;
    if (m_selectionModel->currentIndex().parent() == sceneModelView->rootIndex() ) validNode = 0;
    if (!validNode)
    {
        emit Abort(tr("Cut: No valid node selected to cut.") );
        return;
    }
    CmdCut* cmd = new CmdCut(m_selectionModel->currentIndex(), m_coinNode_Buffer, m_sceneModel);
    m_commandStack->push(cmd);

    UpdateLightSize();
    m_document->SetDocumentModified(true);
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

    QModelIndex nodeIndex = m_sceneModel->IndexFromNodeUrl(nodeURL);

    if (!nodeIndex.isValid() )
    {
        emit Abort(tr("Cut: There is no node with defined url.") );
        return;
    }

    if (nodeIndex == sceneModelView->rootIndex() )
    {
        emit Abort(tr("Cut: Selected node is not valid node to cut.") );
        return;
    }
    if (nodeIndex.parent() == sceneModelView->rootIndex() )
    {
        emit Abort(tr("Cut: Selected node is not valid node to cut.") );
        return;
    }

    CmdCut* command = new CmdCut(nodeIndex, m_coinNode_Buffer, m_sceneModel);
    m_commandStack->push(command);

    UpdateLightSize();
    m_document->SetDocumentModified(true);
}

/*!
 * * Deletes selected node if there is a valid node to delete.
 */
void MainWindow::Delete()
{
    if (!m_selectionModel->hasSelection() )
    {
        emit Abort(tr("Delete: There is no node selected to delete.") );
        return;
    }

    QModelIndex selection = m_selectionModel->currentIndex();
    m_selectionModel->clearSelection();

    InstanceNode* selectionNode = m_sceneModel->NodeFromIndex(selection);
    m_selectionModel->setCurrentIndex(m_sceneModel->IndexFromNodeUrl(selectionNode->GetParent()->GetNodeURL() ), QItemSelectionModel::ClearAndSelect);

    Delete(selection);
}

/*!
 * Creates a new delete command, where the node with the \a nodeURL was deleted.
 *
 * If \a nodeURL is not a valid node url, nothing is done.
 */
void MainWindow::Delete(QString nodeURL)
{
    QModelIndex nodeIndex = m_sceneModel->IndexFromNodeUrl(nodeURL);
    if (!nodeIndex.isValid() )
    {
        emit Abort(tr("Delete: There is no node with defined url.") );
        return;
    }

    Delete(nodeIndex);
    if (m_selectionModel->isSelected (nodeIndex) ) m_selectionModel->clearSelection();
}

/*!
 * Creates a new delete command, where the \a index node was deleted.
 *
 * Returns \a true if the node was successfully deleted, otherwise returns \a false.
 */
bool MainWindow::Delete(QModelIndex index)
{
    if (!index.isValid()) return false;
    if (index == sceneModelView->rootIndex() ) return false;
    if (index.parent() == sceneModelView->rootIndex() ) return false;

    InstanceNode* instanceNode = m_sceneModel->NodeFromIndex(index);
    SoNode* coinNode = instanceNode->GetNode();

    if (coinNode->getTypeId().isDerivedFrom(TrackerAbstract::getClassTypeId() ) )
    {
        CmdDeleteTracker* commandDelete = new CmdDeleteTracker(index, m_document->GetSceneKit(), *m_sceneModel);
        m_commandStack->push(commandDelete);
    }
    else if (coinNode->getTypeId().isDerivedFrom(TLightKit::getClassTypeId() ) ) return false;
    else
    {
        CmdDelete* commandDelete = new CmdDelete(index, *m_sceneModel);

        m_commandStack->push(commandDelete);
    }


    UpdateLightSize();
    m_document->SetDocumentModified(true);

    return true;
}

/*!
 * Save all photon map data into \a fileName file.
 */
double MainWindow::GetwPhoton(){
    //Compute photon power
    TSceneKit* coinScene = m_document->GetSceneKit();
    if (!coinScene) return 0;
    if (!coinScene->getPart("lightList[0]", false) ) return 0;
    TLightKit* lightKit = static_cast< TLightKit* >(coinScene->getPart("lightList[0]", false) );


    if (!lightKit->getPart("tsunshape", false) ) return 0;
    SunAbstract* sunShape = static_cast< SunAbstract* >(lightKit->getPart("tsunshape", false) );
    double irradiance = sunShape->getIrradiance();

    if (!lightKit->getPart("icon", false) ) return 0;
    TLightShape* raycastingShape = static_cast< TLightShape* >(lightKit->getPart("icon", false) );
    double inputAperture = raycastingShape->GetValidArea();

    return double(inputAperture * irradiance) / m_raysTraced;
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
    if ((!sceneModelView->currentIndex().isValid() ) || (sceneModelView->currentIndex() == sceneModelView->rootIndex() ) )
        parentIndex = m_sceneModel->index (0,0,sceneModelView->rootIndex());
    else
        parentIndex = sceneModelView->currentIndex();

    SoNode* coinNode = m_sceneModel->NodeFromIndex(parentIndex)->GetNode();
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

    TSeparatorKit* componentRootNode = static_cast< TSeparatorKit* >(componentSeparator->getChild(0) );
    TSceneKit* scene = m_document->GetSceneKit();
    TLightKit* lightKit = static_cast< TLightKit* >(scene->getPart("lightList[0]", false) );
    if (lightKit)
    {

        SoSearchAction trackersSearch;
        trackersSearch.setType(TrackerAbstract::getClassTypeId() );
        trackersSearch.setInterest(SoSearchAction::ALL);
        trackersSearch.apply(componentRootNode);
        SoPathList& trackersPath = trackersSearch.getPaths();

        for (int index = 0; index <trackersPath.getLength(); ++index)
        {
            SoFullPath* trackerPath = static_cast< SoFullPath* > (trackersPath[index]);
            TrackerAbstract* tracker = static_cast< TrackerAbstract* >(trackerPath->getTail() );
//            tracker->SetAzimuthAngle(&lightKit->azimuth);
//            tracker->SetZenithAngle(&lightKit->zenith);
        }
    }

    CmdInsertSeparatorKit* cmdInsertSeparatorKit = new CmdInsertSeparatorKit(componentRootNode, QPersistentModelIndex(parentIndex), m_sceneModel);
    cmdInsertSeparatorKit->setText("Insert SeparatorKit node");
    m_commandStack->push(cmdInsertSeparatorKit);

    UpdateLightSize();
    m_document->SetDocumentModified(true);
}

/*!
 * Starts new tonatiuh empty model.
 */

void MainWindow::New()
{
    if (!OkToContinue() )
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

    QModelIndex nodeIndex = m_sceneModel->IndexFromNodeUrl(nodeURL);
    if (!nodeIndex.isValid() )
    {
        emit Abort(tr("Paste: The node url is not valid.") );
        return;
    }

    if (pasteType == QString("Shared") )
        Paste(nodeIndex, tgc::Shared);
    else
        Paste(nodeIndex, tgc::Copied);
}


/*!
 * Inserts a new node as a current node child. The new node is a copy to node saved into the clipboard.
 */
void MainWindow::PasteCopy()
{
    if (!m_selectionModel->hasSelection() )
    {
        emit Abort(tr("PasteCopy: There is not node copied.") );
        return;
    }
    Paste(m_selectionModel->currentIndex(), tgc::Copied);
}

/*!
 * Inserts a new node as a current node child. The new node is a link to node saved into the clipboard.
 */
void MainWindow::PasteLink()
{
    if (!m_selectionModel->hasSelection() )
    {
        emit Abort(tr("PasteCopy: There is not node copied.") );
        return;
    }
    Paste(m_selectionModel->currentIndex(), tgc::Shared);
}

/*!
 * Runs ray tracer to defined model and paramenters.
 */
void MainWindow::Run()
{
    InstanceNode* rootSeparatorInstance = 0;
    InstanceNode* lightInstance = 0;
    SoTransform* lightTransform = 0;
    SunAbstract* sunShape = 0;
    TLightShape* raycastingSurface = 0;
    AirAbstract* transmissivity = 0;

    QElapsedTimer timer;
    timer.start();

    if (ReadyForRaytracing(rootSeparatorInstance, lightInstance, lightTransform, sunShape, raycastingSurface, transmissivity) )
    {
        if (!m_pPhotonMap->GetExportMode() )
        {
            if (!m_pExportModeSettings) return;
            else
            {
                PhotonExport* pExportMode = CreatePhotonMapExport();
                if (!pExportMode) return;
                if (!m_pPhotonMap->SetExportMode(pExportMode)  ) return;
            }
        }

        QVector<InstanceNode*> exportSuraceList;
        QStringList exportSurfaceURLList = m_pExportModeSettings->exportSurfaceNodeList;
        for (int s = 0; s < exportSurfaceURLList.count(); s++)
        {
            m_sceneModel->IndexFromNodeUrl(exportSurfaceURLList[s]);
            InstanceNode* surfaceNode = m_sceneModel->NodeFromIndex(m_sceneModel->IndexFromNodeUrl(exportSurfaceURLList[s]) );
            exportSuraceList.push_back(surfaceNode);
        }


        UpdateLightSize();

        //Compute bounding boxes and world to object transforms
        trf::ComputeSceneTreeMap(rootSeparatorInstance, Transform(new Matrix4x4), true);

        m_pPhotonMap->SetConcentratorToWorld(rootSeparatorInstance->GetIntersectionTransform() );

        TLightKit* light = static_cast< TLightKit* > (lightInstance->GetNode() );
        QStringList disabledNodes = QString(light->disabledNodes.getValue().getString() ).split(";", QString::SkipEmptyParts);
        QVector< QPair< TShapeKit*, Transform > > surfacesList;
        trf::ComputeFistStageSurfaceList(rootSeparatorInstance, disabledNodes, &surfacesList);
        light->ComputeLightSourceArea(m_widthDivisions, m_heightDivisions, surfacesList);
        if (surfacesList.count() < 1)
        {
            emit Abort(tr("There are no surfaces defined for ray tracing") );

            ShowRaysIn3DView();
            return;
        }

        QVector<long> raysPerThread;
        int progressMax = 100;
        ulong t1 = m_raysTraced / progressMax;
        for (int progress = 0; progress < progressMax; ++progress)
            raysPerThread << t1;

        if (t1*progressMax < m_raysTraced)
            raysPerThread << m_raysTraced - (t1*progressMax);


        Transform lightToWorld = tgf::TransformFromSoTransform(lightTransform);
        lightInstance->SetIntersectionTransform(lightToWorld.GetInverse() );


        // single thread for gprof
//        QThreadPool::globalInstance()->setMaxThreadCount(1);
// change ideal

        // Create a progress dialog.
        QProgressDialog progressDialog;
        progressDialog.setLabelText(QString("Progressing using %1 thread(s)...").arg(QThread::idealThreadCount() ) );

        // Create a QFutureWatcher and conncect signals and slots.
        QFutureWatcher<void> futureWatcher;
        QObject::connect(&futureWatcher, SIGNAL(finished()), &progressDialog, SLOT(reset()));
        QObject::connect(&progressDialog, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));
        QObject::connect(&futureWatcher, SIGNAL(progressRangeChanged(int, int)), &progressDialog, SLOT(setRange(int, int)));
        QObject::connect(&futureWatcher, SIGNAL(progressValueChanged(int)), &progressDialog, SLOT(setValue(int)));

        std::cout << "QtConcurrent started: " << timer.elapsed() << std::endl;
        QMutex mutex;
        QMutex mutexPhotonMap;
        QFuture<void> photonMap;
        AirAbstract* airTemp = 0;
        if (!dynamic_cast<AirVacuum*>(transmissivity))
            airTemp = transmissivity;

        photonMap = QtConcurrent::map(raysPerThread, RayTracer(rootSeparatorInstance,
            lightInstance, raycastingSurface, sunShape, lightToWorld,
            airTemp,
            *m_rand,
            &mutex, m_pPhotonMap, &mutexPhotonMap,
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
            actionViewRays->setEnabled(false);
            actionViewRays->setChecked(false);
        }

        double irradiance = sunShape->getIrradiance();
        double inputAperture = raycastingSurface->GetValidArea();
        double wPhoton = (inputAperture * irradiance) / m_raysTracedTotal;

        m_pPhotonMap->EndStore(wPhoton);
    }

    std::cout << "Elapsed time (Run): " << timer.elapsed() << std::endl;
}

/*
 * Runs ray trace to calculate a flux distribution map in the surface of the node \a nodeURL related to the side \a surfaceSide.
 * The map will be calculated with the parameters \a nOfRays, \a heightDivisions and \a heightDivisions.
 * The results will save in a file \a directory \a QString fileName, the coordinates of the cells depending on the variable \a saveCoord.
 */
void MainWindow::RunFluxAnalysis(QString nodeURL, QString surfaceSide, unsigned int nOfRays, int heightDivisions, int widthDivisions, QString directory, QString fileName, bool saveCoords)
{
    TSceneKit* coinScene = m_document->GetSceneKit();
    if (!coinScene) return;

    TLightKit* lightKit = static_cast< TLightKit* >(coinScene->getPart("lightList[0]", false) );
    if (!lightKit) return;

    InstanceNode*  rootSeparatorInstance = m_sceneModel->NodeFromIndex(sceneModelView->rootIndex() );
    if (!rootSeparatorInstance) return;

    QVector< RandomFactory* > randomDeviateFactoryList = m_pluginManager->getRandomFactories();
    //Check if there is a random generator selected;
    if (m_selectedRandomDeviate == -1)
    {
        if (randomDeviateFactoryList.size() > 0) m_selectedRandomDeviate = 0;
        else return;
    }

    //Create the random generator
    if (!m_rand) m_rand =  randomDeviateFactoryList[m_selectedRandomDeviate]->create();

    FluxAnalysis fluxAnalysis(coinScene, *m_sceneModel, rootSeparatorInstance, m_widthDivisions, m_heightDivisions, m_rand);

    fluxAnalysis.RunFluxAnalysis(nodeURL, surfaceSide, nOfRays, false, heightDivisions, widthDivisions);

    int** photonCounts = fluxAnalysis.photonCountsValue();
    if (!photonCounts || photonCounts == 0)
    {
        emit Abort(tr("RunFluxAnalysis: Some parameter is not correctly defined.") );
        return;
    }

    fluxAnalysis.ExportAnalysis(directory, fileName, saveCoords);
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
void MainWindow::SelectNode(QString nodeUrl)
{
    QModelIndex nodeIndex = m_sceneModel->IndexFromNodeUrl(nodeUrl);
    m_selectionModel->setCurrentIndex(nodeIndex, QItemSelectionModel::ClearAndSelect);
}

/*!
 * Sets to export all surfaces photons.
 */
void MainWindow::SetExportAllPhotonMap()
{

    if (!m_pExportModeSettings) return;
    m_pExportModeSettings->exportSurfaceNodeList.clear();

}

/*!
 * If \a enabled is true, sets to export photons coordinates. Otherwise, the photons will not be exported.
 * If \a global is true, the coordinates will be exported in global coordinate system. Otherwise, into local to surface.
 */
void MainWindow::SetExportCoordinates(bool enabled, bool global)
{
    if (!m_pExportModeSettings) return;
    m_pExportModeSettings->exportCoordinates = enabled;
    m_pExportModeSettings->exportInGlobalCoordinates = global;

}

/*!
 * If \a enabled is true, sets to export intersection surface.
 * Otherwise, the intersection surface will not be exported.
 */
void MainWindow::SetExportIntersectionSurface(bool enabled)
{
    if (!m_pExportModeSettings) return;
    m_pExportModeSettings->exportSurfaceID = enabled;
}

/*!
 * If \a enabled is true, sets to export intersection surface side.
 * Otherwise, the intersection surface side will not be exported.
 */
void MainWindow::SetExportIntersectionSurfaceSide(bool enabled)
{
    if (!m_pExportModeSettings) return;
    m_pExportModeSettings->exportIntersectionSurfaceSide = enabled;
}

/*!
 * Sets the export photon mode type to \a exportModeType, for photon map.
 */
void MainWindow::SetExportPhotonMapType(QString exportModeType)
{
    QVector< PhotonExportFactory* > factoryList = m_pluginManager->getExportFactories();
    if (factoryList.size() == 0) return;

    QVector< QString > exportPMModeNames;
    for (int i = 0; i < factoryList.size(); i++)
        exportPMModeNames << factoryList[i]->name();

    if (exportPMModeNames.indexOf(exportModeType) < 0)
    {
        emit Abort(tr("exportModeType: Defined export mode is not valid type.") );
        return;
    }

    if (!m_pExportModeSettings)
        m_pExportModeSettings = new PhotonExportSettings;

    m_pExportModeSettings->modeTypeName = exportModeType;
}

/*!
 * If \a enabled is true, sets to export for each photon the prevous and next photoan id.
 * Otherwise, this information will not be exported.
 */
void MainWindow::SetExportPreviousNextPhotonID(bool enabled)
{
    if (!m_pExportModeSettings) return;
    m_pExportModeSettings->exportPreviousNextPhotonID = enabled;
}

/*!
 * Sets to selected export type parameter named \a parameterName the value \a parameterValue.
 */
void MainWindow::SetExportTypeParameterValue(QString parameterName, QString parameterValue)
{
    if (!m_pExportModeSettings) return;
    m_pExportModeSettings->AddParameter(parameterName, parameterValue);

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
void MainWindow::SetNodeName(QString nodeName)
{
    if (!m_selectionModel->hasSelection() )
    {
        emit Abort(tr("SetNodeName: No node selected.") );
        return;
    }

    if (m_selectionModel->currentIndex() == sceneModelView->rootIndex() )
    {
        emit Abort(tr("SetNodeName: Cannot change the name of the current selected node cannot.") );
        return;
    }

    ChangeNodeName(m_selectionModel->currentIndex(), nodeName);
}

/*!
 * Sets the number of photons that the photon map can store to \a nPhotons.
 */
void MainWindow::SetPhotonMapBufferSize(unsigned int nPhotons)
{
    m_bufferPhotons = nPhotons;
}

/*!
 * Sets the random number generator type, \a typeName, for ray tracing.
 */
void MainWindow::SetRandomDeviateType(QString typeName)
{
    QVector< RandomFactory* > factoryList = m_pluginManager->getRandomFactories();
    if (factoryList.size() == 0) return;

    QVector< QString > randomNames;
    for (int i = 0; i < factoryList.size(); i++)
        randomNames << factoryList[i]->name();

    int oldSelectedRandomDeviate = m_selectedRandomDeviate;

    if (randomNames.indexOf(typeName) < 0)
    {
        emit Abort(tr("SetRandomDeviateType: Defined random generator is not valid type.") );
        return;
    }
    m_selectedRandomDeviate = randomNames.indexOf(typeName);
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
void MainWindow::SetRaysPerIteration(unsigned int rays)
{
    m_raysTraced = rays;
}

/*!
 *    Set selected sunshape, \a sunshapeType, to the sun.
 */
void MainWindow::SetSunshape(QString sunshapeType)
{
    SoSceneKit* coinScene = m_document->GetSceneKit();
    TLightKit* lightKit = 0;
    if (coinScene->getPart("lightList[0]", false) )
    {
        lightKit = static_cast< TLightKit* >(coinScene->getPart("lightList[0]", false) );
    }
    else
        lightKit = new TLightKit;


    QVector< SunFactory* > factoryList = m_pluginManager->getSunFactories();

    if (factoryList.size() == 0) return;

    QVector< QString > factoryNames;
    for (int i = 0; i < factoryList.size(); i++)
    {
        QString name = factoryList[i]->name();
        factoryNames << name;
    }

    int selectedSunShapeIndex = factoryNames.indexOf(sunshapeType);
    if (selectedSunShapeIndex < 0)
    {
        emit Abort(tr("SetSunshape: Defined sunshape is not valid type.") );
        return;
    }

    lightKit->setPart("tsunshape", factoryList[selectedSunShapeIndex]->create() );

    CmdLightKitModified* command = new CmdLightKitModified(lightKit, coinScene, *m_sceneModel);
    m_commandStack->push(command);

    //UpdateLightDimensions();
    UpdateLightSize();

    parametersTabs->UpdateView();
    m_document->SetDocumentModified(true);

    actionCalculateSunPosition->setEnabled(true);
}

/*!
 * Set the \a value for the sunshape parameter \a parameter.
 */
void MainWindow::SetSunshapeParameter(QString parameter, QString value)
{
    SoSceneKit* coinScene = m_document->GetSceneKit();
    TLightKit* lightKit = static_cast< TLightKit* >(coinScene->getPart("lightList[0]", false) );
    if (!lightKit)
    {
        emit Abort(tr("SetSunshapeParameter: There is not light defined.") );
        return;
    }

    SunAbstract* sunshape = static_cast< SunAbstract* > (lightKit->getPart("tsunshape", false) );
    if (!sunshape)
    {
        emit Abort(tr("SetSunshapeParameter: There is not sunshape defined.") );
        return;
    }

    CmdModifyParameter* command = new CmdModifyParameter(sunshape, parameter, value, m_sceneModel);
    if (m_commandStack) m_commandStack->push(command);
    m_document->SetDocumentModified(true);
}

/*!
 *    Set selected transmissivity, \a transmissivityType, to the scene.
 */
void MainWindow::SetTransmissivity(QString transmissivityType)
{
    TSceneKit* coinScene = m_document->GetSceneKit();
    if (!coinScene)
    {
        emit Abort(tr("SetTransmissivity: Error defining transmissivity.") );
        return;
    }
    QVector< AirFactory* > transmissivityFactoryList = m_pluginManager->getAirFactories();
    if (transmissivityFactoryList.count() < 1)
    {
        emit Abort(tr("SetTransmissivity: Error defining transmissivity.") );
        return;
    }

    QStringList factoryNames;
    for (int i = 0; i < transmissivityFactoryList.count(); i++)
    {
        QString name = transmissivityFactoryList[i]->name();
        factoryNames << name;
    }


    int transmissivityIndex = factoryNames.indexOf(transmissivityType);
    if (transmissivityIndex < 0)
    {
        emit Abort(tr("SetTransmissivity: Defined transmissivity is not valid type.") );
        return;
    }

    AirAbstract* transmissivity = transmissivityFactoryList[ transmissivityIndex ]->create();
    if (!transmissivity)
    {
        emit Abort(tr("SetTransmissivity: Error defining transmissivity.") );
        return;
    }

    CmdAirModified* command = new CmdAirModified(transmissivity, coinScene);
    if (m_commandStack) m_commandStack->push(command);
    m_document->SetDocumentModified(true);
}

/*!
 * Set the \a value for the transmissivity parameter \a parameter.
 */
void MainWindow::SetTransmissivityParameter(QString parameter, QString value)
{
    SoSceneKit* coinScene = m_document->GetSceneKit();
    AirAbstract* transmissivity = static_cast< AirAbstract* >(coinScene->getPart("transmissivity", false) );
    if (!transmissivity)
    {
        emit Abort(tr("SetTransmissivity: No transmissivity type defined.") );
        return;
    }

    CmdModifyParameter* command = new CmdModifyParameter(transmissivity, parameter, value, m_sceneModel);
    if (m_commandStack) m_commandStack->push(command);
    m_document->SetDocumentModified(true);
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

    QModelIndex nodeIndex = m_sceneModel->IndexFromNodeUrl(nodeUrl);
    if (!nodeIndex.isValid() )
    {
        emit Abort(tr("SetValue: Defined node url is not a valid url.") );
        return;
    }

    InstanceNode* nodeInstance = m_sceneModel->NodeFromIndex(nodeIndex);
    if (!nodeInstance)
    {
        emit Abort(tr("SetValue: Defined node url is not a valid url.") );
        return;
    }

    SoNode* node = nodeInstance->GetNode();
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
    QModelIndex currentIndex = sceneModelView->currentIndex();

    InstanceNode* instanceNode = m_sceneModel->NodeFromIndex(currentIndex);
    SoBaseKit* coinNode = static_cast< SoBaseKit* >(instanceNode->GetNode() );
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

    m_document->SetDocumentModified(true);
}

void MainWindow::SoTransform_to_SoJackManip()
{
    //Transform to a SoJackManip manipulator
    QModelIndex currentIndex = sceneModelView->currentIndex();

    InstanceNode* instanceNode = m_sceneModel->NodeFromIndex(currentIndex);
    SoBaseKit* coinNode = static_cast< SoBaseKit* >(instanceNode->GetNode() );
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

    m_document->SetDocumentModified(true);
}

void MainWindow::SoTransform_to_SoHandleBoxManip()
{
    //Transform to a SoHandleBoxManip manipulator
    QModelIndex currentIndex = sceneModelView->currentIndex();

    InstanceNode* instanceNode = m_sceneModel->NodeFromIndex(currentIndex);
    SoBaseKit* coinNode = static_cast< SoBaseKit* >(instanceNode->GetNode() );
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

    m_document->SetDocumentModified(true);
}

void MainWindow::SoTransform_to_SoTabBoxManip()
{
    //Transform to a SoTabBoxManip manipulator
    QModelIndex currentIndex = sceneModelView->currentIndex();

    InstanceNode* instanceNode = m_sceneModel->NodeFromIndex(currentIndex);
    SoBaseKit* coinNode = static_cast< SoBaseKit* >(instanceNode->GetNode() );
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

    m_document->SetDocumentModified(true);
}

void MainWindow::SoTransform_to_SoTrackballManip()
{
    //Transform to a SoTrackballManip manipulator
    QModelIndex currentIndex = sceneModelView->currentIndex();

    InstanceNode* instanceNode = m_sceneModel->NodeFromIndex(currentIndex);
    SoBaseKit* coinNode = static_cast< SoBaseKit* >(instanceNode->GetNode() );
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

    m_document->SetDocumentModified(true);
}

void MainWindow::SoTransform_to_SoTransformBoxManip()
{
    //Transform to a SoTransformBoxManip manipulator
    QModelIndex currentIndex = sceneModelView->currentIndex();

    InstanceNode* instanceNode = m_sceneModel->NodeFromIndex(currentIndex);
    SoBaseKit* coinNode = static_cast< SoBaseKit* >(instanceNode->GetNode() );
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

    m_document->SetDocumentModified(true);
}

void MainWindow::SoTransform_to_SoTransformerManip()
{
    //Transform to a SoTransformerManip manipulator
    QModelIndex currentIndex = sceneModelView->currentIndex();

    InstanceNode* instanceNode = m_sceneModel->NodeFromIndex(currentIndex);
    SoBaseKit* coinNode = static_cast< SoBaseKit* >(instanceNode->GetNode() );
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

    m_document->SetDocumentModified(true);

}

void MainWindow::SoManip_to_SoTransform()
{
    //Transform manipulator to a SoTransform
    QModelIndex currentIndex = sceneModelView->currentIndex();

    InstanceNode* instanceNode = m_sceneModel->NodeFromIndex(currentIndex);
    SoBaseKit* coinNode = static_cast< SoBaseKit* >(instanceNode->GetNode() );
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

    m_document->SetDocumentModified(true);
}

void MainWindow::ChangeSelection(const QModelIndex& current)
{
    InstanceNode* instanceSelected = m_sceneModel->NodeFromIndex(current);
    SoNode* selectedCoinNode = instanceSelected->GetNode();

    if (selectedCoinNode->getTypeId().isDerivedFrom(SoBaseKit::getClassTypeId() ) )
    {
        SoBaseKit* selectedCoinNodeKit = static_cast< SoBaseKit* >(selectedCoinNode);
        parametersTabs->SelectionChangedToKit(selectedCoinNodeKit);
    }
    else
    {
        parametersTabs->SelectionChangedToPart(selectedCoinNode);
    }
}

/*!
 * Creates a componet subtree from the \a pComponentFactory as current selected node child.
 *
 * If the current node is not a group type node, the component subtree will not be created.
 */
void MainWindow::CreateComponent(ComponentFactory* pComponentFactory)
{
    QModelIndex parentIndex = ( (!sceneModelView->currentIndex().isValid() ) || (sceneModelView->currentIndex() == sceneModelView->rootIndex() ) ) ?
                m_sceneModel->index(0, 0, sceneModelView->rootIndex()) :
                sceneModelView->currentIndex();

    InstanceNode* parentInstance = m_sceneModel->NodeFromIndex(parentIndex);
    SoNode* parentNode = parentInstance->GetNode();
    if (!parentNode->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId() ) ) return;

    TSeparatorKit* componentRootNode = pComponentFactory->CreateTComponent(m_pluginManager);
    if (!componentRootNode) return;

    QString typeName = pComponentFactory->name();
    componentRootNode->setName(typeName.toStdString().c_str() );

//    TSceneKit* scene = m_document->GetSceneKit();
//    TLightKit* lightKit = static_cast< TLightKit* >(scene->getPart("lightList[0]", false) );
//    if (lightKit)
//    {

//        SoSearchAction trackersSearch;
//        trackersSearch.setType(TTracker::getClassTypeId() );
//        trackersSearch.setInterest(SoSearchAction::ALL);
//        trackersSearch.apply(componentRootNode);
//        SoPathList& trackersPath = trackersSearch.getPaths();

//        for (int index = 0; index <trackersPath.getLength(); ++index)
//        {
//            SoFullPath* trackerPath = static_cast< SoFullPath* > (trackersPath[index]);
//            TTracker* tracker = static_cast< TTracker* >(trackerPath->getTail() );
//            //tracker->SetAzimuthAngle( &lightKit->azimuth );
//            //tracker->SetZenithAngle( &lightKit->zenith );
//        }
//    }



    CmdInsertSeparatorKit* cmdInsertSeparatorKit = new CmdInsertSeparatorKit(componentRootNode, QPersistentModelIndex(parentIndex), m_sceneModel);
    QString commandText = QString("Create Component: %1").arg(pComponentFactory->name().toLatin1().constData() );
    cmdInsertSeparatorKit->setText(commandText);
    m_commandStack->push(cmdInsertSeparatorKit);

    UpdateLightSize();
    m_document->SetDocumentModified(true);
}


/*!
 * Creates a material node from the \a pMaterialFactory as current selected node child.
 *
 * If the current node is not a surface type node, the material node will not be created.
 */
void MainWindow::CreateMaterial(MaterialFactory* pMaterialFactory)
{
    QModelIndex parentIndex = ( (!sceneModelView->currentIndex().isValid() ) || (sceneModelView->currentIndex() == sceneModelView->rootIndex() ) ) ?
                m_sceneModel->index(0, 0, sceneModelView->rootIndex()) :
                sceneModelView->currentIndex();

    sceneModelView->expand(parentIndex);

    InstanceNode* parentInstance = m_sceneModel->NodeFromIndex(parentIndex);
    SoNode* parentNode = parentInstance->GetNode();
    if (!parentNode->getTypeId().isDerivedFrom(SoShapeKit::getClassTypeId() ) ) return;

    TShapeKit* shapeKit = static_cast< TShapeKit* >(parentNode);
    MaterialAbstract* material = static_cast< MaterialAbstract* >(shapeKit->getPart("material", false) );

    if (material)
    {
        QMessageBox::information(this, "Tonatiuh Action",
                                 "This TShapeKit already contains a material node", 1);
        return;
    }

    material = pMaterialFactory->create();
    QString typeName = pMaterialFactory->name();
    material->setName(typeName.toStdString().c_str() );

    CmdInsertMaterial* createMaterial = new CmdInsertMaterial(shapeKit, material, m_sceneModel);
    QString commandText = QString("Create Material: %1").arg(pMaterialFactory->name().toLatin1().constData() );
    createMaterial->setText(commandText);
    m_commandStack->push(createMaterial);

    UpdateLightSize();
    m_document->SetDocumentModified(true);
}

/*!
 * Creates a shape node from the \a pShapeFactory as current selected node child.
 *
 * If the current node is not a surface type node, the shape node will not be created.
 */
/*!
 * Creates a shape node from the \a pTrackerFactory as current selected node child.
 *
 */
void MainWindow::CreateShape(ShapeFactory* factory)
{
    QModelIndex parentIndex = sceneModelView->currentIndex();
    if (!parentIndex.isValid()) return;
    sceneModelView->expand(parentIndex);

    InstanceNode* parentInstance = m_sceneModel->NodeFromIndex(parentIndex);
    SoNode* parentNode = parentInstance->GetNode();
    if (!parentNode->getTypeId().isDerivedFrom(SoShapeKit::getClassTypeId() ) ) return;
    TShapeKit* shapeKit = static_cast<TShapeKit*>(parentNode);

    ShapeAbstract* shape = static_cast<ShapeAbstract*>(shapeKit->getPart("shape", false) );
    if (shape) {
        QMessageBox::information(this, "Tonatiuh Action",
                                 "This TShapeKit already contains a shape", 1);
        return;
    }

    shape = factory->create();
    shape->setName(factory->name().toStdString().c_str());

    CmdInsertShape* cmd = new CmdInsertShape(shapeKit, shape, m_sceneModel);
    QString text = QString("Create Shape: %1").arg(factory->name());
    cmd->setText(text);
    m_commandStack->push(cmd);

    UpdateLightSize();
    m_document->SetDocumentModified(true);
}

/*!
 * Creates a shape node from the \a pShapeFactory as current selected node child.
 *
 * If the current node is not a surface type node, the shape node will not be created.
 */
/*!
 * Creates a shape node from the \a pTrackerFactory as current selected node child.
 *
 */
void MainWindow::CreateShape(ShapeFactory* pShapeFactory, int /*numberofParameters*/, QVector< QVariant > parametersList)
{
    QModelIndex parentIndex = ((!sceneModelView->currentIndex().isValid() ) || (sceneModelView->currentIndex() == sceneModelView->rootIndex())) ?
                m_sceneModel->index (0,0,sceneModelView->rootIndex()) : sceneModelView->currentIndex();

    InstanceNode* parentInstance = m_sceneModel->NodeFromIndex(parentIndex);
    SoNode* parentNode = parentInstance->GetNode();
    if (!parentNode->getTypeId().isDerivedFrom(SoShapeKit::getClassTypeId() ) ) return;

    TShapeKit* shapeKit = static_cast< TShapeKit* >(parentNode);
    ShapeAbstract* shape = static_cast< ShapeAbstract* >(shapeKit->getPart("shape", false) );

    if (shape)
    {
        QMessageBox::information(this, "Tonatiuh Action",
                                 "This TShapeKit already contains a shape", 1);
    }
    else
    {
        shape = pShapeFactory->create(parametersList);
        /*Hay que comprobar si shape es nulo para no crear una superficie a partir de nulo que probacara el cierre de la aplicacion.*/
        if (shape!=0) {
            shape->setName(pShapeFactory->name().toStdString().c_str() );
            CmdInsertShape* createShape = new CmdInsertShape(shapeKit, shape, m_sceneModel);
            QString commandText = QString("Create Shape: %1").arg(pShapeFactory->name().toLatin1().constData());
            createShape->setText(commandText);
            m_commandStack->push(createShape);

            UpdateLightSize();
            m_document->SetDocumentModified(true);
        }
    }
}


void MainWindow::CreateTracker(TrackerFactory* pTrackerFactory)
{
    QModelIndex parentIndex = ((!sceneModelView->currentIndex().isValid() ) || (sceneModelView->currentIndex() == sceneModelView->rootIndex())) ?
                m_sceneModel->index (0,0,sceneModelView->rootIndex()) :
                sceneModelView->currentIndex();

    TSceneKit* scene = m_document->GetSceneKit();

    /*Las 2 lineas siguientes son para obtener el valor del nodo padre*/
    InstanceNode* parentInstance = m_sceneModel->NodeFromIndex(parentIndex);
    SoNode* parentNode = parentInstance->GetNode();
    /*Si el valor del nodo padre es del tipo TseparatorKit, permitimos la creacion del Traker**/
    if (parentNode->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId() ) ) {
        /**/
        TSeparatorKit* separatorKit = static_cast< TSeparatorKit* >(parentNode);
        TrackerAbstract* trak = static_cast< TrackerAbstract* >(separatorKit->getPart("tracker", false) );

        if (trak)
        {
            QMessageBox::information(this, "Tonatiuh Action",
                                     "This TSeparatorKit already contains a tracker node", 1);
            return;
        }
        TrackerAbstract* tracker = pTrackerFactory->create();

        tracker->SetSceneKit(scene);
        tracker->setName(pTrackerFactory->name().toStdString().c_str() );
        CmdInsertTracker* command = new CmdInsertTracker(tracker, parentIndex, scene, m_sceneModel);
        m_commandStack->push(command);

        UpdateLightSize();
        m_document->SetDocumentModified(true);
    }
    /*En caso de que el valor del nodo padre no sea del tipo TSeparatorKit,
         * no realizamos ninguna modicacion en SceneModel*/
    else return;
    /**/
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (OkToContinue() )
    {
        WriteSettings();
        event->accept();
    }
    else event->ignore();
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

    if (mainViewRect.contains(x, y) )
    {
        QSplitter* pvSplitter1 = findChild<QSplitter*>("graphicSplitterH1");
        QSplitter* pvSplitter2 = findChild<QSplitter*>("graphicSplitterH1");
        QRect vViewRect1 = pvSplitter1->geometry();
        if (vViewRect1.contains(x,y))
        {
            QList<int> size(pvSplitter2->sizes());
            if (x < size[0])
                m_focusView = 0;
            if (x > size[0])
                m_focusView = 1;
        }
        else
        {
            QList<int> size(pvSplitter1->sizes());
            if (x < size[0])
                m_focusView = 2;
            if (x > size[0])
                m_focusView = 3;
        }
    }
}

/*!
 * Set current document coin scene into Tonatiuh Models and views.
 */
void MainWindow::ChangeModelScene()
{
    TSceneKit* coinScene = m_document->GetSceneKit();

    m_sceneModel->SetCoinScene(*coinScene);
    m_graphicsRoot->AddModel(coinScene);

    QModelIndex viewRootNodeIndex = m_sceneModel->IndexFromNodeUrl(QString("//SunNode") );
    InstanceNode* viewRootNode = m_sceneModel->NodeFromIndex(viewRootNodeIndex);
    sceneModelView->setRootIndex(viewRootNodeIndex);

    InstanceNode* concentratorRoot = viewRootNode->children[ 0 ];

    m_selectionModel->setCurrentIndex(m_sceneModel->IndexFromNodeUrl(concentratorRoot->GetNodeURL() ), QItemSelectionModel::ClearAndSelect);
}

/*!
 * Shows a dialog to allow define current light position with a position calculator.
 */
void MainWindow::CalculateSunPosition()
{
#ifndef NO_MARBLE

    SoSceneKit* coinScene = m_document->GetSceneKit();
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
PhotonExport* MainWindow::CreatePhotonMapExport() const
{
    QVector< PhotonExportFactory* > factoryList = m_pluginManager->getExportFactories();
    if (factoryList.size() == 0) return 0;

    QVector< QString > exportPMModeNames;
    for (int i = 0; i < factoryList.size(); i++)
        exportPMModeNames << factoryList[i]->name();

    int exportModeFactoryIndex = exportPMModeNames.indexOf(m_pExportModeSettings->modeTypeName);
    if (exportModeFactoryIndex < 0) return 0;

    PhotonExportFactory* pExportModeFactory = factoryList[exportModeFactoryIndex];
    if (!pExportModeFactory) return 0;

    PhotonExport* pExportMode = pExportModeFactory->create();
    if (!pExportMode) return 0;

    pExportMode->SetSaveCoordinatesEnabled(m_pExportModeSettings->exportCoordinates);
    pExportMode->SetSaveCoordinatesInGlobalSystemEnabled(m_pExportModeSettings->exportInGlobalCoordinates);
    pExportMode->SetSavePreviousNextPhotonsID(m_pExportModeSettings->exportPreviousNextPhotonID);
    pExportMode->SetSaveSideEnabled(m_pExportModeSettings->exportIntersectionSurfaceSide);
    pExportMode->SetSaveSurfacesIDEnabled(m_pExportModeSettings->exportSurfaceID);


    if (m_pExportModeSettings->exportSurfaceNodeList.count() > 0)
        pExportMode->SetSaveAllPhotonsEnabled();
    else
        pExportMode->SetSaveSurfacesURLList(m_pExportModeSettings->exportSurfaceNodeList);

    QMap< QString, QString > exportTypeParameters = m_pExportModeSettings->modeTypeParameters;
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
 * Return horizontalSplitter splitter object.
 */
QSplitter* MainWindow::GetHorizontalSplitterPointer()
{
    QSplitter* pSplitter = findChild< QSplitter* >("horizontalSplitter");
    if (!pSplitter) gf::SevereError("MainWindow::GetSceneModelViewPointer: splitter not found");
    return pSplitter;
}

/*!
 * Returns \a true if the application is ready to start with other model. Otherwise,
 * returns \a false.
 */
bool MainWindow::OkToContinue()
{
    if (m_document->IsModified() )
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

    InstanceNode* ancestor = m_sceneModel->NodeFromIndex(nodeIndex);
    SoNode* selectedCoinNode = ancestor->GetNode();

    if (!selectedCoinNode->getTypeId().isDerivedFrom(SoBaseKit::getClassTypeId() ) ) return false;
    if ( (selectedCoinNode->getTypeId().isDerivedFrom(TShapeKit::getClassTypeId() ) ) &&
         (m_coinNode_Buffer->getTypeId().isDerivedFrom(SoBaseKit::getClassTypeId() ) ) ) return false;

    if (ancestor->GetNode() == m_coinNode_Buffer) return false;
    while (ancestor->GetParent() )
    {
        if (ancestor->GetParent()->GetNode() == m_coinNode_Buffer) return false;
        ancestor = ancestor->GetParent();
    }

    CmdPaste* commandPaste = new CmdPaste(type, m_selectionModel->currentIndex(), m_coinNode_Buffer, *m_sceneModel);
    m_commandStack->push(commandPaste);

    UpdateLightSize();
    m_document->SetDocumentModified(true);
    return true;

}

/*!
 * Returns the directory of where the plugins are saved.
 */
QDir MainWindow::PluginDirectory()
{
    // Returns the path to the top level plug-in directory.
    // It is assumed that this is a subdirectory named "plugins" of the directory in
    // which the running version of Tonatiuh is located.

    QDir directory(qApp->applicationDirPath() );
    directory.cd("plugins");
    return directory;
}

/**
 * Restores application settings.
 **/
void MainWindow::ReadSettings()
{
    QSettings settings("CyI", "Tonatiuh");
    QRect rect = settings.value("geometry", QRect(200, 200, 400, 400) ).toRect();
    move(rect.topLeft() );
    resize(rect.size() );


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
bool MainWindow::ReadyForRaytracing(InstanceNode*& rootSeparatorInstance,
                                    InstanceNode*& lightInstance,
                                    SoTransform*& lightTransform,
                                    SunAbstract*& sunShape,
                                    TLightShape*& raycastingShape,
                                    AirAbstract*& transmissivity)
{

    //Check if there is a scene
    TSceneKit* coinScene = m_document->GetSceneKit();
    if (!coinScene) return false;

    //Check if there is a transmissivity defined
    if (!coinScene->getPart("transmissivity", false) ) transmissivity = 0;
    else
        transmissivity = static_cast< AirAbstract* > (coinScene->getPart("transmissivity", false) );

    //Check if there is a rootSeparator InstanceNode
    rootSeparatorInstance = m_sceneModel->NodeFromIndex(sceneModelView->rootIndex() );
    if (!rootSeparatorInstance) return false;

    InstanceNode* sceneInstance = rootSeparatorInstance->GetParent();
    if (!sceneInstance) return false;

    //Check if there is a light and is properly configured
    if (!coinScene->getPart("lightList[0]", false) ) return false;
    TLightKit* lightKit = static_cast< TLightKit* >(coinScene->getPart("lightList[0]", false) );

    lightInstance = sceneInstance->children[0];
    if (!lightInstance) return false;

    if (!lightKit->getPart("tsunshape", false) ) return false;
    sunShape = static_cast< SunAbstract* >(lightKit->getPart("tsunshape", false) );

    if (!lightKit->getPart("icon", false) ) return false;
    raycastingShape = static_cast< TLightShape* >(lightKit->getPart("icon", false) );

    if (!lightKit->getPart("transform",false) ) return false;
    lightTransform = static_cast< SoTransform* >(lightKit->getPart("transform",false) );


    QVector< RandomFactory* > randomDeviateFactoryList = m_pluginManager->getRandomFactories();
    //Check if there is a random generator selected;
    if (m_selectedRandomDeviate == -1)
    {
        if (randomDeviateFactoryList.size() > 0) m_selectedRandomDeviate = 0;
        else return false;
    }

    //Create the random generator
    if (!m_rand) m_rand =  randomDeviateFactoryList[m_selectedRandomDeviate]->create();


    //Create the photon map where photons are going to be stored
    if (!m_increasePhotonMap)
    {
        delete m_pPhotonMap;
        m_pPhotonMap = new PhotonMap();
        m_pPhotonMap->SetBufferSize(m_bufferPhotons);
        m_raysTracedTotal = 0;
    }

    if (!m_pPhotonMap)
    {
        m_pPhotonMap = new PhotonMap();
        m_pPhotonMap->SetBufferSize(m_bufferPhotons);
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
    m_document->SetDocumentModified(false);

    QString shownName = "Untitled";
    if (!m_currentFile.isEmpty() )
    {
        shownName = StrippedName(m_currentFile);
        m_recentFiles.removeAll(m_currentFile);
        m_recentFiles.prepend(m_currentFile);
        UpdateRecentFileActions();
    }

    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Tonatiuh") ) );
}

/*!
 * Defines the settings to save or export and save photon map.
 */
bool MainWindow::SetPhotonMapExportSettings()
{
    QVector< PhotonExportFactory* > exportPhotonMapModeList = m_pluginManager->getExportFactories();
    ExportPhotonsDialog exportSettingsDialog(*m_sceneModel, exportPhotonMapModeList);
    if (!exportSettingsDialog.exec() ) return false;

    if (m_pExportModeSettings) delete m_pExportModeSettings;

    m_pExportModeSettings = new PhotonExportSettings;
    *m_pExportModeSettings = exportSettingsDialog.GetExportPhotonMapSettings();
    return true;
}

/*!
 * Creates actions form recent files.
 */
void MainWindow::SetupActions()
{
    for (int i = 0; i < m_maxRecentFiles; ++i) {
        QAction* a = new QAction(this);
        a->setVisible(false);
        connect(
            a, SIGNAL(triggered()),
            this, SLOT(OpenRecentFile())
        );
        m_recentFileActions << a;
    }
}

void MainWindow::SetupActionsInsertComponent()
{
    QMenu* pComponentMenu = menuInsert->findChild<QMenu*>("menuComponent");
    if (!pComponentMenu) return;
    if (pComponentMenu->isEmpty() )
    {
        //Enable material menu
        pComponentMenu->setEnabled(true);
    }

    if (!m_pluginManager) return;
    QVector<ComponentFactory*> componentFactoryList = m_pluginManager->getComponentFactories();
    if (!(componentFactoryList.size() > 0) ) return;

    for (int i = 0; i < componentFactoryList.size(); ++i)
    {
        ComponentFactory* pComponentFactory = componentFactoryList[i];

        ActionInsertComponent* actionInsertComponent = new ActionInsertComponent(pComponentFactory->name(), this, pComponentFactory);
        actionInsertComponent->setIcon(pComponentFactory->icon() );

        pComponentMenu->addAction(actionInsertComponent);
        //m_materialsToolBar->addAction( actionInsertComponent );
        //m_materialsToolBar->addSeparator();
        connect(actionInsertComponent, SIGNAL(triggered()),
                actionInsertComponent, SLOT(OnActionInsertComponentTriggered()) );
        connect(actionInsertComponent, SIGNAL(CreateComponent(ComponentFactory*)),
                this, SLOT(CreateComponent(ComponentFactory*)) );
    }
}

void MainWindow::SetupActionsInsertMaterial()
{
    QMenu* menu = menuInsert->findChild<QMenu*>("menuMaterial");

    for (MaterialFactory* f : m_pluginManager->getMaterialFactories()) {
        ActionInsertMaterial* a = new ActionInsertMaterial(f, this);
        menu->addAction(a);
        connect(
            a, SIGNAL(CreateMaterial(MaterialFactory*)),
            this, SLOT(CreateMaterial(MaterialFactory*))
        );
    }

    QPushButton* button = new QPushButton;
    button->setIcon(QIcon(":/images/scene/nodeMaterial.png"));
    button->setToolTip("Materials");
    button->setMenu(menu);
    findChild<QToolBar*>("insertToolBar")->addWidget(button);
}

/**
 * Creates an action for the /a pShapeFactory and adds to shape insert menu and toolbar.
 */
void MainWindow::SetupActionsInsertShape()
{
    QMenu* menu = menuInsert->findChild<QMenu*>("menuShape");

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
            ActionInsertShape* a = new ActionInsertShape(f, this);
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

    button = new QPushButton;
    button->setIcon(QIcon(":/images/scene/nodeSurface.png"));
    button->setToolTip("Apertures");
    button->setMenu(new QMenu);
    findChild<QToolBar*>("insertToolBar")->addWidget(button);
}

void MainWindow::SetupActionsInsertTracker()
{
    QMenu* menu = menuInsert->findChild<QMenu*>("menuTracker");

    for (TrackerFactory* f : m_pluginManager->getTrackerFactories())
    {
        ActionInsertTracker* a = new ActionInsertTracker(f, this);
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
    if (actionViewBackground->isChecked() ) m_graphicsRoot->ShowBackground(true);
    else m_graphicsRoot->ShowBackground(false);
}

/*!
 * Creates a view for visualize user done last actions.
 */
void MainWindow::SetupCommandView()
{
    m_commandStack = new QUndoStack(this);
    m_commandView = new QUndoView(m_commandStack);
    m_commandView->setWindowTitle(tr("Command List") );
    m_commandView->setAttribute(Qt::WA_QuitOnClose, false);
    connect(m_commandStack, SIGNAL(canRedoChanged(bool)),
            actionRedo, SLOT(setEnabled(bool)) );
    connect(m_commandStack, SIGNAL(canUndoChanged(bool)),
            actionUndo, SLOT(setEnabled(bool)) );
}

/**
 * Action slot to show/hide a grid with the scene dimensions.
 */
void MainWindow::ShowGrid()
{
    m_graphicsRoot->ShowGrid(actionViewGrid->isChecked());

    /*if( actionViewGrid->isChecked() )
           {
            InstanceNode* sceneInstance = m_sceneModel->NodeFromIndex( sceneModelView->rootIndex() );
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
 * Initializes tonatiuh document object.
 */
void MainWindow::SetupDocument()
{
    m_document = new Document();
    if (m_document)
        connect(m_document, SIGNAL(Warning(QString)), this, SLOT(ShowWarning(QString)) );
}

/*!
 * Defines 3D view background.
 */
void MainWindow::SetupGraphcisRoot()
{
    m_graphicsRoot = new GraphicRoot;

    if (m_graphicsRoot)
    {
        m_graphicsRoot->AddModel(m_document->GetSceneKit() );
        connect(m_graphicsRoot, SIGNAL(ChangeSelection(SoSelection*)),
                this, SLOT(SelectionFinish(SoSelection*)) );

        m_gridXElements = 10;
        m_gridZElements = 10;
        m_gridXSpacing = 10;
        m_gridZSpacing = 10;

//        m_graphicsRoot->AddGrid(CreateGrid(m_gridXElements, m_gridZElements, m_gridXSpacing, m_gridZSpacing) );
        m_graphicsRoot->AddGrid(CreateGrid() );

    }
    else gf::SevereError("MainWindow::SetupDocument: Fail to create new document");
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
    }

    m_focusView = 1;
//    on_action_X_Y_Plane_triggered();
    m_focusView = 2;
//    on_action_Y_Z_Plane_triggered();
    m_focusView = 3;
    on_actionViewTop_triggered();

    // 0 splitterH1 1              tree
    //   splitterV      splitter
    // 2 spliter H2 3              parameters
    on_actionQuadView_toggled();
}

/*!
 * Creates a menu for last used files
 */
void MainWindow::SetupMenus()
{
    for (QAction* a : m_recentFileActions)
        menuRecent->addAction(a);
}

/*!
 * Initializes Tonatiuh models.
 */
void MainWindow::SetupModels()
{
    m_sceneModel = new SceneModel();
    m_sceneModel->SetCoinRoot(*m_graphicsRoot->GetNode() );
    m_sceneModel->SetCoinScene(*m_document->GetSceneKit() );

    m_selectionModel = new QItemSelectionModel(m_sceneModel);

    connect(m_sceneModel, SIGNAL(LightNodeStateChanged(int)),
            this, SLOT(SetSunPositionCalculatorEnabled(int)) );
}

/*!
 * Initializes the tonatiuh objects parameters view.
 */
void MainWindow::SetupParametersView()
{
    connect(
        parametersTabs, SIGNAL(valueModified(SoNode*, QString, QString)),
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
    //m_pPluginManager = new PluginManager;
    //m_pPluginManager->LoadAvailablePlugins( PluginDirectory() );

    if (!m_pluginManager) return; // ! runs twice with 0 first?

    SetupActionsInsertComponent();
//    addToolBarBreak();
    SetupActionsInsertShape();
    SetupActionsInsertMaterial();
    SetupActionsInsertTracker();
}

/*!
 * Initializates Tonatiuh objectes tree view.
 */
void MainWindow::SetupTreeView()
{
    sceneModelView->setModel(m_sceneModel);
    sceneModelView->setSelectionModel(m_selectionModel);
    sceneModelView->setRootIndex(m_sceneModel->IndexFromNodeUrl("//SunNode"));

//    sceneModelView->header()->setStretchLastSection(false);
//    sceneModelView->header()->setSectionResizeMode(0, QHeaderView::Stretch);
//    sceneModelView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
//    sceneModelView->header()->resizeSection(1, 80);

    connect(sceneModelView, SIGNAL(dragAndDrop(const QModelIndex&,const QModelIndex&)),
            this, SLOT(ItemDragAndDrop(const QModelIndex&,const QModelIndex&)) );
    connect(sceneModelView, SIGNAL(dragAndDropCopy(const QModelIndex&,const QModelIndex&)),
            this, SLOT(ItemDragAndDropCopy(const QModelIndex&,const QModelIndex&)) );
    connect(sceneModelView, SIGNAL(showMenu(const QModelIndex&)),
            this, SLOT(ShowMenu(const QModelIndex&)) );
    connect(sceneModelView, SIGNAL(nodeNameModificated(const QModelIndex&,const QString&)),
            this, SLOT(ChangeNodeName(const QModelIndex&,const QString&)) );
}

/*!
 * Defines slots function for main window signals.
 */
void MainWindow::SetupTriggers()
{
    // file
    connect(actionNew, SIGNAL(triggered()), this, SLOT(New()) );
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(Open()) );
    connect(actionSave, SIGNAL(triggered()), this, SLOT(Save()) );
    connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(SaveAs()) );
    connect(actionSaveComponent, SIGNAL(triggered()), this, SLOT(SaveComponent()) );
    connect(actionClose, SIGNAL(triggered()), this, SLOT(close()) );

    // edit
    connect(actionUndo, SIGNAL(triggered()), this, SLOT(Undo()) );
    connect(actionRedo, SIGNAL(triggered()), this, SLOT(Redo()) );
    connect(actionUndoView, SIGNAL(triggered()), this, SLOT(ShowCommandView()) );
    connect(actionCut, SIGNAL(triggered()), this, SLOT(Cut()) );
    connect(actionCopy, SIGNAL(triggered()), this, SLOT(Copy()) );
    connect(actionPasteCopy, SIGNAL(triggered()), this, SLOT(PasteCopy()) );
    connect(actionPasteLink, SIGNAL(triggered()), this, SLOT(PasteLink()) );
    connect(actionDelete, SIGNAL(triggered()), this, SLOT(Delete()) );

    // insert
    connect(actionNode, SIGNAL(triggered()), this, SLOT(CreateGroupNode()) );
    connect(actionSurfaceNode, SIGNAL(triggered()), this, SLOT(CreateSurfaceNode()) );
    connect(actionUserComponent, SIGNAL(triggered()), this, SLOT(InsertUserDefinedComponent()) );

    // scene
    connect(actionDefineSunLight, SIGNAL(triggered()), this, SLOT(DefineSunLight()) );
    connect(actionCalculateSunPosition, SIGNAL(triggered()), this, SLOT(CalculateSunPosition()) );
    connect(actionDisconnect_All_Trackers, SIGNAL(toggled(bool)), this, SLOT(DisconnectAllTrackers(bool)) );
    connect(actionDefineTransmissivity, SIGNAL(triggered()), this, SLOT(DefineTransmissivity()) );

    // run
    connect(actionViewRays, SIGNAL(toggled(bool)), this, SLOT(DisplayRays(bool)) );
    connect(actionRun, SIGNAL(triggered()), this, SLOT(RunCompleteRayTracer()) );
    connect(actionRunFluxAnalysis, SIGNAL(triggered()), this, SLOT(RunFluxAnalysisRayTracer()) );
    connect(actionRayTraceOptions, SIGNAL(triggered()), this, SLOT(ShowRayTracerOptionsDialog())  );

    // view
    connect(actionViewGrid, SIGNAL(triggered()), this, SLOT(ShowGrid())  );
    connect(actionGridSettings, SIGNAL(triggered()), this, SLOT(ChangeGridSettings())  );
    connect(actionViewBackground, SIGNAL(triggered()), this, SLOT(ShowBackground())  );
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
 * Shows the rays and photons stored at the photon map in the 3D view.
 */
void MainWindow::ShowRaysIn3DView()
{
    actionViewRays->setEnabled(false);
    actionViewRays->setChecked(false);

    if (m_drawRays || m_drawPhotons)
    {
        SoSeparator* rays = new SoSeparator;
        rays->setName("Rays");

        if (m_drawPhotons)
        {
            SoSeparator* points = trf::DrawPhotons(*m_pPhotonMap);
            rays->addChild(points);
        }

        if (m_drawRays)
        {
            SoSeparator* currentRays = trf::DrawRays(*m_pPhotonMap, m_raysTracedTotal);
            if (currentRays) rays->addChild(currentRays);
        }
        m_graphicsRoot->AddRays(rays);

        actionViewRays->setEnabled(true);
        actionViewRays->setChecked(true);
    }
}

/*!
 * Returns to the start origin state and starts with a new model defined in \a fileName.
 * If the file name is not defined, it starts with an empty scene.
 */
bool MainWindow::StartOver(const QString& fileName)
{
    InstanceNode* sceneInstance = m_sceneModel->NodeFromIndex(sceneModelView->rootIndex() );

    InstanceNode* concentratorRoot = sceneInstance->children[ sceneInstance->children.size() - 1 ];
    m_selectionModel->setCurrentIndex(m_sceneModel->IndexFromNodeUrl(concentratorRoot->GetNodeURL() ), QItemSelectionModel::ClearAndSelect);

    actionViewRays->setEnabled(false);
    actionViewRays->setChecked(false);

    m_graphicsRoot->RemoveRays();
    m_graphicsRoot->RemoveModel();

    m_commandStack->clear();
    m_sceneModel->Clear();

//    SetSunPositionCalculatorEnabled(0);

    QStatusBar* statusbar = new QStatusBar();
    setStatusBar(statusbar);
    //QStatusBar* statusbar = statusBar();
    if (!fileName.isEmpty() && m_document->ReadFile(fileName) )
    {
        statusbar->showMessage(tr("File loaded"), 2000);
        SetCurrentFile(fileName);
    }
    else
    {
        m_document->New();
        statusbar->showMessage(tr("New file"), 2000);

        SetCurrentFile("");
    }

    ChangeModelScene();
    sceneModelView->expandToDepth(1);
    on_actionViewAll_triggered();
    return true;
}

/*!
 * Returns the \a fullFileName file�s name, without path.
 */
QString MainWindow::StrippedName(const QString& fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

/*!
 * Computes the new light size to the scene current dimensions.
 */
void MainWindow::UpdateLightSize()
{
    SoSceneKit* coinScene = m_document->GetSceneKit();

    TLightKit* lightKit = static_cast< TLightKit* >(coinScene->getPart("lightList[0]", false) );
    if (!lightKit) return;

    TSeparatorKit* concentratorRoot = static_cast< TSeparatorKit* >(coinScene->getPart("childList[0]", false) );
    if (!concentratorRoot) return;

    SoGetBoundingBoxAction* action = new SoGetBoundingBoxAction(SbViewportRegion() );
    concentratorRoot->getBoundingBox(action);

    SbBox3f box = action->getXfBoundingBox().project();
    delete action;

    BBox sceneBox;
    if (!box.isEmpty())
    {
        sceneBox.pMin = Point3D(box.getMin()[0], box.getMin()[1], box.getMin()[2]);
        sceneBox.pMax = Point3D(box.getMax()[0], box.getMax()[1], box.getMax()[2]);

        if (lightKit) lightKit->Update(sceneBox);
    }

    m_sceneModel->UpdateSceneModel();
}

/*!
 * Updates the recently opened files actions list.
 */
void MainWindow::UpdateRecentFileActions()
{
    QMutableStringListIterator iterator(m_recentFiles);
    while (iterator.hasNext() ) {
        if (!QFile::exists(iterator.next() ) ) iterator.remove();
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
    SoCamera* camera = m_graphicView[m_focusView]->GetCamera();
    camera->viewAll(m_graphicsRoot->GetNode(), vpr);
}
