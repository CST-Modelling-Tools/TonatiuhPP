#include "SceneModel.h"

#include <QIcon>
#include <QMessageBox>

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodekits/SoNodeKitListPart.h>
#include <Inventor/nodekits/SoSceneKit.h>
#include <Inventor/nodes/SoSelection.h>

#include "kernel/material/MaterialAbstract.h"
#include "kernel/run/InstanceNode.h"
#include "kernel/scene/TSceneKit.h"
#include "kernel/scene/TSeparatorKit.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/ShapeAbstract.h"
#include "kernel/sun/TLightKit.h"
#include "kernel/trackers/TrackerAbstract.h"
#include "libraries/geometry/gcf.h"
#include "tree/SoPathVariant.h"

/*!
 * Creates an empty model.
 */
SceneModel::SceneModel(QObject* parent):
    QAbstractItemModel(parent), m_coinRoot(0), m_coinScene(0), m_instanceRoot(0)
{

}

/*!
 * Destroys the model.
 */
SceneModel::~SceneModel()
{
    Clear();
}

/*!
 * Clears the model scene
 *
 * Clears nodes from the model.
 */
void SceneModel::Clear()
{
    DeleteInstanceTree(*m_instanceRoot);

    delete m_instanceRoot;
    m_instanceRoot = 0;
}

/*!
 * Sets \a coinRoot as the model root.
 */
void SceneModel::setSceneRoot(SoSeparator& coinRoot)
{
    m_coinRoot = &coinRoot;
    m_mapCoinQt.clear();
}

/*!
 * Sets the model scene to the given \a coinScene.
 *
 * Creates nodes for the model to the scene subnodes.
 */
void SceneModel::setSceneKit(TSceneKit& coinScene)
{
    beginResetModel();

    if (m_instanceRoot) Clear();
    m_mapCoinQt.clear();
    m_coinScene = 0;
    m_coinScene = &coinScene;
    SetRoot();

    endResetModel();
}

void SceneModel::SetRoot()
{
    m_instanceRoot = new InstanceNode(m_coinScene);

    m_mapCoinQt[m_coinScene].append(m_instanceRoot);

    if (TLightKit* lightKit = static_cast<TLightKit*>(m_coinScene->getPart("lightList[0]", false)))
        InsertLightNode(*lightKit);

    SoNodeKitListPart* coinPartList = static_cast<SoNodeKitListPart*>(m_coinScene->getPart("childList", true));

    //TSeparatorKit* separatorKit;
    if (coinPartList->getNumChildren() == 0)
    {
        // Sun node
        TSeparatorKit* nodeSun = new TSeparatorKit();
        nodeSun->setName("SunNode");
        nodeSun->setSearchingChildren(true);
        coinPartList->addChild(nodeSun);
        InstanceNode* instanceSun = AddInstanceNode(*m_instanceRoot, nodeSun);
        SoNodeKitListPart* childrenSun = static_cast<SoNodeKitListPart*>(nodeSun->getPart("childList", true));

        // Layout node
        TSeparatorKit* nodeLayout = new TSeparatorKit();
        nodeLayout->setName("Layout");
        childrenSun->addChild(nodeLayout);
        nodeLayout->setSearchingChildren(true);
        m_instanceLayout = AddInstanceNode(*instanceSun, nodeLayout);
    }
    else
    {
        TSeparatorKit* nodeSun = static_cast<TSeparatorKit*>(coinPartList->getChild(0));
        if (!nodeSun) return;
        InstanceNode* instanceSun = AddInstanceNode(*m_instanceRoot, nodeSun);
        SoNodeKitListPart* childrenSun = static_cast<SoNodeKitListPart*>(nodeSun->getPart("childList", true));
        if (!childrenSun) return;

        TSeparatorKit* nodeLayout = static_cast<TSeparatorKit*>(childrenSun->getChild(0));
        if (!nodeLayout) return;
        m_instanceLayout = AddInstanceNode(*instanceSun, nodeLayout);
        GenerateInstanceTree(*m_instanceLayout);
    }
}

InstanceNode* SceneModel::AddInstanceNode(InstanceNode& parent, SoNode* node)
{
    InstanceNode* instance = new InstanceNode(node);
    parent.addChild(instance);
    m_mapCoinQt[node].append(instance);
    return instance;
}

