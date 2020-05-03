#pragma once

#include <Inventor/SbVec3f.h>

#include "commands/tgc.h"

#include "ui_mainwindow.h"

class Document;
class GraphicRoot;
class GraphicView;
class InstanceNode;
class PhotonExport;
class PluginManager;
class QDir;
class QUndoStack;
class QUndoView;
class RandomAbstract;
class SoDragger;
class SoSelection;
class SoSeparator;
class SoTransform;
class ComponentFactory;
class TLightShape;
class MaterialFactory;
class PhotonMap;
class PhotonToMemory;
class ShapeFactory;
class SunAbstract;
class TrackerFactory;
class AirAbstract;
class SoCamera;

struct PhotonExportSettings;

//!  Main window class.
/*!
  Tonatiuh's main window class. It includes the menu bars and all the actions, signals
  and slots to provide user interaction with the program.
*/

class MainWindow: public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QString tonatiuhFile = 0, QWidget* parent = 0, Qt::WindowFlags flags = 0);
    ~MainWindow();

    void FinishManipulation();
    void StartManipulation(SoDragger* dragger);
    void ExecuteScriptFile(QString tonatiuhScriptFile);
    void SetPluginManager(PluginManager* pluginManager);

signals:
    void Abort(QString error);

public slots:
    void AddExportSurfaceURL(QString nodeURL);
    void ChangeSunPosition(double azimuth, double elevation);
    void ChangeSunPosition(int year, int month, int day, double hours, double minutes, double seconds, double latitude, double longitude);
    void Clear();
    void Copy();
    void Copy(QString nodeURL);
    void CreateGroupNode();
    void CreateComponentNode(QString componentType, QString nodeName, int numberofParameters, QVector< QVariant > parametersList);
    void CreateMaterial(QString materialType);
    void CreateShape(QString name);
    void CreateShape(QString shapeType, int numberOfParameters, QVector< QVariant > parametersList);
    void CreateSurfaceNode();
    void CreateTracker(QString trackerType);
    void Cut();
    void Cut(QString nodeURL);
    void Delete();
    void Delete(QString nodeURL);
    void InsertFileComponent(QString componentFileName = "");
    void New();
    void Open(QString fileName);
    void Paste(QString nodeURL, QString pasteType = "Shared");
    void PasteCopy();
    void PasteLink();
    void Run();
    void RunFluxAnalysis(QString nodeURL, QString surfaceSide, unsigned int nOfRays, int heightDivisions, int widthDivisions, QString directory, QString fileName, bool saveCoords);
    bool Save();
    void SaveComponent(QString componentFileName);
    void SaveAs(QString fileName);
    void SelectNode(QString nodeUrl);
    void SetExportAllPhotonMap();
    void SetExportCoordinates(bool enabled, bool global);
    void SetExportIntersectionSurface(bool enabled);
    void SetExportIntersectionSurfaceSide(bool enabled);
    void SetExportPhotonMapType(QString exportModeType);
    void SetExportPreviousNextPhotonID(bool enabled);
    void SetExportTypeParameterValue(QString parameterName, QString parameterValue);
    void SetIncreasePhotonMap(bool increase);
    void SetNodeName(QString nodeName);
    void SetPhotonMapBufferSize(unsigned int nPhotons);
    void SetRandomDeviateType(QString typeName);
    void SetRayCastingGrid(int widthDivisions, int heightDivisions);
    void SetRaysDrawingOptions(bool drawRays, bool drawPhotons);
    void SetRaysPerIteration(unsigned int rays);
    void SetSunshape(QString sunshapeType);
    void SetSunshapeParameter(QString parameter, QString value);
    void SetTransmissivity(QString transmissivityType);
    void SetTransmissivityParameter(QString parameter, QString value);
    void SetValue(QString nodeUrl, QString parameter, QString value);

    protected:
    void closeEvent(QCloseEvent* event);
    void mousePressEvent (QMouseEvent* e);

    private slots:
    void CalculateSunPosition();
    void ChangeGridSettings();
    void ChangeNodeName(const QModelIndex& index, const QString& newName);
    void ChangeSelection(const QModelIndex& current);
    void CreateComponent(ComponentFactory* pComponentFactory);
    void CreateMaterial(MaterialFactory* pMaterialFactory);
    void CreateShape(ShapeFactory* pShapeFactory);
    void CreateShape(ShapeFactory* pShapeFactory, int numberOfParameters, QVector<QVariant> parametersList);
    void CreateTracker(TrackerFactory* pTrackerFactory);
    void DefineSunLight();
    void DefineTransmissivity();
    void DisconnectAllTrackers(bool disconnect);
    void DisplayRays(bool display);
    void InsertUserDefinedComponent();
    void ItemDragAndDrop(const QModelIndex& newParent, const QModelIndex& node);
    void ItemDragAndDropCopy(const QModelIndex& newParent, const QModelIndex& node);
    void Open();
    void OpenRecentFile();
    void Redo();
    void RunCompleteRayTracer();
    void RunFluxAnalysisRayTracer();
    bool SaveAs();
    bool SaveComponent();
    void SelectionFinish(SoSelection* selection);
    void SetParameterValue(SoNode* node, QString paramenterName, QString newValue);
    void SetSunPositionCalculatorEnabled(int enabled);
    void ShowBackground();
    void ShowCommandView();
    void ShowGrid();
    void ShowMenu(const QModelIndex& index);
    void ShowRayTracerOptionsDialog();
    void ShowWarning(QString message);
    void Undo();

    SbVec3f getTargetOfCamera(SoCamera* cam);

    //Manipulators actions
    void SoTransform_to_SoCenterballManip();
    void SoTransform_to_SoJackManip();
    void SoTransform_to_SoHandleBoxManip();
    void SoTransform_to_SoTabBoxManip();
    void SoTransform_to_SoTrackballManip();
    void SoTransform_to_SoTransformBoxManip();
    void SoTransform_to_SoTransformerManip();
    void SoManip_to_SoTransform();

    //View menu actions
    void on_actionAxis_toggled();
    void on_actionQuadView_toggled();
    void on_actionSunPlane_triggered();
    void on_action_X_Y_Plane_triggered();
    void on_action_X_Z_Plane_triggered();
    void on_action_Y_Z_Plane_triggered();

    //Automation menu actions
    void on_actionOpenScriptEditor_triggered();

    //Help menu actions
    void on_actionAbout_triggered();
    //void on_actionCheckForUpdates_triggered();


