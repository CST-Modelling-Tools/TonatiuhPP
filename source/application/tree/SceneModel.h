#pragma once

#include <QAbstractItemModel>

#include "commands/tgc.h"

class InstanceNode;
class QModelIndex;
class SoBaseKit;
class SoNode;
class SoNodeKitPath;
class SoSeparator;
class SunKit;
class TSeparatorKit;
class TSceneKit;
class TShapeKit;
class Document;

class SceneModel: public QAbstractItemModel
{
    Q_OBJECT

public:
    SceneModel(QObject* parent = 0);
    ~SceneModel();

    void clear();

    void setDocument(Document* document);
    TSceneKit* getSceneKit() const {return m_nodeScene;}

    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    int rowCount(const QModelIndex& index) const;
    int columnCount(const QModelIndex& index) const;
    QModelIndex parent(const QModelIndex& index) const;
    QVariant data(const QModelIndex& modelIndex, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    Qt::DropActions supportedDropActions() const;
    Qt::DropActions supportedDragActions() const;

    InstanceNode* getInstance(const QModelIndex& index) const;
    bool SetNodeName(SoNode* node, QString name);
    QModelIndex indexFromUrl(QString url) const;
    QModelIndex indexFromPath(const SoNodeKitPath& path) const;
    SoNodeKitPath* pathFromIndex(const QModelIndex& index) const;

    void insertSunNode(SunKit* lightKit);
    void removeSunNode(SunKit* sunKit);

    int insertCoinNode(SoNode* coinChild, SoBaseKit* coinParent);
    void removeCoinNode(int row, SoBaseKit* parent);
    void replaceCoinNode(TShapeKit* parent, SoNode* node);

    bool Cut(SoBaseKit& parent, int row);
    bool Paste(tgc::PasteType type, SoBaseKit& coinParent, SoNode& coinChild, int row);

    void UpdateSceneModel();

private:
    InstanceNode* addInstanceNode(InstanceNode* parent, SoNode* node);
    void generateInstanceTree(InstanceNode* instance);
    void deleteInstanceTree(InstanceNode* instance);

private:
    SoSeparator* m_nodeRoot;
    TSceneKit* m_nodeScene; // child of root
    InstanceNode* m_instanceScene;
    InstanceNode* m_instanceLayout; // child of scene

    // the same SoNode can be attached to multiple parents
    // InstanceNode have a single parent
    std::map< SoNode*, QList<InstanceNode*> > m_mapCoinQt;
};
