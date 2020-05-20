#pragma once

#include <QAbstractItemModel>

#include "commands/tgc.h"

class InstanceNode;
class QModelIndex;
class SoBaseKit;
class SoNode;
class SoNodeKitPath;
class SoSeparator;
class TLightKit;
class TSeparatorKit;
class TSceneKit;


class SceneModel: public QAbstractItemModel
{
    Q_OBJECT

public:
    SceneModel(QObject* parent = 0);
    ~SceneModel();

    void Clear();

    void setSceneRoot(SoSeparator& coinRoot);
    void setSceneKit(TSceneKit& coinScene);
    TSceneKit* getSceneKit() const {return m_coinScene;}

    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    int rowCount(const QModelIndex& index) const;
    int columnCount(const QModelIndex&index) const;
    QModelIndex parent(const QModelIndex& index) const;
    QVariant data(const QModelIndex& modelIndex, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    Qt::DropActions supportedDropActions() const;
    Qt::DropActions supportedDragActions() const;

    InstanceNode* getInstance(const QModelIndex& index) const;
    bool SetNodeName(SoNode* node, QString name);
    QModelIndex IndexFromNodeUrl(QString url) const;
    QModelIndex IndexFromPath(const SoNodeKitPath& coinNodePath) const;
    SoNodeKitPath* PathFromIndex(const QModelIndex& modelIndex) const;

    void InsertLightNode(TLightKit& lightKit);
    void RemoveLightNode(TLightKit& lightKit);

    int InsertCoinNode(SoNode& coinChild, SoBaseKit& coinParent);
    void RemoveCoinNode(int row, SoBaseKit& coinParent);

    bool Cut(SoBaseKit& coinParent, int row);
    bool Paste(tgc::PasteType type, SoBaseKit& coinParent, SoNode& coinChild, int row);

    void UpdateSceneModel();

private:
    void SetRoot();
    InstanceNode* AddInstanceNode(InstanceNode& parent, SoNode* node);
    void GenerateInstanceTree(InstanceNode& instance);
    void DeleteInstanceTree(InstanceNode& instance);

private:
    SoSeparator* m_coinRoot;
    TSceneKit* m_coinScene;
    InstanceNode* m_instanceRoot;
    InstanceNode* m_instanceLayout;

    // the same SoNode can be attached to multiple parents
    // InstanceNode have a single parent
    std::map< SoNode*, QList<InstanceNode*> > m_mapCoinQt;
};