void SceneModel::GenerateInstanceTree(InstanceNode& instance)
{
    SoNode* node = instance.getNode();
    if (!node->getTypeId().isDerivedFrom(SoBaseKit::getClassTypeId() ) ) return;

    if (TShapeKit* shapeKit = dynamic_cast<TShapeKit*>(node))
    {
        SoNode* shape = shapeKit->getPart("shape", false);
        if (shape)
            AddInstanceNode(instance, shape);

        SoNode* material = shapeKit->getPart("appearance.material", false);
        if (material)
            AddInstanceNode(instance, material);
    }
    else
    {
        SoBaseKit* separatorKit = static_cast<SoBaseKit*>(node);
        if (!separatorKit) return;

        SoNode* tracker = separatorKit->getPart("tracker", false);
        if (tracker)
            AddInstanceNode(instance, tracker);

        SoNodeKitListPart* coinPartList = static_cast<SoNodeKitListPart*>(separatorKit->getPart("childList", false));
        if (!coinPartList) return;

        for (int n = 0; n < coinPartList->getNumChildren(); ++n)
        {
            SoBaseKit* coinChild = static_cast<SoBaseKit*>(coinPartList->getChild(n));
            InstanceNode* instanceChild = AddInstanceNode(instance, coinChild);
            GenerateInstanceTree(*instanceChild);
        }
    }
}

void SceneModel::DeleteInstanceTree(InstanceNode& instance)
{
    while (instance.children.count() > 0)
    {
        InstanceNode* child = instance.children[instance.children.count() - 1];
        DeleteInstanceTree(*child);
        delete child;
    }

//    for (InstanceNode* child : instance.children)
//    {
//        DeleteInstanceTree(*child);
//        delete child;
//    }

    QList<InstanceNode*>& instanceList = m_mapCoinQt[instance.getNode()];
    int q = instanceList.indexOf(&instance);
    instanceList.removeAt(q);

    InstanceNode* instanceParent = instance.getParent();
    if (instanceParent)
    {
        int row = instanceParent->children.indexOf(&instance);
        instanceParent->children.remove(row);
    }
}

QModelIndex SceneModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!m_instanceRoot) return QModelIndex();
    InstanceNode* instance = getInstance(parent);
    return createIndex(row, column, instance->children[row]);
}

int SceneModel::rowCount(const QModelIndex& index) const
{
    InstanceNode* instance = getInstance(index);
    return instance ? instance->children.count() : 0;
}

int SceneModel::columnCount(const QModelIndex& /*index*/) const
{
    return 1;
}

QModelIndex SceneModel::parent(const QModelIndex& index) const
{
    InstanceNode* instance = getInstance(index);
    if (!instance) return QModelIndex();

    InstanceNode* instanceParent = instance->getParent();
    if (!instanceParent) return QModelIndex();

    InstanceNode* instanceGrandparent = instanceParent->getParent();
    if (!instanceGrandparent) return QModelIndex();

    int row = instanceGrandparent->children.indexOf(instanceParent);
    return createIndex(row, 0, instanceParent);
}

QVariant SceneModel::data(const QModelIndex& index, int role) const
{
    if (role != Qt::DisplayRole && role != Qt::UserRole && role != Qt::DecorationRole) return QVariant();

    InstanceNode* instance = getInstance(index);
    if (!instance) return QVariant();

    SoNode* node = instance->getNode();
    if (!node) return QVariant();

    if (index.column() != 0) return QVariant();

    if (role == Qt::DisplayRole)
    {
        QString name;
        if (node->getName() == SbName())
            name = node->getTypeId().getName().getString();
        else
            name = node->getName().getString();

        SoSearchAction action;
        action.setNode(node);
        action.setInterest(SoSearchAction::ALL);
        action.apply(m_coinRoot);

        int count = action.getPaths().getLength();
        if (count > 1)
            name = QString("%1 [%2]").arg(name).arg(count);

        return name;
    }
    if (role == Qt::UserRole)
    {
        SoNodeKitPath* coinKitPath = PathFromIndex(index);
        if (coinKitPath)
        {
            SoPathVariant pathWrapper(coinKitPath);
            return QVariant::fromValue(pathWrapper);
        }
        coinKitPath->unref();
    }
    if (role == Qt::DecorationRole)
    {
        if (node->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId()))
        {
            return QIcon(":/images/scene/nodeGroup.png");
        }
        else if (node->getTypeId().isDerivedFrom(TShapeKit::getClassTypeId()))
        {
            return QIcon(":/images/scene/nodeShape.png");
        }
        else if (node->getTypeId().isDerivedFrom(SoShape::getClassTypeId()))
        {
            ShapeAbstract* shape = static_cast<ShapeAbstract*>(node);
            return QIcon(shape->getTypeIcon());
        }
        else if (node->getTypeId().isDerivedFrom(MaterialAbstract::getClassTypeId()))
        {
            MaterialAbstract* material = static_cast<MaterialAbstract*>(node);
            return QIcon(material->getTypeIcon());
        }
        else if (node->getTypeId().isDerivedFrom(TrackerAbstract::getClassTypeId()))
        {
            TrackerAbstract* tracker = static_cast<TrackerAbstract*>(node);
            return QIcon(tracker->getTypeIcon());
        }
        else if (node->getTypeId().isDerivedFrom(TLightKit::getClassTypeId()))
        {
            return QIcon(":/images/scene/environmentSun.png");
        }
    }

    return QVariant();
}

