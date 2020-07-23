#pragma once

#include <QObject>

class QString;
class SoSelection;
class SoSeparator;
class TSceneKit;
class TShapeKit;


class Document: public QObject
{
    Q_OBJECT

public:
    Document();
    ~Document() {}

    void New();
    bool ReadFile(const QString& fileName);
    bool WriteFile(const QString& fileName);

    bool isModified() {return m_isModified;}
    void setModified(bool status) {m_isModified = status;}

    TSceneKit* getSceneKit() const {return m_scene;}

signals:
    void Warning(QString message);

private:
    void ClearScene();

    TSceneKit* m_scene;
    bool m_isModified;
};

