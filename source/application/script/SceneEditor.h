#pragma once

#include <QObject>
class TSeparatorKit;
class SoBaseKit;

class SceneEditor: public QObject
{
    Q_OBJECT
public:
    explicit SceneEditor(QObject* parent = nullptr);
    ~SceneEditor();

    TSeparatorKit* getRoot() const {return m_root;}

public slots:
    void insertNode(QString name = "");
    void insertShape(QString name = "");

    void insertSurface(QString name = "");
    void insertProfile(QString name = "");
    void insertMaterial(QString name = "");
    void insertTracker(QString name = "");

    void select(QString name);
    void setName(QString name);
    void setValue(QString name, QString value);

signals:

private:
    TSeparatorKit* m_root;
    SoBaseKit* m_selection;
};

