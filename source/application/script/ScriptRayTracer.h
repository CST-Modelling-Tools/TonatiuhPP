#pragma once

#include <QMap>
#include <QObject>
#include <QPair>
#include <QString>
#include <QVector>

class Document;
class GraphicRoot;
class InstanceNode;
class Random;
class RandomFactory;
class QScriptContext;
class SceneModel;
class PhotonsBuffer;
class Transform;


class ScriptRayTracer: public QObject
{
    Q_OBJECT

public:
    ScriptRayTracer(QVector<RandomFactory*> listRandomFactory);
    ~ScriptRayTracer();

    void Clear();

    QString GetDir();

    bool IsValidRandomGeneratorType(QString type);
    bool IsValidSurface(QString surfaceName);

    double GetArea() {
        return m_area;
    }
    double GetNumrays() {
        return m_numberOfRays;
    }

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
    int Save(const QString& fileName);

private:
    Document* m_document;

    double m_irradiance;

    ulong m_numberOfRays;

    PhotonsBuffer* m_photonMap;
    bool m_photonMapToFile;

    QVector<RandomFactory*> m_RandomFactoryList;
    Random* m_randomDeviate;

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