QVariant SceneModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            if (section == 0) return "Node";
        }
    }
    return QVariant();
}

Qt::ItemFlags SceneModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);
    if (!index.isValid()) return defaultFlags;
    if (index.column() > 0) return defaultFlags;
    return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable | defaultFlags;
}

Qt::DropActions SceneModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

Qt::DropActions SceneModel::supportedDragActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

InstanceNode* SceneModel::getInstance(const QModelIndex& index) const
{
    if (index.isValid())
        return (InstanceNode*) index.internalPointer();
    else
        return m_instanceRoot;
}

bool SceneModel::SetNodeName(SoNode* node, QString name)
{
    SbName sbname(name.toStdString().c_str());

    for (InstanceNode* instance : m_mapCoinQt[node])
    {
        for (InstanceNode* sibling : instance->getParent()->children)
        {
            if (sibling == instance) continue;
            if (sibling->getNode()->getName() == sbname)
                return false;
        }
    }

    node->setName(sbname);
    emit layoutChanged();
    return true;
}

/**
 * Returns the index of item with the \a nodeUrl.
 *
 * If \a nodeUrl is not a valid node url, the function returns root node index.
 *
 *
 * \sa IndexFromNodeUrl, NodeFromIndex, PathFromIndex.
**/
QModelIndex SceneModel::IndexFromNodeUrl(QString url) const
{
    QStringList path = url.split("/", QString::SkipEmptyParts);

    if (path.size() == 1 && path[0] == "Light")
        return index(0, 0);

    if (path.size() > 0)
    {
        QString nodeName = path.last();
        path.removeLast();
        QString parentURL = QString("//") + path.join("/");

        QModelIndex parentIndex = IndexFromNodeUrl(parentURL);
        InstanceNode* parentNode = getInstance(parentIndex);

        int child = 0;
        int row = -1;
        while (child < parentNode->children.count())
        {
            if (parentNode->children[child]->getNode()->getName() == SbName( nodeName.toStdString().c_str() ) )
            {
                row = child;
                break;
            }
            child++;
        }
        if (row < 0) return QModelIndex();
        //if(nodeName == ".") return index(row+1, 0, parentIndex );
        return index(row, 0, parentIndex);
    }
    else
        return QModelIndex();
}

/**
 * Returns the index of item with the \a coinNodePath.
 *
 * If \a coinNodePath is not a valid node path, the function returns root node index.
 *
 *
 * \sa IndexFromNodeUrl, NodeFromIndex, PathFromIndex.
**/
QModelIndex SceneModel::IndexFromPath( const SoNodeKitPath& coinNodePath ) const
{
    SoBaseKit* coinNode = static_cast< SoBaseKit* >( coinNodePath.getTail() );
    if( !coinNode ) gcf::SevereError( "IndexFromPath Null coinNode." );

    if( coinNode->getTypeId().getName().getString() == SbName("TLightKit") ) return index( 0, 0 );

    if ( coinNodePath.getLength() > 1 )
    {
        SoBaseKit* coinParent = static_cast< SoBaseKit* >( coinNodePath.getNodeFromTail(1) );
        if( !coinParent ) gcf::SevereError( "IndexFromPath Null coinParent." );

        if( coinParent->getTypeId().getName() == SbName( "TLightKit") ) return index( 0, 0 );

        SoNodeKitListPart* coinPartList = static_cast< SoNodeKitListPart* >( coinParent->getPart( "childList", true ) );
        if( !coinPartList ) gcf::SevereError( "IndexFromPath Null coinPartList." );

        int row = coinPartList->findChild(coinNode);
        if( coinParent->getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) )
        {
            if (coinParent->getPart("tracker", false)) row++;
        }
        if( coinNodePath.getNodeFromTail(1)->getTypeId().isDerivedFrom( SoSceneKit::getClassTypeId() ) )
        {
            int child = 0;
            while( child < m_instanceRoot->children.count() )
            {
                if ( m_instanceRoot->children[child]->getNode() == coinNode )
                    break;
                child++;
            }
            return index( child, 0 );
        }
        else
        {
            SoNodeKitPath* parentPath = static_cast< SoNodeKitPath* > ( coinNodePath.copy( 0, 0 ) );
            parentPath->truncate( parentPath->getLength() - 1 );
                        QModelIndex parentIndex = IndexFromPath( *parentPath );
            QModelIndex childIndex =  index (row, 0, parentIndex );
            return childIndex;
        }

    }
    else
        return QModelIndex();
}

