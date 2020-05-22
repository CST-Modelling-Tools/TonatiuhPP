#pragma once

#include <QMainWindow>

#include <QVariant>

class QItemSelectionModel;
class QSplitter;
class QSplashScreen;

#include <Inventor/SbVec3f.h>

#include "commands/tgc.h"


class Document;
class GraphicRoot;
class GraphicView;
class InstanceNode;
class PhotonsAbstract;
class PluginManager;
class QDir;
class QUndoStack;
class QUndoView;
class Random;
class SoDragger;
class SoSelection;
class SoSeparator;
class SoTransform;
class ComponentFactory;
class SunAperture;
class MaterialFactory;
class Photons;
class PhotonToMemory;
class ShapeFactory;
class SunShape;
class TrackerFactory;
class Air;
class SoCamera;

class SceneModel;
struct PhotonsSettings;

//!  Main window class.
/*!
  Tonatiuh's main window class. It includes the menu bars and all the actions, signals
  and slots to provide user interaction with the program.
*/

namespace Ui {
class MainWindow;
}


class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QString tonatiuhFile = 0, QSplashScreen* splash = 0, QWidget* parent = 0, Qt::WindowFlags flags = 0);
    ~MainWindow();

    void FinishManipulation();
    void StartManipulation(SoDragger* dragger);
    void ExecuteScriptFile(QString tonatiuhScriptFile);

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
    void CreateMaterial(QString name);
    void CreateShape(QString name);
    void CreateShape(QString shapeType, int numberOfParameters, QVector< QVariant > parametersList);
    void CreateSurfaceNode();
    void CreateTracker(QString name);
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
    void SetExportPhotonMapType(QString name);
    void SetExportPreviousNextPhotonID(bool enabled);
    void SetExportTypeParameterValue(QString parameterName, QString parameterValue);
    void SetIncreasePhotonMap(bool increase);
    void SetNodeName(QString name);
    void SetPhotonMapBufferSize(uint nPhotons);
    void SetRandomDeviateType(QString name);
    void SetRayCastingGrid(int widthDivisions, int heightDivisions);
    void SetRaysDrawingOptions(bool drawRays, bool drawPhotons);
    void SetRaysPerIteration(unsigned int rays);
    void SetSunshape(QString name);
    void SetSunshapeParameter(QString parameter, QString value);
    void SetAir(QString name);
    void SetAirParameter(QString parameter, QString value);
    void SetValue(QString nodeUrl, QString parameter, QString value);

protected:
    void closeEvent(QCloseEvent* event);
    void mousePressEvent(QMouseEvent* e);

private slots:
    void CalculateSunPosition();
    void ChangeGridSettings();
    void ChangeNodeName(const QModelIndex& index, const QString& name);
    void ChangeSelection(const QModelIndex& current);
    void CreateComponent(ComponentFactory* factory);
    void CreateMaterial(MaterialFactory* factory);
    void CreateShape(ShapeFactory* factory);
    void CreateShape(ShapeFactory* factory, int numberOfParameters, QVector<QVariant> parametersList);
    void CreateTracker(TrackerFactory* factory);
    void onSunDialog();
    void onAirDialog();
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
    void SetParameterValue(SoNode* node, QString paramenterName, QString value);
    void SetSunPositionCalculatorEnabled(int enabled);
    void ShowBackground();
    void ShowCommandView();
    void ShowGrid();
    void ShowMenu(const QModelIndex& index);
    void onRayOptionsDialog();
    void ShowWarning(QString message);
    void Undo();

    SbVec3f getTarget(SoCamera* camera);

    // manipulators actions
    void SoTransform_to_SoCenterballManip();
    void SoTransform_to_SoJackManip();
    void SoTransform_to_SoHandleBoxManip();
    void SoTransform_to_SoTabBoxManip();
    void SoTransform_to_SoTrackballManip();
    void SoTransform_to_SoTransformBoxManip();
    void SoTransform_to_SoTransformerManip();
    void SoManip_to_SoTransform();

    // view actions
    void on_actionViewAxes_toggled();

    void on_actionViewAll_triggered();
    void on_actionViewSelected_triggered();

    void on_actionViewTop_triggered();
    void on_actionLookNorth_triggered();
    void on_actionLookEast_triggered();
    void on_actionLookSouth_triggered();
    void on_actionLookWest_triggered();
    void on_actionViewSun_triggered();

    void on_actionQuadView_toggled();


    //Automation menu actions
    void on_actionOpenScriptEditor_triggered();

    //Help menu actions
    void on_actionAbout_triggered();
    //void on_actionCheckForUpdates_triggered();

private:
    void ChangeModelScene();
    SoSeparator* CreateGrid();
    PhotonsAbstract* CreatePhotonMapExport() const;
    bool Delete(QModelIndex index);
    bool OkToContinue();
    bool Paste(QModelIndex nodeIndex, tgc::PasteType type);

    bool ReadyForRaytracing(InstanceNode*& instanceLayout,
                            InstanceNode*& instanceSun,
                            SunShape*& sunShape,
                            SunAperture*& shape,
                            Air*& air);
    bool SaveFile(const QString& fileName);
    void SetCurrentFile(const QString& fileName);
    bool SetPhotonMapExportSettings();

    void SetupActions();
    void SetupDocument();
    void SetupGraphicsRoot();
    void SetupModels();

    void SetupViews();
    void SetupCommandView();
    void SetupGraphicView();
    void SetupTreeView();
    void SetupParametersView();

    void SetupPluginsManager();
    void SetupActionsInsertComponent();
    void SetupActionsInsertMaterial();
    void SetupActionsInsertShape();
    void SetupActionsInsertTracker();

    void SetupTriggers();

    void ReadSettings();
    void UpdateRecentFileActions();
    void WriteSettings();

    bool StartOver(const QString& fileName);
    void setModified(bool value);

    void ShowRaysIn3DView();
    void UpdateLightSize();

private:
    Ui::MainWindow* ui;
    PluginManager* m_pluginManager;

    static const int m_maxRecentFiles = 7;
    QStringList m_recentFiles;
    QVector<QAction*> m_recentFileActions;

    QUndoStack* m_commandStack;
    QUndoView* m_commandView;

    QString m_currentFile;
    Document* m_document;

    SceneModel* m_sceneModel;
    QItemSelectionModel* m_selectionModel;

    Random* m_rand;
    int m_selectedRandomDeviate;

    ulong m_bufferPhotons;
    bool m_increasePhotonMap;
    PhotonsSettings* m_photonsSettings;
    Photons* m_photons;

    QString m_lastExportFileName;
    QString m_lastExportSurfaceUrl;
    bool m_lastExportInGlobal;

    GraphicRoot* m_graphicsRoot;
    SoNode* m_coinNode_Buffer;
    QStringList* m_manipulators_Buffer;

    ulong m_raysTracedTotal;
    ulong m_raysTraced;
    int m_heightDivisions;
    int m_widthDivisions;

    bool m_drawPhotons;
    bool m_drawRays;

    QVector<GraphicView*> m_graphicView;
    int m_focusView;
    bool m_saveCoordinates;
    bool m_saveSide;
};
