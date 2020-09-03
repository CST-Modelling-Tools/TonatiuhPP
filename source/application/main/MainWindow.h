#pragma once

#include <QMainWindow>
#include <QVariant>
#include <QScriptValue>

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
class PhotonsBuffer;
class PhotonToMemory;
class ShapeFactory;
class SunShape;
class TrackerFactory;
class AirTransmission;
class SoCamera;
class SoGroup;
class SceneTreeModel;
struct PhotonsSettings;
class ProfileFactory;
class UndoView;
class NodeObject;
class TFactory;
class SoBaseKit;
class TNode;

namespace Ui {
class MainWindow;
}


class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QString tonatiuhFile = "", QSplashScreen* splash = 0, QWidget* parent = 0, Qt::WindowFlags flags = 0);
    ~MainWindow();

    PluginManager* getPlugins() {return m_pluginManager;}
    void FinishManipulation();
    void StartManipulation(SoDragger* dragger);
//    void ExecuteScriptFile(QString fileName); // del?

public slots:
    void FileNew();
    void FileOpen(QString fileName);
    bool FileSave();
    void FileSaveAs(QString fileName);
    void SaveComponent(QString fileName);
    void Clear();

    void Copy();
    void Copy(QString url);
    void Cut();
    void Cut(QString url);
    void Delete();
    void Delete(QString url);
    void Paste(QString url, QString pasteType = "Shared");
    void PasteCopy();
    void PasteLink();

    void InsertNode();
    void InsertShape();
    void InsertTracker();
    void InsertShapeSurface(QString shapeType, int numberOfParameters, QVector<QVariant> parametersList);
    void CreateComponentNode(QString componentType, QString nodeName, int numberofParameters, QVector<QVariant> parametersList);
    void InsertFileComponent(QString componentFileName = "");
    void InsertScene(QScriptValue v);
    QScriptValue FindInterception(QScriptValue surface, QScriptValue rays);

    void Select(QString url);
    void SetName(QString name);
    void SetValue(QString url, QString parameter, QString value);

    void ChangeSunPosition(double azimuth, double elevation);
    void ChangeSunPosition(int year, int month, int day, double hours, double minutes, double seconds, double latitude, double longitude);
    void SetSunshape(QString name);
    void SetSunParameter(QString parameter, QString value);
    void SetAir(QString name);
    void SetAirParameter(QString parameter, QString value);

    void Run();
    void RunFluxAnalysis(QString nodeURL, QString surfaceSide, uint nOfRays, int heightDivisions, int widthDivisions, QString directory, QString file, bool saveCoords);
    void SetExportAllPhotonMap();
    void SetExportCoordinates(bool enabled, bool global);
    void SetExportIntersectionSurface(bool enabled);
    void SetExportIntersectionSurfaceSide(bool enabled);
    void SetExportPhotonMapType(QString name);
    void SetExportPreviousNextPhotonID(bool enabled);
    void SetExportTypeParameterValue(QString parameterName, QString value);
    void AddExportSurfaceURL(QString nodeURL);
    void SetRaysNumber(uint rays) {m_raysNumber = rays;}
    void SetRaysScreen(uint rays) {m_raysScreen = rays;}
    void SetRaysRandomFactory(QString name);
    void SetRaysGrid(int width, int height);
    void SetPhotonBufferSize(uint size) {m_photonBufferSize = size;}
    void SetPhotonBufferAppend(bool on) {m_photonBufferAppend = on;}

private slots:
    void onAbort(QString error);
    void onUndoStack();
    void FileOpen();
    void FileOpenRecent();
    bool FileSaveAs();

    void ChangeNodeName(const QModelIndex& index, const QString& name);
    void ChangeSelection(const QModelIndex& index);
    void CreateComponent(ComponentFactory* factory);

//    void InsertShapeSurface(TFactory* f);
    void InsertShapeSurface(ShapeFactory* factory, int numberOfParameters, QVector<QVariant> parametersList);

    void onSunDialog();
    void onAirDialog();

    void InsertUserDefinedComponent();
    void ItemDragAndDrop(const QModelIndex& newParent, const QModelIndex& node);
    void ItemDragAndDropCopy(const QModelIndex& newParent, const QModelIndex& node);

    void undo();
    void redo();
    void showUndoHistory();

    void RunCompleteRayTracer();
    void RunFluxAnalysisDialog();

    bool SaveComponent();
    void SelectionFinish(SoSelection* selection);
    void setFieldText(SoNode* node, QString field, QString value);
    void setFieldNode(SoNode* node, QString field, TNode* value);

    void showGrid();
    void showRays(bool on);
    void showPhotons(bool on);

    void ShowMenu(const QModelIndex& index);
    void ShowWarning(QString message);


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

    // automation menu actions
    void on_actionRunScript_triggered();

    // help menu actions
    void on_actionAbout_triggered();
    //void on_actionCheckForUpdates_triggered();

signals:
    void Abort(QString error);

protected:
    void closeEvent(QCloseEvent* event);
    void mousePressEvent(QMouseEvent* e);

private:
    void ChangeModelScene();
    PhotonsAbstract* CreatePhotonMapExport() const;
    bool Delete(QModelIndex index);
    bool OkToContinue();
    bool Paste(QModelIndex index, tgc::PasteType type);

    bool ReadyForRaytracing(InstanceNode*& instanceLayout,
                            InstanceNode*& instanceSun,
                            AirTransmission*& air);
    bool SaveFile(const QString& fileName);
    void SetCurrentFile(const QString& fileName);

    void SetupDocument();

    void SetupViews();
    void SetupGraphicView();
    void SetupTreeView();

    void SetupPluginManager();
    void SetupActionsInsertComponent();

    void SetupTriggers();

    void ReadSettings();
    void SetupRecentFiles();
    void UpdateRecentFileActions();
    void WriteSettings();

    bool StartOver(const QString& fileName);
    void setDocumentModified(bool value);

    void ShowRaysIn3DView();
    void UpdateLightSize();

private:
    Ui::MainWindow* ui;
    PluginManager* m_pluginManager;

    static const int m_maxRecentFiles = 7;
    QStringList m_recentFiles;
    QString m_currentFile;

    QUndoStack* m_undoStack;
    UndoView* m_undoView;

    Document* m_document;
    GraphicRoot* m_graphicsRoot;
    SceneTreeModel* m_modelScene;
    QItemSelectionModel* m_modelSelection;

    QString m_lastExportFileName;
    QString m_lastExportSurfaceUrl;
    bool m_lastExportInGlobal;

    SoNode* m_coinNode_Buffer;
    QStringList* m_manipulators_Buffer;

    ulong m_raysNumber;
    ulong m_raysScreen;
    int m_raysRandomFactoryIndex;
    int m_raysGridWidth;
    int m_raysGridHeight;

    ulong m_raysTracedTotal;
    Random* m_rand;

    PhotonsBuffer* m_photonsBuffer;
    ulong m_photonBufferSize;
    bool m_photonBufferAppend;
    PhotonsSettings* m_photonsSettings;

    QVector<GraphicView*> m_graphicView;
    int m_focusView;
    bool m_saveCoordinates;
    bool m_saveSide;
};