SoNodeKitPath* SceneModel::PathFromIndex( const QModelIndex& modelIndex ) const
{
    SoNode* coinNode = getInstance( modelIndex )->getNode();
    if (!coinNode->getTypeId().isDerivedFrom( SoBaseKit::getClassTypeId() ) )
    {
        QModelIndex parentIndex = parent( modelIndex );
        SoNodeKitPath* parentPath = PathFromIndex( parentIndex );
        return parentPath;
    }
    else
    {
        SoSearchAction coinSearch;
        coinSearch.setNode( m_coinScene );
        coinSearch.setInterest( SoSearchAction::FIRST);
        coinSearch.apply( m_coinRoot );

        SoPath* coinScenePath = coinSearch.getPath( );
        if( !coinScenePath ) gcf::SevereError( "PathFromIndex Null coinScenePath." );

        SoNodeKitPath* nodePath = static_cast< SoNodeKitPath* > ( coinScenePath );
        if( !nodePath ) gcf::SevereError( "PathFromIndex Null nodePath." );

        nodePath->ref();

        InstanceNode* instanceNode = getInstance( modelIndex );

        SoNodeList nodeList;
        while(instanceNode->getParent() )
        {
            nodeList.append( instanceNode->getNode() );
            instanceNode = instanceNode->getParent();
        }

        for( int i = nodeList.getLength(); i > 0; --i )
        {
            SoBaseKit* coinNode = static_cast< SoBaseKit* >( nodeList[i-1] );
            if ( coinNode )    nodePath->append( coinNode );

        }
        return nodePath;
    }
}

/*!
 * Insert a light node to the model. If the model has an other light node, the previous node
 * will be deleted.
 */
void SceneModel::InsertLightNode(TLightKit& lightKit)
{
    SoNodeKitListPart* lightList =
        static_cast<SoNodeKitListPart*>(m_coinScene->getPart("lightList", true));

    if (lightList->getNumChildren() > 0)
        if (m_instanceRoot->children.size() > 0)
            m_instanceRoot->children.remove(0);

    m_coinScene->setPart("lightList[0]", &lightKit);

    InstanceNode* instance = new InstanceNode(&lightKit);
    m_instanceRoot->insertChild(0, instance);

    emit LightNodeStateChanged(1);
    emit layoutChanged();
}

void SceneModel::RemoveLightNode(TLightKit& lightKit)
{
    SoNodeKitListPart* lightList =
        static_cast<SoNodeKitListPart*>(m_coinScene->getPart("lightList", true));
    if (lightList) lightList->removeChild(&lightKit);
    m_instanceRoot->children.remove(0);

    emit LightNodeStateChanged(0);
    emit layoutChanged();
}

