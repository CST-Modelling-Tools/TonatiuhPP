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

    void SetCoinRoot(SoSeparator& coinRoot);
    void SetCoinScene(TSceneKit& coinScene);


    QModelIndex index(int row, int column, const QModelIndex& parentModelIndex = QModelIndex()) const;
    int rowCount(const QModelIndex& parentModelIndex) const;
    int columnCount (const QModelIndex&) const;
    QModelIndex parent (const QModelIndex& childModelIndex) const;
    QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex& modelIndex, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    Qt::DropActions supportedDropActions() const;
    Qt::DropActions supportedDragActions() const;

    bool Cut(SoBaseKit& coinParent, int row);


    QModelIndex IndexFromNodeUrl(QString nodeUrl) const;
    QModelIndex IndexFromPath(const SoNodeKitPath& coinNodePath) const;

    int InsertCoinNode(SoNode& coinChild, SoBaseKit& coinParent);
    void InsertLightNode(TLightKit& lightKit);

    InstanceNode* NodeFromIndex(const QModelIndex& modelIndex) const;

    SoNodeKitPath* PathFromIndex(const QModelIndex& modelIndex) const;

    bool Paste(tgc::PasteType type, SoBaseKit& coinParent, SoNode& coinChild, int row);

    void RemoveCoinNode(int row, SoBaseKit& coinParent);
    void RemoveLightNode(TLightKit& coinLight);

    bool SetNodeName(SoNode* coinChild, QString newName);

    void UpdateSceneModel();
    void DisconnectAllTrackers();
    void ReconnectAllTrackers();

signals:
    void LightNodeStateChanged(int newState);

private:
    void DeleteInstanceTree(InstanceNode& instanceNode);
    void SetRoot();
    void SetLight();
    void SetConcentrator();
    InstanceNode* AddInstanceNode(InstanceNode& instanceNodeParent, SoNode* separatorKit);
    void GenerateInstanceTree(InstanceNode& instanceParent);
    void GenerateTShapeKitSubTree(InstanceNode& instanceNodeParent, SoNode* parentNode);
//    void GenerateTAnalyzerKitSubTree( InstanceNode& instanceNodeParent, SoNode* parentNode );
//    void GenerateSoNodeKitListPartSubTree( InstanceNode& instanceNodeParent, SoNode* parentNode );
    void GenerateTSeparatorKitSubTree(InstanceNode& instanceNodeParent, SoNode* parentNode);

private:
    SoSeparator* m_coinRoot;
    TSceneKit* m_coinScene;
    InstanceNode* m_instanceRoot;
    InstanceNode* m_instanceConcentrator;
    std::map< SoNode*, QList<InstanceNode*> > m_mapCoinQt;
};
