#pragma once

#include <vector>

#include <QVector>
#include <QMutex>
#include <Inventor/SbBox3f.h>
#include <Inventor/SbMatrix.h>

#include "libraries/geometry/BBox.h"
#include "libraries/geometry/Transform.h"

class RandomDeviate;
class Ray;
class SoNode;
class TLightKit;
class SceneModel;


//!  InstanceNode class represents a instance of a node in the scene.
/*! In a scene, a node can be shared by more that one parent. Each of these shared instances is represented in a scene as a InstanceNode object.
 * Any change made whitin a shared node is reflected in all node's InstanceNode.
 */

class InstanceNode
{
public:
    InstanceNode(SoNode* node);
    ~InstanceNode();

    void SetNode(SoNode* node);
    void SetParent(InstanceNode* parent);
    void AddChild(InstanceNode* child);
    void InsertChild(int row, InstanceNode* instanceChild);

    SoNode* GetNode() const;
    InstanceNode* GetParent() const;
    QString GetNodeURL() const;
    void Print(int level) const;

    bool Intersect(const Ray& ray, RandomDeviate& rand, bool* isShapeFront, InstanceNode** modelNode, Ray* outputRay);

    //template<class T> void RecursivlyApply(void (T::*func)(void));
    //template<class T,class Param1> void RecursivlyApply(void (T::*func)(Param1),Param1 param1);
    //template<class T,class Param1> void RecursivlyApplyWithMto(void (T::*func)(Param1),Param1 param1);
    //template<class T,class Param1,class Param2> void RecursivlyApplyWithMto(void (T::*func)(Param1,Param2),Param1 param1,Param1 param2);

    void DisconnectAllTrackers();
    void ReconnectAllTrackers(TLightKit* coinLight);
    void SetAimingPointRelativity(bool relative);
    void extendBoxForLight(SbBox3f* extendedBox);
    BBox GetIntersectionBBox();
    Transform GetIntersectionTransform();
    void SetIntersectionBBox(BBox nodeBBox);
    void SetIntersectionTransform(Transform nodeTransform);

    QVector< InstanceNode* > children;

private:
    SoNode* m_coinNode;
    InstanceNode* m_parent;
    BBox m_bbox;
    Transform m_transformWTO;
    Transform m_transformOTW;
};

QDataStream& operator<< (QDataStream& s, const InstanceNode& node);
QDataStream& operator>> (QDataStream& s, const InstanceNode& node);
bool operator==(const InstanceNode& thisNode,const InstanceNode& otherNode);

inline void InstanceNode::SetParent(InstanceNode* parent)
{
    m_parent = parent;
}

inline void InstanceNode::SetNode(SoNode* node)
{
    m_coinNode = node;
}

inline SoNode* InstanceNode::GetNode() const
{
    return m_coinNode;
}

/**
 * Returns parent instance.
 */
inline InstanceNode* InstanceNode::GetParent() const
{
    return m_parent;
}