int SceneModel::InsertCoinNode(SoNode& coinChild, SoBaseKit& coinParent)
{
    int row = -1;
    if (coinParent.getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) )
    {
        if( coinChild.getTypeId().isDerivedFrom( TrackerAbstract::getClassTypeId() ) )
        {
            coinParent.setPart("tracker", &coinChild);
            row = 0;
        }
        else
        {
            SoNodeKitListPart* coinPartList = static_cast< SoNodeKitListPart* >( coinParent.getPart( "childList", true ) );
            if ( !coinPartList ) return -1;

            row = coinPartList->getNumChildren();

            if( coinChild.getTypeId().isDerivedFrom( SoBaseKit::getClassTypeId() ) )
            {
                SoBaseKit* childKit = static_cast< SoBaseKit* >( &coinChild );
                coinPartList->addChild( childKit );
                childKit->setSearchingChildren( true );

                if( coinParent.getPart( "tracker", false ) ) row++;
            }
        }
    }
    else
    {
        TShapeKit* shapeKit = static_cast< TShapeKit* > ( &coinParent );
        if( shapeKit->getPart( "shape", false ) ) row++;
        if( shapeKit->getPart( "appearance.material", false ) ) row++;
    }

    QList<InstanceNode*>& instanceListParent = m_mapCoinQt[ &coinParent ];
    for ( int index = 0; index < instanceListParent.count(); index++ )
    {
        InstanceNode* instanceParent = instanceListParent[index];
        InstanceNode* instanceChild = new InstanceNode( &coinChild );
        instanceParent->insertChild( row, instanceChild );

        //Inserting InstanceNode in the map
        QList< InstanceNode* > instanceNodeList;
            m_mapCoinQt.insert( std::make_pair( &coinChild, instanceNodeList ) );
        m_mapCoinQt[ &coinChild ].append( instanceChild );

        GenerateInstanceTree( *instanceChild );
    }
    emit layoutChanged();

    return row;
}

void SceneModel::RemoveCoinNode( int row, SoBaseKit& coinParent )
{
    if (coinParent.getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) )
    {
        if( ( row == 0 ) && coinParent.getPart( "tracker", false ) )
        {
            coinParent.setPart( "tracker", NULL );
        }
        else
        {
            SoNodeKitListPart* coinPartList = static_cast< SoNodeKitListPart* >( coinParent.getPart( "childList", false ) );
            if ( coinPartList )
            {
                if( coinParent.getPart( "tracker", false ) )    coinPartList->removeChild( row - 1 );
                else coinPartList->removeChild( row );
            }
        }
    }

    QList<InstanceNode*> instanceListParent = m_mapCoinQt[ &coinParent ];

    for( int index = 0; index< instanceListParent.size(); ++index )
    {
        InstanceNode* instanceParent = instanceListParent[index];
        InstanceNode* instanceNode = instanceParent->children[row];
        instanceParent->children.remove(row);

        QList<InstanceNode*>& instanceList = m_mapCoinQt[ instanceNode->getNode()];
        instanceList.removeAt( instanceList.indexOf( instanceNode ) );
    }
    emit layoutChanged();
}

/**
 * Cuts the \a row child from \a coinParent node.
 *
 * Returns whether the cut is successfully done.
**/
bool SceneModel::Cut(SoBaseKit& coinParent, int row)
{
    if (row < 0) return false;

    QList<InstanceNode*> instanceListParent = m_mapCoinQt[&coinParent];
    InstanceNode* instanceParent = instanceListParent[0];

    SoNode* coinChild = instanceParent->children[row]->getNode();

    QModelIndex parentIndex = createIndex(instanceParent->getParent()->children.indexOf(instanceParent), 0, instanceParent);
    beginRemoveRows(parentIndex, row, row);

    if( !coinChild->getTypeId().isDerivedFrom( SoBaseKit::getClassTypeId() ) )
    {
        SbString partName = coinParent.getPartString( coinChild );
        if( partName.getLength() == 0 ) partName = "material";

        coinParent.setPart( partName, NULL );
    }
    else
    {
        if( coinParent.getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) )
        {

            SoNodeKitListPart* coinPartList = static_cast< SoNodeKitListPart* >( coinParent.getPart( "childList", false ) );
            if( !coinPartList ) return false;


            if( coinParent.getPart( "tracker", false ) )
            {
                coinChild = coinPartList->getChild( row-1 );
                coinPartList->removeChild( row -1 );
            }
            else
            {
                coinChild = coinPartList->getChild( row );
                coinPartList->removeChild( row );
            }

            if( !coinChild ) gcf::SevereError( "SceneModel::Cut Null coinChild pointer" );

        }
    }

    QList<InstanceNode*> instanceList = m_mapCoinQt[ coinChild ];

    // Test if the node is shared
    if( instanceList.size() == 1 )
    {
        InstanceNode* instanceNode = instanceList[0];
        DeleteInstanceTree( *instanceNode );
        //delete instanceNode;
        //instanceNode = 0;
    }
    else
    {
        QList<InstanceNode*> instanceListParent = m_mapCoinQt[ &coinParent ];
        if( instanceListParent.size() == 1 )
        {
            InstanceNode* instanceParent = instanceListParent[0];
            InstanceNode* instanceNode = instanceParent->children[row];
            DeleteInstanceTree( *instanceNode );
            //delete instanceNode;
            //instanceNode = 0;
        }
        else
        {
            for( int index = 0; index< instanceListParent.size(); ++index )
            {
                InstanceNode* instanceParent = instanceListParent[index];
                InstanceNode* instanceNode = instanceParent->children[row];
                DeleteInstanceTree( *instanceNode );
                //delete instanceNode;
                //instanceNode = 0;
            }
        }
    }

    endRemoveRows();
    emit layoutChanged();

    return true;
}

