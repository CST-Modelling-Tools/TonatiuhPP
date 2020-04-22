#pragma once

#include <QObject>

class QString;
class SoSelection;
class SoSeparator;
class TSceneKit;
class TShapeKit;

//!  Document class stores the application scene.
/*!
 * This class is not yet documented.
 */

class Document: public QObject
{
    Q_OBJECT

public:
    Document();
    ~Document() {}
    void SetDocumentModified(bool status);
    void New();
    bool ReadFile(const QString& fileName);
    bool WriteFile(const QString& fileName);

    bool IsModified();
    TSceneKit* GetSceneKit() const;

signals:
    void Warning(QString message);

private:
    TSceneKit* GetSceneKitFromFile(const QString& fileName);
    void InitializeScene();
    void ClearScene();

    TSceneKit* m_scene;
    bool m_isModified;
};

