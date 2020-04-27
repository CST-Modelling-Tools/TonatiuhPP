#pragma once

#include <QMap>
#include <QObject>
#include <QPair>
#include <QString>
#include <QVector>

class Document;
class GraphicRoot;
class InstanceNode;
class RandomDeviate;
class RandomDeviateFactory;
class QScriptContext;
class SceneModel;
class PhotonMap;
class Transform;

class ScriptRayTracer: public QObject
{
    Q_OBJECT

public:
    ScriptRayTracer(QVector< RandomDeviateFactory* > listRandomDeviateFactory);
    ~ScriptRayTracer();

    void Clear();

    QString GetDir();

    bool IsValidRandomGeneratorType(QString type);
    bool IsValidSurface(QString surfaceName);

    double GetArea();
    double GetNumrays();

    int SetDir(QString dir);

    int SetIrradiance(double irradiance);

    int SetNumberOfRays(double nrays);

    int SetNumberOfWidthDivisions(int wdivisions);
    int SetNumberOfHeightDivisions(int hdivisions);

    int SetPhotonMapExportMode(QString typeName);

    int SetRandomDeviateType(QString typeName);

    void SetSunAzimtuh(double azimuth);
    void SetSunElevation(double elevation);

    void SetupGraphcisRoot();
    void SetupModels();
    int SetTonatiuhModelFile (QString filename);

    int Trace();

    int SetSunPositionToScene();
    int SetDisconnectAllTrackers(bool disconnect);
    int Save(const QString& fileName);

private:
    Document* m_document;

    double m_irradiance;

    unsigned long m_numberOfRays;

    PhotonMap* m_photonMap;
    bool m_photonMapToFile;

    QVector< RandomDeviateFactory* > m_RandomDeviateFactoryList;
    RandomDeviate* m_randomDeviate;

    SceneModel* m_sceneModel;

    double m_area;
    int m_widthDivisions;
    int m_heightDivisions;
    bool m_sunPosistionChanged;
    double m_sunAzimuth;
    double m_sunElevation;

    double m_wPhoton;

    QString m_dirName;
};