/**
 * Adds a child to \a coinParent as \a row child. If \a type is tgc::Copied, the added child is a copy of \a coinNode. But the type is tgc::Shared the node is shared with previous parent.
 * Return true if the paste action is correctly done. Otherwise returns false.
**/
bool SceneModel::Paste( tgc::PasteType type, SoBaseKit& coinParent, SoNode& coinNode, int row )
{
    SoNode* coinChild = 0;
    SoNode* pCoinParent = 0;
    pCoinParent = &coinParent;

    if (type == tgc::Shared) coinChild = &coinNode;

    switch (type)
    {
        case tgc::Copied :
            coinChild = static_cast< SoNode* >( coinNode.copy( true ) );
            break;

        case tgc::Shared :
        default :
            coinChild = &coinNode;
            break;
    }
    if ( !coinChild->getTypeId().isDerivedFrom( SoBaseKit::getClassTypeId() ) )
    {
        if ( coinChild->getTypeId().isDerivedFrom( TrackerAbstract::getClassTypeId() ) )
        {
            TSeparatorKit* separatorKit = static_cast< TSeparatorKit* >( pCoinParent );
            TrackerAbstract* tracker = static_cast< TrackerAbstract* >( separatorKit->getPart( "tracker", false ) );
            if (tracker)
            {
                QMessageBox::warning( 0, tr( "Tonatiuh warning" ), tr( "This TSeparatorKit already contains a tracker" ) );
                return false;
            }
            coinParent.setPart( "tracker", coinChild );

        }
        if ( coinChild->getTypeId().isDerivedFrom( SoShape::getClassTypeId() ) )
        {
            TShapeKit* shapeKit = static_cast< TShapeKit* >( pCoinParent );
            if(!shapeKit)    return false;
            ShapeAbstract* shape = static_cast< ShapeAbstract* >( shapeKit->getPart( "shape", false ) );

            if (shape)
            {
                QMessageBox::warning( 0, tr( "Tonatiuh warning" ), tr( "This TShapeKit already contains a shape" ) );
                return false;
            }
            coinParent.setPart("shape", coinChild );
        }
        if ( coinChild->getTypeId().isDerivedFrom( SoMaterial::getClassTypeId() ) )
        {
            TShapeKit* shapeKit = static_cast< TShapeKit* >( pCoinParent );
            if(!shapeKit)    return false;
            MaterialAbstract* material = static_cast< MaterialAbstract* >( shapeKit->getPart( "material", false ) );
            if (material)
            {
                QMessageBox::warning( 0, tr( "Tonatiuh warning" ), tr( "This TShapeKit already contains a material" ) );
                return false;
            }
            coinParent.setPart("material", coinChild );
        }

    }
    else
    {
        SoNodeKitListPart* coinPartList = static_cast< SoNodeKitListPart* >( coinParent.getPart( "childList", true ) );
        if( !coinPartList ) gcf::SevereError( "SceneModel::Paste Null coinPartList pointer" );
        if( coinParent.getPart( "tracker", false ) ) coinPartList->insertChild( coinChild, row -1 );
        else coinPartList->insertChild( coinChild, row );
    }

    QList<InstanceNode*>& instanceListParent = m_mapCoinQt[ &coinParent ];
    for ( int index = 0; index < instanceListParent.count(); ++index )
    {
        InstanceNode* instanceParent = instanceListParent[index];
        InstanceNode* instanceChild = new InstanceNode( coinChild );
        instanceParent->insertChild( row, instanceChild );

        //Inserting InstanceNode in the map
        QList< InstanceNode* > instanceNodeList;
            m_mapCoinQt.insert( std::make_pair( coinChild, instanceNodeList ) );
        m_mapCoinQt[ coinChild ].append( instanceChild );
        GenerateInstanceTree( *instanceChild );
    }

    emit layoutChanged();
    return true;
}

void SceneModel::UpdateSceneModel()
{
    emit layoutChanged();
}