private:
    void ChangeModelScene();
    SoSeparator* CreateGrid( int xDimension, int zDimension, double xSpacing, double zSpacing );
    PhotonExport* CreatePhotonMapExport() const;
    QToolBar* CreateTrackerTooBar( QMenu* pMaterialsMenu );
    bool Delete( QModelIndex index );
    QSplitter* GetHorizontalSplitterPointer();
    bool OkToContinue();
    bool Paste( QModelIndex nodeIndex, tgc::PasteType type );
    QDir PluginDirectory();

    void ReadSettings();
    bool ReadyForRaytracing(InstanceNode*& rootSeparatorInstance,
                            InstanceNode*& lightInstance,
                            SoTransform*& lightTransform,
                            SunAbstract*& sunShape,
                            TLightShape*& shape,
                            AirAbstract*& transmissivity);
    bool SaveFile(const QString& fileName);
    void SetCurrentFile(const QString& fileName);
    bool SetPhotonMapExportSettings();
    void SetupActions();
    void SetupActionsInsertComponent();
    void SetupActionsInsertMaterial();
    void SetupActionsInsertShape();
    void SetupActionsInsertTracker();
    void SetupCommandView();
    void SetupDocument();
    void SetupGraphcisRoot();
    void SetupGraphicView();
    void SetupMenus();
    void SetupModels();
    void SetupParametersView();
    void SetupPluginsManager();
    void SetupTreeView();
    void SetupTriggers();
    void SetupViews();
    void ShowRaysIn3DView();
    bool StartOver(const QString& fileName);
    QString StrippedName(const QString& fullFileName);
    void UpdateLightSize();
    void UpdateRecentFileActions();
    void WriteSettings();
    double GetwPhoton();

    enum {m_maxRecentFiles = 7};
    QUndoStack* m_commandStack;
    QUndoView* m_commandView;
    QString m_currentFile;
    Document* m_document;
    QStringList m_recentFiles;
    QVector<QAction*> m_recentFileActions;//[m_maxRecentFiles];

    PluginManager* m_pluginManager;

    SceneModel* m_sceneModel;
    QItemSelectionModel* m_selectionModel;

    RandomAbstract* m_rand;
    int m_selectedRandomDeviate;


    unsigned long m_bufferPhotons;
    bool m_increasePhotonMap;
    PhotonExportSettings* m_pExportModeSettings;
    PhotonMap* m_pPhotonMap;

    QString m_lastExportFileName;
    QString m_lastExportSurfaceUrl;
    bool m_lastExportInGlobal;

    GraphicRoot* m_graphicsRoot;
    SoNode* m_coinNode_Buffer;
    QStringList* m_manipulators_Buffer;

    unsigned long m_raysTracedTotal;
    unsigned long m_raysTraced;
    int m_heightDivisions;
    int m_widthDivisions;

    bool m_drawPhotons;
    bool m_drawRays;

    int m_gridXElements;
    int m_gridZElements;
    double m_gridXSpacing;
    double m_gridZSpacing;
    QVector<GraphicView*> m_graphicView;
    int m_focusView;
    bool m_saveCoordinates;
    bool m_saveSide;
};
